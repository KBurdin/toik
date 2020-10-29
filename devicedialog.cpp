#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

#include "referencedialog.h"
#include "devicedialog.h"
#include "trmessagebox.h"

DeviceDialog::DeviceDialog(QWidget *parent, QTableView *table): ReferenceDialog(parent, table),
    mainTableView(table),
    mainModel(new RefTableModel(DEVICE_COMMENT_COLUMN)),
    proxyModel(new QSortFilterProxyModel),
    deviceDialog(new ValDialog),
    kksValidator(new QRegularExpressionValidator(kksRegExp, this)),
    deviceCodeModel(new SqlModel),
    deviceFormLayout(new QFormLayout),
    kksLineEdit(new QLineEdit),
    deviceNameComboBox(new QComboBox),
    serialLineEdit(new QLineEdit),
    ismsrCheckBox(new QCheckBox(tr("Measuring equipment"))),
    ismsrLabel(new QLabel),
    mntTypeComboBox(new QComboBox),
    commentTextEdit(new QTextEdit),
    ismsrRowNameLabel(new QLabel(tr("Measuring type:"))),
    deviceNameHBoxLayout(new QHBoxLayout),
    ismsrHBoxLayout(new QHBoxLayout),
    mntTypeHBLayout(new QHBoxLayout),
    deviceNameLineEdit(new QLineEdit),
    mntTypeLineEdit(new QLineEdit)


{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(900, 400));

    proxyModel->setSourceModel(mainModel);
    proxyModel->setFilterKeyColumn(8);
    proxyModel->setSortLocaleAware(true);
    setFilterModel(proxyModel);

    mainModel->setQuery("SELECT id, kks, dev_name_id, part_num, serial_num, ismsr, mnt_type_val, comment, " \
                        "kks || part_num || serial_num AS find_field FROM devices_list_view");

    table->setModel(proxyModel);

    mainModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    mainModel->setHeaderData(1, Qt::Horizontal, QObject::tr("KKS"));
    mainModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Device name id"));
    mainModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Part number"));
    mainModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Serial number"));
    mainModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Is measurement"));
    mainModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Maintenance type val"));
    mainModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Comment"));
    mainModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Find field"));

    table->setColumnHidden(0, true);
    table->setColumnHidden(2, true);
    table->setColumnHidden(5, true);
    table->setColumnHidden(6, true);
    table->setColumnHidden(8, true);

    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);

    table->horizontalHeader()->resizeSection(1, 170); // KKS
    table->horizontalHeader()->resizeSection(3, 160); // Part number
    table->horizontalHeader()->resizeSection(4, 160); // Serial number

    table->setSortingEnabled(true);
    table->sortByColumn(1, Qt::AscendingOrder);

    //----------------------------------------------------------------------------------

    kksRegExp = QRegularExpression(".*");
    deviceCodeModel->setQuery("SELECT id, part_num FROM dev_name_list");
    kksLineEdit->setFixedWidth(150);
    kksLineEdit->setValidator(kksValidator);
    deviceLineEdits << kksLineEdit;
    deviceNameComboBox->setFixedWidth(170);
    deviceNameComboBox->setModel(deviceCodeModel);
    deviceNameComboBox->setModelColumn(1);
    deviceNameLineEdit->setFixedWidth(150);
    deviceNameLineEdit->setReadOnly(true);
    deviceNameLineEdit->setHidden(true);    
    mntTypeLineEdit->setFixedWidth(250);
    mntTypeLineEdit->setReadOnly(true);
    mntTypeLineEdit->setHidden(true);    
    ismsrLabel->setHidden(true);
    serialLineEdit->setFixedWidth(150);
    ismsrRowNameLabel->setWordWrap(true);
    ismsrRowNameLabel->setFixedWidth(120);    
    mntTypeComboBox->setFixedWidth(250);
    mntTypeComboBox->addItem(tr("No service"), 0);
    mntTypeComboBox->addItem(tr("Verify"), 1);
    mntTypeComboBox->addItem(tr("Calibrate"), 2);
    mntTypeComboBox->addItem(tr("Verify and calibrate"), 3);
    commentTextEdit->setFixedHeight(50);
    deviceNameHBoxLayout->addWidget(deviceNameComboBox);
    deviceNameHBoxLayout->addWidget(deviceNameLineEdit);    
    ismsrHBoxLayout->addWidget(ismsrCheckBox);
    ismsrHBoxLayout->addWidget(ismsrLabel);    
    mntTypeHBLayout->addWidget(mntTypeComboBox);
    mntTypeHBLayout->addWidget(mntTypeLineEdit);
    deviceFormLayout->addRow(tr("KKS:"), kksLineEdit);
    deviceFormLayout->addRow(tr("Device:"), deviceNameHBoxLayout);
    deviceFormLayout->addRow(tr("Serial number:"), serialLineEdit);
    deviceFormLayout->addRow(ismsrHBoxLayout);

    deviceFormLayout->addRow(ismsrRowNameLabel, mntTypeHBLayout);
    deviceFormLayout->addRow(tr("Comment:"), commentTextEdit);

    deviceDialog->setMainLayout(deviceFormLayout);

    deviceDialog->setWindowFlags((deviceDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    deviceDialog->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    deviceDialog->setMinimumSize(QSize(500, 300));

    setGroupBoxTitle(tr("Device"));

    connect(kksLineEdit, &QLineEdit::textChanged, [this]() {
        this->setEdited(deviceDialog);
    });

    connect(deviceNameComboBox, &QComboBox::currentTextChanged, [this]() {
        this->setEdited(deviceDialog);
    });

    connect(serialLineEdit, &QLineEdit::textChanged, [this]() {
        this->setEdited(deviceDialog);
    });

    connect(ismsrCheckBox, &QCheckBox::stateChanged, [this]() {
        this->setEdited(deviceDialog);
    });

    connect(mntTypeComboBox, &QComboBox::currentTextChanged, [this]() {
        this->setEdited(deviceDialog);
    });

    connect(commentTextEdit, &QTextEdit::textChanged, [this]() {
        this->setEdited(deviceDialog);
    });

    connect(table->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged()));

}

RefTableModel *DeviceDialog::getModel()
{
    return mainModel;
}

SqlModel *DeviceDialog::getDeviceModel()
{
    return deviceCodeModel;
}

void DeviceDialog::setEdited(ValDialog *dialog)
{
    dialog->setEdited();
    dialog->setWindowTitle(tr("TOiK") + "*");
}

void DeviceDialog::addAct()
{
    int result;
    QSqlQuery query;
    QString sql_query;
    //QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();
    QString curDeviceNameId;

    kksLineEdit->setText("");
    deviceNameComboBox->setCurrentIndex(0);
    serialLineEdit->setText("");
    ismsrCheckBox->setChecked(true);
    mntTypeComboBox->setCurrentIndex(0);
    commentTextEdit->setText("");

    deviceDialog->setWindowTitle(tr("TOiK"));
    kksLineEdit->setFocus();
    result = deviceDialog->exec();

    curDeviceNameId = deviceNameComboBox->model()->data(deviceNameComboBox->model()->index(deviceNameComboBox->currentIndex(), 0)).toString();

    if (result == QDialog::Accepted && deviceDialog->isEdited()) {
        sql_query = "SELECT COUNT(id) FROM dev_list WHERE kks LIKE \"" + kksLineEdit->text() + "\"";
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
                sql_query = "INSERT INTO dev_list (kks, dev_name_id, serial_num, comment, mnt_type, ismsr) VALUES (\"" + \
                            kksLineEdit->text() + "\", " + \
                            curDeviceNameId + ", \"" + \
                            serialLineEdit->text() + "\", \"" + \
                            commentTextEdit->toPlainText() + "\", " + \
                            mntTypeComboBox->itemData(mntTypeComboBox->currentIndex()).toString() + ", " + \
                            (ismsrCheckBox->isChecked() ? "1" : "0") + ")";
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
                QMessageBox::warning(this, tr("Attention"), tr("Device ") + kksLineEdit->text() + tr(" already exist"));
            }
        }
    } // if (result == QDialog::Accepted)

    if (deviceDialog->isEdited()) {
        deviceDialog->setWindowTitle(tr("TOiK"));
    }

    mainTableView->setFocus();
}

void DeviceDialog::editAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    int i, id1;
    //QModelIndex curIdx = mainTableView->selectionModel()->currentIndex();
    QSqlQuery query;
    QString sql_query;
    QString curDeviceNameId;

    if (!mainTableView->selectionModel()->hasSelection()) {
        return;
    }

    i = 0;
    id1 = proxyModel->index(rowidx, 2).data().toInt();
    while ( (i < deviceNameComboBox->model()->rowCount()) && (deviceNameComboBox->model()->data(deviceNameComboBox->model()->index(i, 0)).toInt() != id1) ) {
        i++;
    }
    deviceNameComboBox->setCurrentIndex(i);

    i = 0;
    id1 = proxyModel->index(rowidx, 6).data().toInt();
    while ( (i < mntTypeComboBox->model()->rowCount()) && (mntTypeComboBox->model()->data(mntTypeComboBox->model()->index(i, 1)).toInt() != id1) ) {
        i++;
    }
    mntTypeComboBox->setCurrentIndex(proxyModel->index(rowidx, 6).data().toInt());

    kksLineEdit->setText(proxyModel->index(rowidx, 1).data().toString());
    serialLineEdit->setText(proxyModel->index(rowidx, 4).data().toString());
    if (proxyModel->index(rowidx, 5).data().toInt() == 1) {
        ismsrCheckBox->setChecked(true);
    } else {
        ismsrCheckBox->setChecked(false);
    }
    commentTextEdit->setText(proxyModel->index(rowidx, 7).data().toString());

    deviceDialog->setWindowTitle(tr("TOiK"));
    result = deviceDialog->exec();

    curDeviceNameId = deviceNameComboBox->model()->data(deviceNameComboBox->model()->index(deviceNameComboBox->currentIndex(), 0)).toString();

    if ((result == QDialog::Accepted) && deviceDialog->isEdited()) {

        sql_query = "UPDATE dev_list SET kks = \"" + kksLineEdit->text() + "\", " + \
                                        "dev_name_id = " + curDeviceNameId + ", " + \
                                        "serial_num = \"" + serialLineEdit->text() + "\", " + \
                                        "comment = \"" + commentTextEdit->toPlainText() + "\", " + \
                                        "mnt_type = " + mntTypeComboBox->itemData(mntTypeComboBox->currentIndex()).toString() + ", " + \
                                        "ismsr = " + (ismsrCheckBox->isChecked() ? "1" : "0") + " " \
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
    if (deviceDialog->isEdited()) {
        deviceDialog->setWindowTitle(tr("TOiK"));
    }

    mainTableView->setFocus();
}

void DeviceDialog::delAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int result;
    QSqlQuery query;
    QString sql_query;
    QSqlQueryModel *queryModel;
    bool cond1, cond2, cond3;

    if (!mainTableView->selectionModel()->hasSelection()) {
        return;
    }

    queryModel = (QSqlQueryModel *) mainTableView->model();

    cond1 = checkRelation("SELECT COUNT(id) FROM rto_list WHERE dev_id = " + queryModel->index(rowidx, 0).data().toString());
    cond2 = checkRelation("SELECT COUNT(id) FROM rp_list WHERE dev_id = " + queryModel->index(rowidx, 0).data().toString());
    cond3 = checkRelation("SELECT COUNT(id) FROM rk_list WHERE dev_id = " + queryModel->index(rowidx, 0).data().toString());

    if (!cond1 && !cond2 && !cond3) {

        const TrMessageBox::StandardButton ret = QMessageBox::warning(this, tr("TOiK"),
                               tr("Do you realy want to delete device ") + "\"" + mainTableView->model()->index(rowidx, 1).data().toString() + "\"?",
                               QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);

        if (ret == QMessageBox::Yes) {
            sql_query = "DELETE FROM dev_list WHERE id = " + queryModel->index(rowidx, 0).data().toString();
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

void DeviceDialog::viewAct()
{
    int rowidx = mainTableView->selectionModel()->currentIndex().row();
    int i, id1;
    QSqlQuery query;
    QString sql_query;
    QString curDeviceNameId;

    i = 0;
    id1 = proxyModel->index(rowidx, 2).data().toInt();
    while ( (i < deviceNameComboBox->model()->rowCount()) && (deviceNameComboBox->model()->data(deviceNameComboBox->model()->index(i, 0)).toInt() != id1) ) {
        i++;
    }
    deviceNameComboBox->setCurrentIndex(i);

    i = 0;
    id1 = proxyModel->index(rowidx, 6).data().toInt();
    while ( (i < mntTypeComboBox->model()->rowCount()) && (mntTypeComboBox->model()->data(mntTypeComboBox->model()->index(i, 1)).toInt() != id1) ) {
        i++;
    }
    mntTypeComboBox->setCurrentIndex(proxyModel->index(rowidx, 6).data().toInt());

    kksLineEdit->setText(proxyModel->index(rowidx, 1).data().toString());
    serialLineEdit->setText(proxyModel->index(rowidx, 4).data().toString());
    if (proxyModel->index(rowidx, 5).data().toInt() == 1) {
        ismsrCheckBox->setChecked(true);
    } else {
        ismsrCheckBox->setChecked(false);
    }
    commentTextEdit->setText(proxyModel->index(rowidx, 7).data().toString());

    kksLineEdit->setReadOnly(true);
    deviceNameComboBox->setHidden(true);
    deviceNameLineEdit->setText(deviceNameComboBox->currentText());
    deviceNameLineEdit->setVisible(true);
    serialLineEdit->setReadOnly(true);
    ismsrCheckBox->setHidden(true);
    ismsrLabel->setText((ismsrCheckBox->isChecked() ? tr("Measuring equipment — YES") : tr("Measuring equipment — NO")));
    ismsrLabel->setVisible(true);
    mntTypeComboBox->setHidden(true);
    mntTypeLineEdit->setText(mntTypeComboBox->currentText());
    mntTypeLineEdit->setVisible(true);
    commentTextEdit->setReadOnly(true);
    deviceDialog->hideCancelButton();

    deviceDialog->setWindowTitle(tr("TOiK"));
    deviceDialog->exec();

    kksLineEdit->setReadOnly(false);
    deviceNameLineEdit->setHidden(true);
    deviceNameComboBox->setVisible(true);
    serialLineEdit->setReadOnly(false);
    ismsrLabel->setHidden(true);
    ismsrCheckBox->setVisible(true);
    mntTypeLineEdit->setHidden(true);
    mntTypeComboBox->setVisible(true);
    commentTextEdit->setReadOnly(false);
    deviceDialog->showCancelButton();

    mainTableView->setFocus();
}
