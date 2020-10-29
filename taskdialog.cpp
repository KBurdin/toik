#include <QHeaderView>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>

#include "referencedialog.h"
#include "taskdialog.h"
#include "valdialog.h"
#include "sqlmodel.h"
#include "trmessagebox.h"

TaskDialog::TaskDialog(QWidget *parent, QTableView *table): ReferenceDialog(parent, table)
{    
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(900, 400));
    //setMaximumSize(QSize(900, 500));

    mainTableView = table;
    //mainModel = new QSqlQueryModel;
    mainModel = new RefTableModel(TASK_COMMENT_COLUMN);
    proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(mainModel);
    proxyModel->setFilterKeyColumn(9);
    proxyModel->setSortLocaleAware(true);
    setFilterModel(proxyModel);

    mainModel->setQuery("SELECT id, task_name_id, task_code, dev_name_id, part_num, name, period, amount, comment, " \
                        "task_code || part_num || name AS find_field FROM task_list_view");
    //table->setModel(mainModel);
    table->setModel(proxyModel);

    mainModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    mainModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Task name id"));
    mainModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Task code", "taskdialog"));
    mainModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Device name id"));
    mainModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Part number"));
    mainModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Task name", "taskdialog"));
    mainModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Period"));
    mainModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Manpower"));
    mainModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Comment"));
    mainModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Find field"));

    table->setColumnHidden(0, true);
    table->setColumnHidden(1, true);
    table->setColumnHidden(3, true);
    table->setColumnHidden(9, true);

    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);

    table->horizontalHeader()->resizeSection(2, 90); // Task code
    table->horizontalHeader()->resizeSection(4, 110); // Part number
    table->horizontalHeader()->resizeSection(5, 180); // Task name
    table->horizontalHeader()->resizeSection(6, 90); // Period
    table->horizontalHeader()->resizeSection(7, 120); // Manpower

    table->setSortingEnabled(true);
    table->sortByColumn(5, Qt::AscendingOrder);

    //--------------------------------------------------------------------------------

    periodRegExp = QRegularExpression("^[0-9]\\d*$");
    periodValidator = new QRegularExpressionValidator(periodRegExp, this);

    amountRegExp = QRegularExpression("\\d+(\\.\\d+)?");
    amountValidator = new QRegularExpressionValidator(amountRegExp, this);

    taskCodeModel = new SqlModel;
    taskCodeModel->setQuery("SELECT id, name FROM task_name_list");
    deviceCodeModel = new SqlModel;
    deviceCodeModel->setQuery("SELECT id, part_num FROM dev_name_list");

    taskCodeHBoxLayout = new QHBoxLayout;
    deviceCodeHBoxLayout = new QHBoxLayout;
    taskCodeLineEdit = new QLineEdit;
    taskCodeLineEdit->setFixedWidth(150);
    taskCodeLineEdit->setHidden(true);
    taskCodeLineEdit->setReadOnly(true);

    deviceCodeLineEdit = new QLineEdit;
    deviceCodeLineEdit->setFixedWidth(150);
    deviceCodeLineEdit->setHidden(true);
    deviceCodeLineEdit->setReadOnly(true);

    taskFormLayout = new QFormLayout;
    taskCodeComboBox = new QComboBox;
    taskCodeComboBox->setFixedWidth(150);
    taskCodeComboBox->setModel(taskCodeModel);
    taskCodeComboBox->setModelColumn(1);
    taskCodeHBoxLayout->addWidget(taskCodeComboBox);
    taskCodeHBoxLayout->addWidget(taskCodeLineEdit);
    deviceCodeComboBox = new QComboBox;
    deviceCodeComboBox->setFixedWidth(150);
    deviceCodeComboBox->setModel(deviceCodeModel);
    deviceCodeComboBox->setModelColumn(1);
    deviceCodeHBoxLayout->addWidget(deviceCodeComboBox);
    deviceCodeHBoxLayout->addWidget(deviceCodeLineEdit);
    taskNameLineEdit = new QLineEdit;
    periodLineEdit = new QLineEdit;
    periodLineEdit->setFixedWidth(100);
    periodLineEdit->setValidator(periodValidator);
    taskLineEdits << periodLineEdit;
    amountLineEdit = new QLineEdit;
    amountLineEdit->setFixedWidth(100);
    amountLineEdit->setValidator(amountValidator);
    taskLineEdits << amountLineEdit;
    commentTextEdit = new QTextEdit;
    //commentTextEdit->setFixedWidth(200);
    commentTextEdit->setFixedHeight(50);

    taskFormLayout->addRow(tr("Task code:"), taskCodeHBoxLayout);
    taskFormLayout->addRow(tr("Device code:"), deviceCodeHBoxLayout);
    taskFormLayout->addRow(tr("Name:"), taskNameLineEdit);
    taskFormLayout->addRow(tr("Period:"), periodLineEdit);
    taskFormLayout->addRow(tr("Manpower:"), amountLineEdit);
    taskFormLayout->addRow(tr("Comment:"), commentTextEdit);

    taskDialog = new ValDialog;
    taskDialog->setMainLayout(taskFormLayout);

    taskDialog->setWindowFlags((taskDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    taskDialog->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    taskDialog->setMinimumSize(QSize(500, 280));
    //taskDialog->setMaximumSize(QSize(500, 400));

    taskDialog->setValidatorFields(taskLineEdits);
    setGroupBoxTitle(tr("Task"));

    connect(taskCodeComboBox, &QComboBox::currentTextChanged, [=](){this->setEdited(taskDialog);} );
    connect(deviceCodeComboBox, &QComboBox::currentTextChanged, [=](){this->setEdited(taskDialog);} );
    connect(taskNameLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(taskDialog);} );
    connect(periodLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(taskDialog);} );
    connect(amountLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(taskDialog);} );
    connect(commentTextEdit, &QTextEdit::textChanged, [=](){this->setEdited(taskDialog);} );

    connect(taskCodeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTaskName()));
    connect(deviceCodeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTaskName()));
    connect(taskNameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(setEditedTaskName()));

    connect(table->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged()));

    //mainTableView->setFocus();
    //--------------------------------------------------------------------------------
}

RefTableModel *TaskDialog::getModel()
{
    return mainModel;
}

SqlModel *TaskDialog::getTaskModel()
{
    return taskCodeModel;
}

SqlModel *TaskDialog::getDeviceModel()
{
    return deviceCodeModel;
}

void TaskDialog::setEdited(ValDialog *dialog)
{
    dialog->setEdited();
    dialog->setWindowTitle(tr("TOiK") + "*");
}

void TaskDialog::setEditedTaskName()
{
    editedTaskName = true;
}

void TaskDialog::updateTaskName()
{
    if (!editedTaskName) {
        taskNameLineEdit->setText(taskCodeComboBox->itemText(taskCodeComboBox->currentIndex()) + tr(" for ", "taskdialog") + \
                              deviceCodeComboBox->itemText(deviceCodeComboBox->currentIndex()));
        taskNameLineEdit->setFocus();
    }

}

void TaskDialog::addAct()
{
    int result;
    QSqlQuery query;
    QString sql_query;
   // QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();
    QString curTaskCodeId, curDeviceCodeId;

    editedTaskName = false;

    taskCodeComboBox->setCurrentIndex(0);
    deviceCodeComboBox->setCurrentIndex(0);

    taskNameLineEdit->setText(taskCodeComboBox->itemText(taskCodeComboBox->currentIndex()) + tr(" for ", "taskdialog") + \
                              deviceCodeComboBox->itemText(deviceCodeComboBox->currentIndex()));
    periodLineEdit->setText("");
    amountLineEdit->setText("");
    commentTextEdit->setText("");

    taskDialog->setWindowTitle(tr("TOiK"));
    taskNameLineEdit->setFocus();
    result = taskDialog->exec();

    curTaskCodeId = taskCodeComboBox->model()->data(taskCodeComboBox->model()->index(taskCodeComboBox->currentIndex(), 0)).toString();
    curDeviceCodeId = deviceCodeComboBox->model()->data(deviceCodeComboBox->model()->index(deviceCodeComboBox->currentIndex(), 0)).toString();

    if (result == QDialog::Accepted && taskDialog->isEdited()) {
        sql_query = "SELECT COUNT(id) FROM task_list WHERE task_name_id = " + curTaskCodeId + \
                                                      " AND dev_name_id = " + curDeviceCodeId;
        //qDebug() << sql_query;
        if (!query.exec(sql_query)) {
            //qDebug() << sql_query;
            qDebug() << query.lastError().text();
        } else {
            while (query.next()) {
                result = query.value(0).toInt();
                //qDebug() << result;
            }
            if (result == 0) {
                sql_query = "INSERT INTO task_list (task_name_id, dev_name_id, name, period, task_amount, comment) VALUES (" + \
                            curTaskCodeId + ", " + \
                            curDeviceCodeId + ", \"" + \
                            taskNameLineEdit->text() + "\", " + \
                            periodLineEdit->text() + ", " + \
                            amountLineEdit->text() + ", \"" + \
                            commentTextEdit->toPlainText() + "\")";
                //qDebug() << sql_query;
                result = query.exec(sql_query);

                if (!result) {
                    qDebug() << query.lastError().text();
                } else {
                    //qDebug() << "restart model";
                    //mainModel->setQuery(mainModel->query().lastQuery());
                    emit dbChanged();
                    //mainTableView->selectionModel()->setCurrentIndex(curIdx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
                }

            } else { // if (result == 0)
                QMessageBox::warning(this, tr("Attention"), tr("Task ") + taskNameLineEdit->text() + tr(" already exist"));
            }
        }
    } // if (result == QDialog::Accepted)

    if (taskDialog->isEdited()) {
        taskDialog->setWindowTitle(tr("TOiK"));
    }

    mainTableView->setFocus();
}

void TaskDialog::editAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    int i, id1, id2;
    //QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();
    QSqlQuery query;
    QString sql_query;
    QString curTaskCodeId, curDeviceCodeId;

    if (!mainTableView->selectionModel()->hasSelection()) {
        return;
    }

    setEditedTaskName();

    i = 0;
    id1 = proxyModel->index(rowidx, 1).data().toInt();
    while ( (i < taskCodeComboBox->model()->rowCount()) && (id2 = taskCodeComboBox->model()->data(taskCodeComboBox->model()->index(i, 0)).toInt() != id1) ) {
        i++;
    }
    taskCodeComboBox->setCurrentIndex(i);

    i = 0;
    id1 = proxyModel->index(rowidx, 3).data().toInt();
    while ( (i < deviceCodeComboBox->model()->rowCount()) && (id2 = deviceCodeComboBox->model()->data(deviceCodeComboBox->model()->index(i, 0)).toInt() != id1) ) {
        i++;
    }
    deviceCodeComboBox->setCurrentIndex(i);

    taskNameLineEdit->setText(proxyModel->index(rowidx, 5).data().toString());
    periodLineEdit->setText(proxyModel->index(rowidx, 6).data().toString());
    amountLineEdit->setText(proxyModel->index(rowidx, 7).data().toString());
    commentTextEdit->setText(proxyModel->index(rowidx, 8).data().toString());

    taskDialog->setWindowTitle(tr("TOiK"));
    result = taskDialog->exec();

    curTaskCodeId = taskCodeComboBox->model()->data(taskCodeComboBox->model()->index(taskCodeComboBox->currentIndex(), 0)).toString();
    curDeviceCodeId = deviceCodeComboBox->model()->data(deviceCodeComboBox->model()->index(deviceCodeComboBox->currentIndex(), 0)).toString();

    if ((result == QDialog::Accepted) && taskDialog->isEdited()) {
        sql_query = "UPDATE task_list SET task_name_id = " + curTaskCodeId + ", " + \
                                         "dev_name_id = " + curDeviceCodeId + ", " + \
                                         "name = \"" + taskNameLineEdit->text() + "\", " + \
                                         "period = " + periodLineEdit->text() + ", " + \
                                         "task_amount = " + amountLineEdit->text() + ", " + \
                                         "comment = \"" + commentTextEdit->toPlainText() + "\" "
                    "WHERE id = " + proxyModel->index(rowidx, 0).data().toString();
        //qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            //mainModel->setQuery(mainModel->query().lastQuery());
            emit dbChanged();
            //mainTableView->selectionModel()->setCurrentIndex(curIdx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
    }
    if (taskDialog->isEdited()) {
        taskDialog->setWindowTitle(tr("TOiK"));
    }

    mainTableView->setFocus();
}

void TaskDialog::delAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    QSqlQuery query;
    QString sql_query;
    QSqlQueryModel *queryModel;

    if (!mainTableView->selectionModel()->hasSelection()) {
        return;
    }

    queryModel = (QSqlQueryModel *) mainTableView->model();

    if (!checkRelation("SELECT COUNT(id) FROM rto_list WHERE task_id = " + queryModel->index(rowidx, 0).data().toString())) {

    const TrMessageBox::StandardButton ret = TrMessageBox::warning(this, tr("TOiK"),
                               tr("Do you realy want to delete task ") + "\"" + mainTableView->model()->index(rowidx, 5).data().toString() + "\"?",
                               QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);

    if (ret == QMessageBox::Yes) {
        sql_query = "DELETE FROM task_list WHERE id = " + queryModel->index(rowidx, 0).data().toString();
        //qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            //queryModel->setQuery(queryModel->query().lastQuery());
            emit dbChanged();
            emit ReferenceDialog::selectionChanged();
        }
    }
    } else {
        QMessageBox::warning(this, tr("Attention"), tr("Cannot delete this task.\nIt is using in the other tables"));
    }

    mainTableView->setFocus();
}

void TaskDialog::viewAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int i, id1, id2;
    QSqlQuery query;
    QString sql_query;
    QString curTaskCodeId, curDeviceCodeId;

    i = 0;
    id1 = proxyModel->index(rowidx, 1).data().toInt();
    while ( (i < taskCodeComboBox->model()->rowCount()) && (id2 = taskCodeComboBox->model()->data(taskCodeComboBox->model()->index(i, 0)).toInt() != id1) ) {
        i++;
    }
    taskCodeComboBox->setCurrentIndex(i);

    i = 0;
    id1 = proxyModel->index(rowidx, 3).data().toInt();
    while ( (i < deviceCodeComboBox->model()->rowCount()) && (id2 = deviceCodeComboBox->model()->data(deviceCodeComboBox->model()->index(i, 0)).toInt() != id1) ) {
        i++;
    }
    deviceCodeComboBox->setCurrentIndex(i);

    taskNameLineEdit->setText(proxyModel->index(rowidx, 5).data().toString());
    periodLineEdit->setText(proxyModel->index(rowidx, 6).data().toString());
    amountLineEdit->setText(proxyModel->index(rowidx, 7).data().toString());
    commentTextEdit->setText(proxyModel->index(rowidx, 8).data().toString());


    taskCodeLineEdit->setText(taskCodeComboBox->currentText());
    taskCodeComboBox->setHidden(true);
    taskCodeLineEdit->setVisible(true);
    deviceCodeLineEdit->setText(deviceCodeComboBox->currentText());
    deviceCodeComboBox->setHidden(true);
    deviceCodeLineEdit->setVisible(true);
    taskNameLineEdit->setReadOnly(true);
    periodLineEdit->setReadOnly(true);
    amountLineEdit->setReadOnly(true);
    commentTextEdit->setReadOnly(true);
    taskDialog->hideCancelButton();
    commentTextEdit->setFocus();

    taskDialog->setWindowTitle(tr("TOiK"));

    taskDialog->exec();

    taskCodeComboBox->setVisible(true);
    taskCodeLineEdit->setHidden(true);
    deviceCodeComboBox->setVisible(true);
    deviceCodeLineEdit->setHidden(true);
    taskNameLineEdit->setReadOnly(false);
    periodLineEdit->setReadOnly(false);
    amountLineEdit->setReadOnly(false);
    commentTextEdit->setReadOnly(false);
    taskDialog->showCancelButton();

    mainTableView->setFocus();
}
