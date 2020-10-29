#include <QDebug>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include "referencedialog.h"
#include "rtoeditdialog.h"
#include "rtotablemodel.h"
#include "weekcalc.h"
#include "rtodialog.h"
#include "trmessagebox.h"

RTOEditDialog::RTOEditDialog(QTableView *table, QAbstractItemModel *model, QWidget *parent)
{
    parentWidget = parent;
    mainTableView = table;
    //mainModel = table->model();
    mainModel = model;

    periodModel = new SqlModel;
    //periodModel->setQuery("SELECT id, year, week, name");

    taskModel = new SqlModel;
    taskModel->setQuery("SELECT id, name, dev_name_id FROM task_list WHERE dev_name_id IN (SELECT dev_name_id FROM dev_list)");
    deviceModel = new SqlModel;

    //deviceModel->setQuery("SELECT id, printf(\"%s — %s\", kks, part_num) AS device FROM devices_list_view");
    //deviceComboBox->model()->data(deviceComboBox->model()->index(i, 0)).toInt()
    taskEditLineEdits << Q_NULLPTR;

    periodHBoxLayout = new QHBoxLayout;
    taskHBoxLayout = new QHBoxLayout;
    deviceHBoxLayout = new QHBoxLayout;
    statusHBoxLayout = new QHBoxLayout;

    taskEditFormLayout = new QFormLayout;

    periodComboBox = new QComboBox;
    periodComboBox->setFixedWidth(180);
    //periodComboBox->setModel(periodModel);
    //periodComboBox->setModelColumn(0);

    periodLineEdit = new QLineEdit;
    periodLineEdit->setFixedWidth(180);
    periodLineEdit->setHidden(true);
    periodLineEdit->setReadOnly(true);

    periodHBoxLayout->addWidget(periodComboBox);
    periodHBoxLayout->addWidget(periodLineEdit);

    taskComboBox = new QComboBox;
    taskComboBox->setFixedWidth(280);
    taskComboBox->setModel(taskModel);
    taskComboBox->setModelColumn(1);

    taskLineEdit = new QLineEdit;
    taskLineEdit->setFixedWidth(280);
    taskLineEdit->setHidden(true);
    taskLineEdit->setReadOnly(true);

    taskHBoxLayout->addWidget(taskComboBox);
    taskHBoxLayout->addWidget(taskLineEdit);

    deviceComboBox = new QComboBox;
    deviceComboBox->setFixedWidth(280);
    deviceModel->setQuery("SELECT id, kks || ' — ' || part_num AS device FROM devices_list_view WHERE dev_name_id = " + \
                          taskComboBox->model()->data(taskComboBox->model()->index(taskComboBox->currentIndex(), 2)).toString());

    //qDebug() << taskComboBox->model()->data(taskComboBox->model()->index(taskComboBox->currentIndex(), 2)).toString();
    deviceComboBox->setModel(deviceModel);
    deviceComboBox->setModelColumn(1);

    deviceLineEdit = new QLineEdit;
    deviceLineEdit->setFixedWidth(280);
    deviceLineEdit->setHidden(true);
    deviceLineEdit->setReadOnly(true);

    deviceHBoxLayout->addWidget(deviceComboBox);
    deviceHBoxLayout->addWidget(deviceLineEdit);

    statusComboBox = new QComboBox;
    statusComboBox->setFixedWidth(150);
    statusComboBox->addItem(tr("Not completed"), 0);
    statusComboBox->addItem(tr("Completed"), 1);

    statusLabel = new QLabel;
    statusLabel->setHidden(true);

    statusHBoxLayout->addWidget(statusComboBox);
    statusHBoxLayout->addWidget(statusLabel);

    commentTextEdit = new QTextEdit;
    commentTextEdit->setFixedHeight(50);

    taskEditFormLayout->addRow(tr("Period:"), periodHBoxLayout);
    taskEditFormLayout->addRow(tr("Task:"), taskHBoxLayout);
    taskEditFormLayout->addRow(tr("Device:"), deviceHBoxLayout);
    taskEditFormLayout->addRow(tr("Status:"), statusHBoxLayout);
    taskEditFormLayout->addRow(tr("Comment:"), commentTextEdit);

    setMainLayout(taskEditFormLayout);

    setWindowFlags((this->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(500, 260));

    setValidatorFields(taskEditLineEdits);
    setGroupBoxTitle(tr("Task", "rtoeditdialog"));

    connect(periodLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(this);} );
    connect(taskComboBox, &QComboBox::currentTextChanged, [=](){this->setEdited(this);} );
    connect(deviceComboBox, &QComboBox::currentTextChanged, [=](){this->setEdited(this);} );
    connect(statusComboBox, &QComboBox::currentTextChanged, [=](){this->setEdited(this);} );
    connect(commentTextEdit, &QTextEdit::textChanged, [=](){this->setEdited(this);} );

    connect(taskComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(taskModelChanged()));    
}

QAbstractItemModel *RTOEditDialog::getModel()
{
    return mainModel;
}

SqlModel *RTOEditDialog::getTaskModel()
{
    return taskModel;
}

SqlModel *RTOEditDialog::getDeviceModel()
{
    return deviceModel;
}

void RTOEditDialog::setEdited(ValDialog *dialog)
{
    dialog->setEdited();
    dialog->setWindowTitle(tr("TOiK") + "*");
}

void RTOEditDialog::taskModelChanged()
{
    deviceModel->setQuery("SELECT id, kks || ' — ' || part_num AS device FROM devices_list_view WHERE dev_name_id = " + \
                          taskComboBox->model()->data(taskComboBox->model()->index(taskComboBox->currentIndex(), 2)).toString());
    //qDebug() << taskComboBox->model()->data(taskComboBox->model()->index(taskComboBox->currentIndex(), 2)).toString();
    deviceComboBox->setModel(deviceModel);
    deviceComboBox->setModelColumn(1);
    deviceComboBox->setCurrentIndex(0);
}

void RTOEditDialog::taskModelUpdate()
{
    ((SqlModel *) taskComboBox->model())->setQuery(((SqlModel *) taskComboBox->model())->query().lastQuery());
    taskComboBox->setCurrentIndex(0);
}

void RTOEditDialog::addAct()
{
    int i, result;
    QDate date;
    weekDate_s weekDate, selWeekDate;;
    QVariant weekDateVariant = QVariant::fromValue(weekDate);
    QString sql_query;
    QString curTaskId, curDeviceId;
    QSqlQuery query;

    selWeekDate.year = date.currentDate().year();
    selWeekDate.week = date.currentDate().weekNumber();

    periodComboBox->clear();
    date.setDate(selWeekDate.year, 1, 1);
    date.setDate(date.addDays((selWeekDate.week - 60) * 7).year(), \
                 date.addDays((selWeekDate.week - 60) * 7).month(), \
                 date.addDays((selWeekDate.week - 60) * 7).day());

    for (i = 0; i < 60 + 80; i++) {
        weekDate.year = date.year();
        weekDate.week = date.weekNumber();
        weekDateVariant.setValue(weekDate);

        int yearOfWeek;
        date.weekNumber(&yearOfWeek);
        periodComboBox->addItem(QString::number(date.year()) + ", " + \
                                QString::number(date.weekNumber()) + " : " + \
                                WeekCalc::getWeekRange(date.weekNumber(), date.year(), yearOfWeek), weekDateVariant);

        date.setDate(date.addDays(7).year(), date.addDays(7).month(), date.addDays(7).day());
    }
    // = qvariant_cast<weekDate_s>(weekDateVariant);
    for (i = 0; i < 60 + 80; i++) {
        if ( (periodComboBox->itemData(i).value<weekDate_s>().year == selWeekDate.year) && \
             (periodComboBox->itemData(i).value<weekDate_s>().week == selWeekDate.week) ){
            periodComboBox->setCurrentIndex(i);
        }
    }

    statusComboBox->setCurrentIndex(0);

    commentTextEdit->setText("");

    setWindowTitle(tr("TOiK"));
    result = exec();

    /*
    taskComboBox->setVisible(true);
    taskLineEdit->setHidden(true);

    deviceComboBox->setVisible(true);
    deviceLineEdit->setHidden(true);
    */


    //((SqlModel *) deviceComboBox->model())->setQuery(((SqlModel *) deviceComboBox->model())->query().lastQuery());

    curTaskId = taskComboBox->model()->data(taskComboBox->model()->index(taskComboBox->currentIndex(), 0)).toString();
    curDeviceId = deviceComboBox->model()->data(deviceComboBox->model()->index(deviceComboBox->currentIndex(), 0)).toString();

    if ((result == QDialog::Accepted) && isEdited()) {
        sql_query = "INSERT INTO rto_list (week, task_id, status, comment, dev_id, year) VALUES (" + \
                                          QString::number(periodComboBox->currentData().value<weekDate_s>().week) + ", " + \
                                          curTaskId + ", " + \
                                          statusComboBox->itemData(statusComboBox->currentIndex()).toString() + ", \"" + \
                                          commentTextEdit->toPlainText() + "\", " + \
                                          curDeviceId + ", " + \
                                          QString::number(periodComboBox->currentData().value<weekDate_s>().year) + ")";
        //qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            emit dbChanged();
            emit ((RTODialog *) parentWidget)->selectionChanged();
            //mainTableView->selectionModel()->setCurrentIndex(curIdx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
    }
    if (isEdited()) {
        setWindowTitle(tr("TOiK"));
    }

    //mainTableView->setFocus();
}

void RTOEditDialog::editAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int i, result;
    QDate date;
    weekDate_s weekDate, selWeekDate;;
    QVariant weekDateVariant = QVariant::fromValue(weekDate);
    QString sql_query;
    QString curTaskId, curDeviceId;
    QSqlQuery query;

    selWeekDate.year = mainModel->index(rowidx, RTO_YEAR).data().toInt();
    selWeekDate.week = mainModel->index(rowidx, RTO_WEEK).data().toInt();

    taskComboBox->setHidden(true);
    taskLineEdit->setVisible(true);

    deviceComboBox->setHidden(true);
    deviceLineEdit->setVisible(true);

    periodComboBox->clear();
    date.setDate(selWeekDate.year, 1, 1);
    date.setDate(date.addDays((selWeekDate.week - 60) * 7).year(), \
                 date.addDays((selWeekDate.week - 60) * 7).month(), \
                 date.addDays((selWeekDate.week - 60) * 7).day());

    for (i = 0; i < 60 + 80; i++) {
        weekDate.year = date.year();
        weekDate.week = date.weekNumber();
        weekDateVariant.setValue(weekDate);

        int yearOfWeek;
        date.weekNumber(&yearOfWeek);
        periodComboBox->addItem(QString::number(date.year()) + ", " + \
                                QString::number(date.weekNumber()) + " : " + \
                                WeekCalc::getWeekRange(date.weekNumber(), date.year(), yearOfWeek), weekDateVariant);

        date.setDate(date.addDays(7).year(), date.addDays(7).month(), date.addDays(7).day());
    }
    // = qvariant_cast<weekDate_s>(weekDateVariant);
    for (i = 0; i < 60 + 80; i++) {
        if ( (periodComboBox->itemData(i).value<weekDate_s>().year == selWeekDate.year) && \
             (periodComboBox->itemData(i).value<weekDate_s>().week == selWeekDate.week) ){
            periodComboBox->setCurrentIndex(i);
        }
    }

    taskLineEdit->setText(mainTableView->model()->index(rowidx, RTO_TASK_NAME).data().toString());

    deviceLineEdit->setText(mainTableView->model()->index(rowidx, RTO_DEV_KKS).data().toString() + " — " + \
                            mainTableView->model()->index(rowidx, RTO_DEV_PART_NUM).data().toString());

    statusComboBox->setCurrentIndex(mainTableView->model()->index(rowidx, RTO_STATUS_VAL).data().toInt());

    commentTextEdit->setText(mainTableView->model()->index(rowidx, RTO_COMMENT).data().toString());

    setWindowTitle(tr("TOiK"));
    result = exec();

    taskComboBox->setVisible(true);
    taskLineEdit->setHidden(true);

    deviceComboBox->setVisible(true);
    deviceLineEdit->setHidden(true);

    curTaskId = taskComboBox->model()->data(taskComboBox->model()->index(taskComboBox->currentIndex(), 0)).toString();
    curDeviceId = deviceComboBox->model()->data(deviceComboBox->model()->index(deviceComboBox->currentIndex(), 0)).toString();

    if ((result == QDialog::Accepted) && isEdited()) {
        sql_query = "UPDATE rto_list SET week = " + QString::number(periodComboBox->currentData().value<weekDate_s>().week) + ", " \
                                        "status = " + statusComboBox->itemData(statusComboBox->currentIndex()).toString() + ", " \
                                        "comment = \"" + commentTextEdit->toPlainText() + "\", " \
                                        "year = " + QString::number(periodComboBox->currentData().value<weekDate_s>().year) + " " \
                    "WHERE id = " + mainModel->index(rowidx, RTO_ID).data().toString();
        //qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            emit dbChanged();
            //mainTableView->selectionModel()->setCurrentIndex(curIdx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
    }
    if (isEdited()) {
        setWindowTitle(tr("TOiK"));
    }

    //mainTableView->setFocus();
}

void RTOEditDialog::delAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    QSqlQuery query;
    QString sql_query;
    QSqlQueryModel *queryModel;

    queryModel = (QSqlQueryModel *) mainTableView->model();

    //if (!checkRelation("SELECT COUNT(id) FROM rto_list WHERE task_id = " + queryModel->index(rowidx, 0).data().toString())) {

    const TrMessageBox::StandardButton ret = TrMessageBox::warning(this, tr("TOiK"),
                               tr("Do you realy want to delete task ") + "\"" + mainTableView->model()->index(rowidx, RTO_TASK_NAME).data().toString() + "\"?",
                               QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);

    if (ret == QMessageBox::Yes) {
        sql_query = "DELETE FROM rto_list WHERE id = " + queryModel->index(rowidx, RTO_ID).data().toString();
        //qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            //queryModel->setQuery(queryModel->query().lastQuery());

            emit dbChanged();
            emit ((RTODialog *) parentWidget)->selectionChanged();
        }
    }
    //} else {
    //    QMessageBox::warning(this, tr("Attention"), tr("Cannot delete this task.\nIt is using in the other tables"));
    //}

    //mainTableView->setFocus();

}

void RTOEditDialog::viewAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;

    periodComboBox->setHidden(true);
    periodLineEdit->setVisible(true);

    taskComboBox->setHidden(true);
    taskLineEdit->setVisible(true);

    deviceComboBox->setHidden(true);
    deviceLineEdit->setVisible(true);

    statusComboBox->setHidden(true);
    statusLabel->setVisible(true);

    commentTextEdit->setReadOnly(true);

    periodLineEdit->setText(mainTableView->model()->index(rowidx, RTO_WEEK_TEXT).data().toString());

    taskLineEdit->setText(mainTableView->model()->index(rowidx, RTO_TASK_NAME).data().toString());

    deviceLineEdit->setText(mainTableView->model()->index(rowidx, RTO_DEV_KKS).data().toString() + " — " + \
                            mainTableView->model()->index(rowidx, RTO_DEV_PART_NUM).data().toString());

    //statusComboBox->setCurrentIndex(mainTableView->model()->index(rowidx, RTO_STATUS_VAL).data().toInt());

    statusLabel->setText(mainTableView->model()->index(rowidx, RTO_STATUS).data().toString());

    commentTextEdit->setText(mainTableView->model()->index(rowidx, RTO_COMMENT).data().toString());

    hideCancelButton();

    setWindowTitle(tr("TOiK"));
    result = exec();

    showCancelButton();

    periodLineEdit->setHidden(true);
    periodComboBox->setVisible(true);

    taskLineEdit->setHidden(true);
    taskComboBox->setVisible(true);

    deviceLineEdit->setHidden(true);
    deviceComboBox->setVisible(true);

    statusLabel->setHidden(true);
    statusComboBox->setVisible(true);

    commentTextEdit->setReadOnly(false);

    mainTableView->setFocus();
}
