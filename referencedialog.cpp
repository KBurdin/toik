#include <QDebug>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>

#include "referencedialog.h"

ReferenceDialog::ReferenceDialog(QWidget *parent, QTableView *table) : QWidget(parent)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    mainTableView = table;
    filterModel = Q_NULLPTR;
    filterFlag = false;

    mainVBoxLayout = new QVBoxLayout;
    mainHBoxLayout = new QHBoxLayout;
    buttonVBoxLayout = new QVBoxLayout;
    closeHBoxLayout = new QHBoxLayout;

    findHBoxLayout = new QHBoxLayout;
    findLabel = new QLabel(tr("Find:"));
    findLineEdit = new QLineEdit;
    findLineEdit->setFixedWidth(180);
    findPushButton = new QPushButton(tr("Find"));
    findPushButton->setHidden(true);
    clearPushButton = new QPushButton(tr("Clear"));
    //clearPushButton->setDisabled(true);
    findHBoxLayout->addWidget(findLabel);
    findHBoxLayout->addWidget(findLineEdit);
    findHBoxLayout->addWidget(findPushButton);
    findHBoxLayout->addWidget(clearPushButton);
    findHBoxLayout->addStretch();

    //mainTableView = new QTableView;
    addPushButton = new QPushButton(tr("Add"));
    editPushButton = new QPushButton(tr("Edit"));
    editPushButton->setDisabled(true);
    delPushButton = new QPushButton(tr("Delete"));
    delPushButton->setDisabled(true);
    closePushButton = new QPushButton(tr("Close"));
    viewPushButton = new QPushButton(tr("View"));
    viewPushButton->setDisabled(true);
    mainGroupBox = new QGroupBox;

    buttonVBoxLayout->addWidget(addPushButton);
    buttonVBoxLayout->addWidget(editPushButton);
    buttonVBoxLayout->addWidget(delPushButton);
    buttonVBoxLayout->addSpacing(20);
    buttonVBoxLayout->addWidget(viewPushButton);
    buttonVBoxLayout->addStretch();

    mainHBoxLayout->addWidget(table);
    mainHBoxLayout->addLayout(buttonVBoxLayout);

    groupBoxVBoxLayout = new QVBoxLayout;
    groupBoxVBoxLayout->addLayout(findHBoxLayout);
    groupBoxVBoxLayout->addLayout(mainHBoxLayout);

    mainGroupBox->setLayout(groupBoxVBoxLayout);

    mainVBoxLayout->addWidget(mainGroupBox);

    closeHBoxLayout->addStretch();
    closeHBoxLayout->addWidget(closePushButton);
    closeHBoxLayout->addStretch();

    mainVBoxLayout->addLayout(closeHBoxLayout);
    setLayout(mainVBoxLayout);

    table->setSelectionMode(QAbstractItemView::SingleSelection);

    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);

    table->sortByColumn(0, Qt::AscendingOrder); // id

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->verticalHeader()->setDefaultSectionSize(22);

    deleteShortcut = new QShortcut(QKeySequence(QKeySequence::Delete), table);
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(delAct()));

    connect(closePushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(addPushButton, SIGNAL(clicked(bool)), this, SLOT(addAct()));
    connect(editPushButton, SIGNAL(clicked(bool)), this, SLOT(editAct()));
    connect(delPushButton, SIGNAL(clicked(bool)), this, SLOT(delAct()));
    connect(viewPushButton, SIGNAL(clicked(bool)), this, SLOT(viewAct()));
    connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(viewAct()));
    connect(findLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setFilter()));
    connect(findPushButton, SIGNAL(clicked(bool)), this, SLOT(setFilter()));
    connect(clearPushButton, SIGNAL(clicked(bool)), this, SLOT(clearFilter()));
}

void ReferenceDialog::setGroupBoxTitle(const QString &title)
{
    mainGroupBox->setTitle(title);
}

bool ReferenceDialog::checkRelation(const QString &query_str)
{
    QSqlQuery query;
    if (!query.exec(query_str)) {
        qDebug() << query.lastError().text();
    } else {
        while (query.next()) {
            return query.value(0).toInt();
        }
    }
    return true;
}

void ReferenceDialog::hideViewButton()
{
    viewPushButton->setHidden(true);
}

void ReferenceDialog::showViewButton()
{
    viewPushButton->setVisible(true);
}

void ReferenceDialog::setFilterModel(QSortFilterProxyModel *model)
{
    filterModel = model;
}

void ReferenceDialog::setFilter()
{
    QString pattern = findLineEdit->displayText();
    QRegExp regExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp);

    //qDebug() << pattern;

    if (pattern != "") {
        findLabel->setStyleSheet("QLabel {color: red;}");
        filterFlag = true;
        //clearPushButton->setEnabled(true);
    } else {
        findLabel->setStyleSheet("QLabel {color: black;}");
        filterFlag = false;
        //clearPushButton->setDisabled(true);
    }
    filterModel->setFilterRegExp(regExp);
    //mainTableView->setFocus();
}

void ReferenceDialog::clearFilter()
{
    findLineEdit->setText("");
    QRegExp regExp("", Qt::CaseInsensitive, QRegExp::RegExp);
    filterModel->invalidate();
    filterModel->setFilterRegExp(regExp);
    findLabel->setStyleSheet("QLabel {color: black;}");
    filterFlag = false;
    //clearPushButton->setDisabled(true);
    mainTableView->setFocus();
}

void ReferenceDialog::selectionChanged()
{
    if (mainTableView->selectionModel()->hasSelection()) {
        viewPushButton->setEnabled(true);
        editPushButton->setEnabled(true);
        delPushButton->setEnabled(true);
    } else {
        viewPushButton->setDisabled(true);
        editPushButton->setDisabled(true);
        delPushButton->setDisabled(true);
    }
}

void ReferenceDialog::addAct()
{

}

void ReferenceDialog::editAct()
{

}

void ReferenceDialog::delAct()
{

}

void ReferenceDialog::viewAct()
{

}
