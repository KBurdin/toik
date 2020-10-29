#include <QDebug>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include "rpeditdialog.h"
#include "referencedialog.h"
#include "rtoeditdialog.h"
#include "rptablemodel.h"
#include "weekcalc.h"
#include "rpdialog.h"
#include "trmessagebox.h"

RPEditDialog::RPEditDialog(QTableView *table, QAbstractItemModel *model, QWidget *parent)
{
    parentWidget = parent;
    mainTableView = table;
    //mainModel = table->model();
    mainModel = model;

    periodModel = new SqlModel;
    //periodModel->setQuery("SELECT id, year, week, name");

    deviceModel = new SqlModel;

    taskEditLineEdits << Q_NULLPTR;

    periodHBoxLayout = new QHBoxLayout;
    deviceHBoxLayout = new QHBoxLayout;
    statusHBoxLayout = new QHBoxLayout;

    taskEditFormLayout = new QFormLayout;

    periodComboBox = new QComboBox;
    periodComboBox->setFixedWidth(180);

    periodLineEdit = new QLineEdit;
    periodLineEdit->setFixedWidth(180);
    periodLineEdit->setHidden(true);
    periodLineEdit->setReadOnly(true);

    periodHBoxLayout->addWidget(periodComboBox);
    periodHBoxLayout->addWidget(periodLineEdit);

    deviceComboBox = new QComboBox;
    deviceComboBox->setFixedWidth(350);
    deviceModel->setQuery("SELECT id, kks || ' — ' || part_num AS device, part_num, pov_period, " \
                          "pov_amount, serial_num FROM devices_list_view WHERE ismsr = 1 AND (mnt_type_val = 1 OR mnt_type_val = 3)");
    deviceComboBox->setModel(deviceModel);
    deviceComboBox->setModelColumn(1);

    deviceLineEdit = new QLineEdit;
    deviceLineEdit->setFixedWidth(350);
    deviceLineEdit->setHidden(true);
    deviceLineEdit->setReadOnly(true);

    deviceHBoxLayout->addWidget(deviceComboBox);
    deviceHBoxLayout->addWidget(deviceLineEdit);

    partNumberLineEdit = new QLineEdit;
    partNumberLineEdit->setFixedWidth(220);
    partNumberLineEdit->setReadOnly(true);

    serialLineEdit = new QLineEdit;
    serialLineEdit->setFixedWidth(220);
    serialLineEdit->setReadOnly(true);

    manHourLineEdit = new QLineEdit;
    manHourLineEdit->setFixedWidth(60);
    manHourLineEdit->setReadOnly(true);

    planPeriodLineEdit = new QLineEdit;
    planPeriodLineEdit->setFixedWidth(60);
    planPeriodLineEdit->setReadOnly(true);

    statusComboBox = new QComboBox;
    statusComboBox->setFixedWidth(150);
    statusComboBox->addItem(tr("Not completed"), 0);
    statusComboBox->addItem(tr("Completed"), 1);
    statusComboBox->addItem(tr("In process"), 2);

    statusLabel = new QLabel;
    statusLabel->setHidden(true);

    statusHBoxLayout->addWidget(statusComboBox);
    statusHBoxLayout->addWidget(statusLabel);

    reportLineEdit = new QLineEdit;
    reportLineEdit->setFixedWidth(350);

    commentTextEdit = new QTextEdit;
    commentTextEdit->setFixedHeight(50);

    taskEditFormLayout->addRow(tr("Period:"), periodHBoxLayout);
    taskEditFormLayout->addRow(tr("Device:"), deviceHBoxLayout);
    taskEditFormLayout->addRow(tr("Part number:"), partNumberLineEdit);
    taskEditFormLayout->addRow(tr("Serial number:"), serialLineEdit);
    taskEditFormLayout->addRow(tr("Manpower:"), manHourLineEdit);
    taskEditFormLayout->addRow(tr("Period:"), planPeriodLineEdit);
    taskEditFormLayout->addRow(tr("Status:"), statusHBoxLayout);
    taskEditFormLayout->addRow(tr("Final report:"), reportLineEdit);
    taskEditFormLayout->addRow(tr("Comment:"), commentTextEdit);

    setMainLayout(taskEditFormLayout);

    setWindowFlags((this->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(500, 350));

    setValidatorFields(taskEditLineEdits);
    setGroupBoxTitle("Task");

    connect(periodLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(this);} );
    connect(deviceComboBox, &QComboBox::currentTextChanged, [=](){this->setEdited(this);} );
    connect(statusComboBox, &QComboBox::currentTextChanged, [=](){this->setEdited(this);} );
    connect(reportLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(this);} );
    connect(commentTextEdit, &QTextEdit::textChanged, [=](){this->setEdited(this);} );

    connect(deviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(deviceUpdated()));

}

QAbstractItemModel *RPEditDialog::getModel()
{
    return mainModel;
}

SqlModel *RPEditDialog::getDeviceModel()
{
    return deviceModel;
}

void RPEditDialog::setEdited(ValDialog *dialog)
{
    dialog->setEdited();
    dialog->setWindowTitle(tr("TOiK") + "*");
}

void RPEditDialog::deviceUpdated()
{
    partNumberLineEdit->setText(getDeviceModelVal(2));
    serialLineEdit->setText(getDeviceModelVal(5));
    manHourLineEdit->setText(getDeviceModelVal(4));
    planPeriodLineEdit->setText(getDeviceModelVal(3));
    deviceComboBox->setFocus();
}

QString RPEditDialog::getDeviceModelVal(int rowNum) const
{
    return deviceComboBox->model()->data(deviceComboBox->model()->index(deviceComboBox->currentIndex(), rowNum)).toString();
}

void RPEditDialog::addAct()
{
    QModelIndex curDevIdx;
    int i, result;
    QDate date;
    weekDate_s weekDate, selWeekDate;;
    QVariant weekDateVariant = QVariant::fromValue(weekDate);
    QString sql_query;
    QString curDeviceId;
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
    deviceComboBox->setCurrentIndex(1);
    deviceComboBox->setCurrentIndex(0);
    statusComboBox->setCurrentIndex(0);
    reportLineEdit->setText("");
    commentTextEdit->setText("");

    setWindowTitle(tr("TOiK"));
    result = exec();

    curDeviceId = deviceComboBox->model()->data(deviceComboBox->model()->index(deviceComboBox->currentIndex(), RP_ID)).toString();

    if ((result == QDialog::Accepted) && isEdited()) {
        sql_query = "INSERT INTO rp_list (week, status, comment, dev_id, report, year) VALUES (" + \
                                          QString::number(periodComboBox->currentData().value<weekDate_s>().week) + ", " + \
                                          statusComboBox->currentData().toString() + ", \"" + \
                                          commentTextEdit->toPlainText() + "\", " + \
                                          curDeviceId + ", \"" + \
                                          reportLineEdit->text() + "\", " +\
                                          QString::number(periodComboBox->currentData().value<weekDate_s>().year) + ")";
        qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            emit dbChanged();
            emit ((RPDialog *) parentWidget)->selectionChanged();
            //mainTableView->selectionModel()->setCurrentIndex(curIdx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
    }
    if (isEdited()) {
        setWindowTitle(tr("TOiK"));
    }

    //mainTableView->setFocus();
}

void RPEditDialog::editAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int i, result;
    QDate date;
    weekDate_s weekDate, selWeekDate;;
    QVariant weekDateVariant = QVariant::fromValue(weekDate);
    QString sql_query;
    QString curTaskId, curDeviceId;
    QSqlQuery query;

    selWeekDate.year = mainModel->index(rowidx, RP_YEAR).data().toInt();
    selWeekDate.week = mainModel->index(rowidx, RP_WEEK).data().toInt();

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

    deviceLineEdit->setText(mainTableView->model()->index(rowidx, RP_KKS).data().toString() + " — " + \
                            mainTableView->model()->index(rowidx, RP_PART_NUM).data().toString());
    partNumberLineEdit->setText(mainTableView->model()->index(rowidx, RP_PART_NUM).data().toString());
    serialLineEdit->setText(mainTableView->model()->index(rowidx, RP_SERIAL_NUM).data().toString());
    manHourLineEdit->setText(mainTableView->model()->index(rowidx, RP_AMOUNT).data().toString());
    planPeriodLineEdit->setText(mainTableView->model()->index(rowidx, RP_PERIOD).data().toString());

    statusComboBox->setCurrentIndex(mainTableView->model()->index(rowidx, RP_STATUS_VAL).data().toInt());
    reportLineEdit->setText(mainTableView->model()->index(rowidx, RP_REPORT).data().toString());

    commentTextEdit->setText(mainTableView->model()->index(rowidx, RP_COMMENT).data().toString());

    setWindowTitle(tr("TOiK"));
    result = exec();

    deviceComboBox->setVisible(true);
    deviceLineEdit->setHidden(true);

    curDeviceId = deviceComboBox->model()->data(deviceComboBox->model()->index(deviceComboBox->currentIndex(), 0)).toString();

    if ((result == QDialog::Accepted) && isEdited()) {
        sql_query = "UPDATE rp_list SET week = " + QString::number(periodComboBox->currentData().value<weekDate_s>().week) + ", " \
                                        "status = " + statusComboBox->currentData().toString() + ", " \
                                        "comment = \"" + commentTextEdit->toPlainText() + "\", " \
                                        "report = \"" + reportLineEdit->text() + "\", " \
                                        "year = " + QString::number(periodComboBox->currentData().value<weekDate_s>().year) + " " \
                    "WHERE id = " + mainModel->index(rowidx, RP_ID).data().toString();
        qDebug() << sql_query;
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

void RPEditDialog::delAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    QSqlQuery query;
    QString sql_query;
    QSqlQueryModel *queryModel;

    queryModel = (QSqlQueryModel *) mainTableView->model();

    const TrMessageBox::StandardButton ret = TrMessageBox::warning(this, tr("TOiK"),
                               tr("Do you realy want to delete task ") + "\"" + mainTableView->model()->index(rowidx, RP_KKS).data().toString() + "\"?", \
                               QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);

    if (ret == QMessageBox::Yes) {
        sql_query = "DELETE FROM rp_list WHERE id = " + queryModel->index(rowidx, RP_ID).data().toString();
        //qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            //queryModel->setQuery(queryModel->query().lastQuery());
            emit dbChanged();
            emit ((RPDialog *) parentWidget)->selectionChanged();
        }
    }

    //mainTableView->setFocus();
}

void RPEditDialog::viewAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;

    periodComboBox->setHidden(true);
    periodLineEdit->setVisible(true);

    deviceComboBox->setHidden(true);
    deviceLineEdit->setVisible(true);

    statusComboBox->setHidden(true);
    statusLabel->setVisible(true);

    reportLineEdit->setReadOnly(true);
    commentTextEdit->setReadOnly(true);

    periodLineEdit->setText(mainTableView->model()->index(rowidx, RP_WEEK_TEXT).data().toString());

    deviceLineEdit->setText(mainTableView->model()->index(rowidx, RP_KKS).data().toString() + " — " + \
                            mainTableView->model()->index(rowidx, RP_PART_NUM).data().toString());
    partNumberLineEdit->setText(mainTableView->model()->index(rowidx, RP_PART_NUM).data().toString());
    serialLineEdit->setText(mainTableView->model()->index(rowidx, RP_SERIAL_NUM).data().toString());
    manHourLineEdit->setText(mainTableView->model()->index(rowidx, RP_AMOUNT).data().toString());
    planPeriodLineEdit->setText(mainTableView->model()->index(rowidx, RP_PERIOD).data().toString());

    statusLabel->setText(mainTableView->model()->index(rowidx, RP_STATUS).data().toString());

    reportLineEdit->setText(mainTableView->model()->index(rowidx, RP_REPORT).data().toString());

    commentTextEdit->setText(mainTableView->model()->index(rowidx, RP_COMMENT).data().toString());

    hideCancelButton();

    setWindowTitle(tr("TOiK"));
    result = exec();

    showCancelButton();

    periodLineEdit->setHidden(true);
    periodComboBox->setVisible(true);

    deviceLineEdit->setHidden(true);
    deviceComboBox->setVisible(true);

    statusLabel->setHidden(true);
    statusComboBox->setVisible(true);

    reportLineEdit->setReadOnly(false);
    commentTextEdit->setReadOnly(false);

    mainTableView->setFocus();
}
