#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "referencedialog.h"
#include "taskcodedialog.h"
#include "valdialog.h"
#include "trmessagebox.h"

TaskCodeDialog::TaskCodeDialog(QWidget *parent, QTableView *table): ReferenceDialog(parent, table)
{
    mainTableView = table;
    //mainModel = new QSqlQueryModel;
    mainModel = new RefTableModel(TASK_CODE_COMMENT_COLUMN);
    proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(mainModel);
    proxyModel->setFilterKeyColumn(1);
    proxyModel->setSortLocaleAware(true);
    setFilterModel(proxyModel);

    mainModel->setQuery("SELECT id, name FROM task_name_list");
    //table->setModel(mainModel);
    table->setModel(proxyModel);

    mainModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    mainModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    table->setColumnHidden(0, true);

    table->setSortingEnabled(true);
    table->sortByColumn(1, Qt::AscendingOrder);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(500, 400));
    //setMaximumSize(QSize(300, 400));

    taskCodeRegExp = QRegularExpression("^(?=\\S+$).{1,}$");
    taskCodeValidator = new QRegularExpressionValidator(taskCodeRegExp, this);

    taskCodeLineEdit = new QLineEdit;
    taskCodeLineEdit->setValidator(taskCodeValidator);
    taskCodeLineEdits << taskCodeLineEdit;

    taskCodeFormLayout = new QFormLayout;
    taskCodeFormLayout->addRow(tr("Task code: "), taskCodeLineEdit);
    taskCodeLineEdit->setFixedWidth(100);

    addDialog = new ValDialog;
    addDialog->setMainLayout(taskCodeFormLayout);

    taskCodeLineEdit->setFocus();
    addDialog->setWindowFlags((addDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    addDialog->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    addDialog->setMinimumSize(QSize(300, 200));
    addDialog->setMaximumSize(QSize(300, 200));

    addDialog->setValidatorFields(taskCodeLineEdits);
    setGroupBoxTitle(tr("Task code"));

    connect(taskCodeLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(addDialog);} );

    connect(table->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged()));

    //mainTableView->setFocus();
}

RefTableModel *TaskCodeDialog::getModel()
{
    return mainModel;
}

void TaskCodeDialog::setEdited(ValDialog *dialog)
{
    dialog->setEdited();
    dialog->setWindowTitle(tr("TOiK") + "*");
}

void TaskCodeDialog::addAct()
{
    int result;
    QSqlQuery query;
    QString sql_query;
    //QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();

    taskCodeLineEdit->setText("");

    addDialog->setWindowTitle(tr("TOiK"));
    result = addDialog->exec();

    if (result == QDialog::Accepted && addDialog->isEdited()) {
        sql_query = "SELECT COUNT(id) FROM task_name_list WHERE name LIKE \"" + taskCodeLineEdit->text() + "\"";
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
                sql_query = "INSERT INTO task_name_list (name) VALUES (\"" + \
                    taskCodeLineEdit->text() + "\")";
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
                QMessageBox::warning(this, tr("Attention"), tr("Task code ") + taskCodeLineEdit->text() + tr(" already exist"));
            }
        }
    } // if (result == QDialog::Accepted)

    if (addDialog->isEdited()) {
        addDialog->setWindowTitle(tr("TOiK"));
    }
    mainTableView->setFocus();
}

void TaskCodeDialog::editAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    //QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();
    QSqlQuery query;
    QString sql_query;

    if (!mainTableView->selectionModel()->hasSelection()) {
        return;
    }

    taskCodeLineEdit->setText(proxyModel->index(rowidx, 1).data().toString());

    addDialog->setWindowTitle(tr("TOiK"));
    result = addDialog->exec();

    if ((result == QDialog::Accepted) && addDialog->isEdited()) {
        sql_query = "UPDATE task_name_list SET name = \"" + taskCodeLineEdit->text() + "\" " + \
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
    if (addDialog->isEdited()) {
        addDialog->setWindowTitle(tr("TOiK"));
    }
    mainTableView->setFocus();
}

void TaskCodeDialog::delAct()
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

    if (!checkRelation("SELECT COUNT(id) FROM task_list WHERE task_name_id = " + queryModel->index(rowidx, 0).data().toString())) {

    const TrMessageBox::StandardButton ret = TrMessageBox::warning(this, tr("TOiK"),
                               tr("Do you realy want to delete task code ") + "\"" + mainTableView->model()->index(rowidx, 1).data().toString() + "\"?",
                               QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);

    if (ret == QMessageBox::Yes) {
        sql_query = "DELETE FROM task_name_list WHERE id = " + queryModel->index(rowidx, 0).data().toString();
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
        QMessageBox::warning(this, tr("Attention"), tr("Cannot delete this task code.\nIt is using in the other tables"));
    }

    mainTableView->setFocus();
}
