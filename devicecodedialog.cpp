#include <QMessageBox>
#include <QDebug>

#include "referencedialog.h"
#include "devicecodedialog.h"
#include "trmessagebox.h"

DeviceCodeDialog::DeviceCodeDialog(QWidget *parent, QTableView *table): ReferenceDialog(parent, table),
    mainTableView(table),
    mainModel(new RefTableModel(DEVICE_CODE_COMMENT_COLUMN)),
    proxyModel(new QSortFilterProxyModel),
    deviceCodeDialog(new ValDialog),
    periodValidator(new QRegularExpressionValidator(periodRegExp, this)),
    amountValidator(new QRegularExpressionValidator(amountRegExp, this)),
    deviceCodeFormLayout(new QFormLayout),
    partNumLineEdit(new QLineEdit),
    nameLineEdit(new QLineEdit),
    mnfLineEdit(new QLineEdit),
    povPeriodLineEdit(new QLineEdit),
    calPeriodLineEdit(new QLineEdit),
    povAmountLineEdit(new QLineEdit),
    calAmountLineEdit(new QLineEdit),
    commentTextEdit(new QTextEdit)

{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(800, 400));    

    proxyModel->setSourceModel(mainModel);
    proxyModel->setFilterKeyColumn(9);
    proxyModel->setSortLocaleAware(true);
    setFilterModel(proxyModel);

    mainModel->setQuery("SELECT id, part_num, name, mnf, pov_period, cal_period, pov_amount, cal_amount, comment, " \
                        "part_num || name || mnf AS find_field FROM dev_name_list");
    //table->setModel(mainModel);
    table->setModel(proxyModel);

    mainModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    mainModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Part number"));
    mainModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Name"));
    mainModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Manufacturer"));
    mainModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Verify period"));
    mainModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Calibrate period"));
    mainModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Verify amount"));
    mainModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Calibrate amount"));
    mainModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Comment"));
    mainModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Find field"));

    table->setColumnHidden(0, true);
    table->setColumnHidden(4, true);
    table->setColumnHidden(5, true);
    table->setColumnHidden(6, true);
    table->setColumnHidden(7, true);
    table->setColumnHidden(9, true);

    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);

    table->horizontalHeader()->resizeSection(1, 140); // Part number
    table->horizontalHeader()->resizeSection(2, 170); // Name
    table->horizontalHeader()->resizeSection(3, 140); // Manufacturer
    table->horizontalHeader()->resizeSection(4, 100); // Verify period
    table->horizontalHeader()->resizeSection(5, 100); // Calibrate period
    table->horizontalHeader()->resizeSection(6, 100); // Verify amount
    table->horizontalHeader()->resizeSection(7, 100); // Calibrate amount

    table->setSortingEnabled(true);
    table->sortByColumn(1, Qt::AscendingOrder);

    //---------------------------------------------------------------------------------------
    periodRegExp = QRegularExpression("^[0-9]\\d*$");
    amountRegExp = QRegularExpression("\\d+(\\.\\d+)?");


    partNumLineEdit->setFixedWidth(150);

    nameLineEdit->setFixedWidth(150);

    mnfLineEdit->setFixedWidth(150);

    povPeriodLineEdit->setFixedWidth(100);
    povPeriodLineEdit->setValidator(periodValidator);
    deviceCodeLineEdits << povPeriodLineEdit;

    calPeriodLineEdit->setFixedWidth(100);
    calPeriodLineEdit->setValidator(periodValidator);
    deviceCodeLineEdits << calPeriodLineEdit;

    povAmountLineEdit->setFixedWidth(100);
    povAmountLineEdit->setValidator(amountValidator);
    deviceCodeLineEdits << povAmountLineEdit;

    calAmountLineEdit->setFixedWidth(100);
    calAmountLineEdit->setValidator(amountValidator);
    deviceCodeLineEdits << calAmountLineEdit;

    commentTextEdit->setFixedHeight(50);

    deviceCodeFormLayout->addRow(tr("Part number:"), partNumLineEdit);
    deviceCodeFormLayout->addRow(tr("Name:"), nameLineEdit);
    deviceCodeFormLayout->addRow(tr("Manufacturer:"), mnfLineEdit);
    deviceCodeFormLayout->addRow(tr("Verify period:"), povPeriodLineEdit);
    deviceCodeFormLayout->addRow(tr("Calibrate period:"), calPeriodLineEdit);
    deviceCodeFormLayout->addRow(tr("Verify manpower:"), povAmountLineEdit);
    deviceCodeFormLayout->addRow(tr("Calibrate manpower:"), calAmountLineEdit);
    deviceCodeFormLayout->addRow(tr("Comment:"), commentTextEdit);

    deviceCodeDialog->setMainLayout(deviceCodeFormLayout);

    deviceCodeDialog->setWindowFlags((deviceCodeDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    deviceCodeDialog->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    deviceCodeDialog->setMinimumSize(QSize(800, 300));

    deviceCodeDialog->setValidatorFields(deviceCodeLineEdits);
    setGroupBoxTitle(tr("Device code"));

    connect(partNumLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );
    connect(nameLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );
    connect(mnfLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );
    connect(povPeriodLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );
    connect(calPeriodLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );
    connect(povAmountLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );
    connect(calAmountLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );
    connect(commentTextEdit, &QTextEdit::textChanged, [=](){this->setEdited(deviceCodeDialog);} );

    connect(table->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged()));

    //mainTableView->setFocus();
}

RefTableModel *DeviceCodeDialog::getModel()
{
    return mainModel;
}

void DeviceCodeDialog::setEdited(ValDialog *dialog)
{
    dialog->setEdited();
    dialog->setWindowTitle(tr("TOiK") + "*");
}

void DeviceCodeDialog::addAct()
{
    int result;
    QSqlQuery query;
    QString sql_query;
    //QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();
    QString curTaskCodeId, curDeviceCodeId;

    partNumLineEdit->setText("");
    nameLineEdit->setText("");
    mnfLineEdit->setText("");
    povPeriodLineEdit->setText("0");
    calPeriodLineEdit->setText("0");
    povAmountLineEdit->setText("0");
    calAmountLineEdit->setText("0");
    commentTextEdit->setText("");

    deviceCodeDialog->setWindowTitle(tr("TOiK"));
    partNumLineEdit->setFocus();
    result = deviceCodeDialog->exec();

    if (result == QDialog::Accepted && deviceCodeDialog->isEdited()) {
        sql_query = "SELECT COUNT(id) FROM dev_name_list WHERE part_num LIKE \"" + partNumLineEdit->text() + "\"";
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
                sql_query = "INSERT INTO dev_name_list (part_num, name, mnf, comment, pov_period, cal_period, pov_amount, cal_amount) VALUES (\"" + \
                            partNumLineEdit->text() + "\", \"" + \
                            nameLineEdit->text() + "\", \"" + \
                            mnfLineEdit->text() + "\", \"" + \
                            commentTextEdit->toPlainText() + "\", " + \
                            povPeriodLineEdit->text() + ", " + \
                            calPeriodLineEdit->text() + ", " + \
                            povAmountLineEdit->text() + ", " + \
                            calAmountLineEdit->text() + ")";

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
                QMessageBox::warning(this, tr("Attention"), tr("Device ") + partNumLineEdit->text() + tr(" already exist"));
            }
        }
    } // if (result == QDialog::Accepted)

    if (deviceCodeDialog->isEdited()) {
        deviceCodeDialog->setWindowTitle(tr("TOiK"));
    }

    mainTableView->setFocus();
}

void DeviceCodeDialog::editAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    //QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();
    QSqlQuery query;
    QString sql_query;

    if (!mainTableView->selectionModel()->hasSelection()) {
        return;
    }

    partNumLineEdit->setText(proxyModel->index(rowidx, 1).data().toString());
    nameLineEdit->setText(proxyModel->index(rowidx, 2).data().toString());
    mnfLineEdit->setText(proxyModel->index(rowidx, 3).data().toString());
    povPeriodLineEdit->setText(proxyModel->index(rowidx, 4).data().toString());
    calPeriodLineEdit->setText(proxyModel->index(rowidx, 5).data().toString());
    povAmountLineEdit->setText(proxyModel->index(rowidx, 6).data().toString());
    calAmountLineEdit->setText(proxyModel->index(rowidx, 7).data().toString());
    commentTextEdit->setText(proxyModel->index(rowidx, 8).data().toString());
    partNumLineEdit->setFocus();

    deviceCodeDialog->setWindowTitle(tr("TOiK"));
    result = deviceCodeDialog->exec();

    if ((result == QDialog::Accepted) && deviceCodeDialog->isEdited()) {
        sql_query = "UPDATE dev_name_list SET part_num = \"" + partNumLineEdit->text() + "\", " + \
                                             "name = \"" + nameLineEdit->text() + "\", " + \
                                             "mnf = \"" + mnfLineEdit->text() + "\", " + \
                                             "pov_period = " + povPeriodLineEdit->text() + ", " + \
                                             "cal_period = " + calPeriodLineEdit->text() + ", " + \
                                             "pov_amount = " + povAmountLineEdit->text() + ", " + \
                                             "cal_amount = " + calAmountLineEdit->text() + ", " + \
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
    if (deviceCodeDialog->isEdited()) {
        deviceCodeDialog->setWindowTitle(tr("TOiK"));
    }

    mainTableView->setFocus();
}

void DeviceCodeDialog::delAct()
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

    if (!checkRelation("SELECT COUNT(id) FROM dev_list WHERE dev_name_id = " + queryModel->index(rowidx, 0).data().toString())) {

    const TrMessageBox::StandardButton ret = QMessageBox::warning(this, tr("TOiK"),
                               tr("Do you realy want to delete device ") + "\"" + mainTableView->model()->index(rowidx, 1).data().toString() + "\"?",
                               QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);

    if (ret == QMessageBox::Yes) {
        sql_query = "DELETE FROM dev_name_list WHERE id = " + queryModel->index(rowidx, 0).data().toString();
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
        TrMessageBox::warning(this, tr("Attention"), tr("Cannot delete this device.\nIt is using in the other tables"));
    }

    mainTableView->setFocus();
}

void DeviceCodeDialog::viewAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    QSqlQuery query;
    QString sql_query;

    partNumLineEdit->setText(proxyModel->index(rowidx, 1).data().toString());
    nameLineEdit->setText(proxyModel->index(rowidx, 2).data().toString());
    mnfLineEdit->setText(proxyModel->index(rowidx, 3).data().toString());
    povPeriodLineEdit->setText(proxyModel->index(rowidx, 4).data().toString());
    calPeriodLineEdit->setText(proxyModel->index(rowidx, 5).data().toString());
    povAmountLineEdit->setText(proxyModel->index(rowidx, 6).data().toString());
    calAmountLineEdit->setText(proxyModel->index(rowidx, 7).data().toString());
    commentTextEdit->setText(proxyModel->index(rowidx, 8).data().toString());

    partNumLineEdit->setReadOnly(true);
    nameLineEdit->setReadOnly(true);
    mnfLineEdit->setReadOnly(true);
    povPeriodLineEdit->setReadOnly(true);
    calPeriodLineEdit->setReadOnly(true);
    povAmountLineEdit->setReadOnly(true);
    calAmountLineEdit->setReadOnly(true);
    commentTextEdit->setReadOnly(true);
    deviceCodeDialog->hideCancelButton();

    deviceCodeDialog->setWindowTitle(tr("TOiK"));
    deviceCodeDialog->exec();

    partNumLineEdit->setReadOnly(false);
    nameLineEdit->setReadOnly(false);
    mnfLineEdit->setReadOnly(false);
    povPeriodLineEdit->setReadOnly(false);
    calPeriodLineEdit->setReadOnly(false);
    povAmountLineEdit->setReadOnly(false);
    calAmountLineEdit->setReadOnly(false);
    commentTextEdit->setReadOnly(false);
    deviceCodeDialog->showCancelButton();

    mainTableView->setFocus();
}
