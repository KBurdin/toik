#include <QtWidgets>
#include <QFont>
#include <QSqlQuery>
#include <QDesktopServices>
#include <QMessageBox>
#include <QStatusBar>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDebug>
#include <QAbstractItemView>
#include <QRegExp>
#include <QKeySequence>

#include "maindialog.h"
#include "mainwindow.h"
#include "dbman.h"
#include "printdialog.h"
#include "multicolumnsortfilterproxymodel.h"

MainDialog::MainDialog(const QString &query, SqlModel *model, const QString &title):
    printer(new QPrinter),
    tablePrintDialog(new PrintDialog),
    mainDialogVBoxLayout(new QVBoxLayout),
    dateTaskGridLayout(new QGridLayout),
    tdyLabel(new QLabel(tr("Today:"))),
    curWeekLabel(new QLabel(tr("Current week:"))),
    taskGridLayout(new QGridLayout),
    taskLabel(new QLabel(tr("Tasks for this week"))),
    ndoneTaskLabel(new QLabel(tr("Not completed:", "header"))),
    doneTaskLabel(new QLabel(tr("Completed:", "header"))),
    totalTaskLabel(new QLabel(tr("Total:"))),
    taskVolLabel(new QLabel(tr("Manpowers"))),
    ndoneTaskVolLabel(new QLabel(tr("Not completed:", "header"))),
    doneTaskVolLabel(new QLabel(tr("Completed:", "header"))),
    totalTaskVolLabel(new QLabel(tr("Total:"))),
    tdyValLabel(new QLabel()),
    curWeekValLabel(new QLabel()),
    ndoneTaskValLabel(new QLabel()),
    doneTaskValLabel(new QLabel()),
    totalTaskValLabel(new QLabel()),
    ndoneTaskVolValLabel(new QLabel()),
    doneTaskVolValLabel(new QLabel()),
    totalTaskVolValLabel(new QLabel()),
    taskFindHBoxLayout(new QHBoxLayout),
    taskFindLabel(new QLabel(tr("Find:"))),
    taskFindLineEdit(new QLineEdit),
    viewTaskFindLabel(new QLabel(tr("Display"))),
    countViewTaskFindComboBox(new QComboBox),
    findTaskPushButton(new QPushButton(tr("Find"))),
    clearFindTaskPushButton(new QPushButton(tr("Clear"))),
    addActionPushButton(new QPushButton(tr("Add"))),
    editActionPushButton(new QPushButton(tr("Edit"))),
    delActionPushButton(new QPushButton(tr("Delete"))),
    viewActionPushButton(new QPushButton(tr("View"))),
    taskTableGroupBox(new QGroupBox(title)),
    taskTableVBoxLayout(new QVBoxLayout),
    taskTableView(new QTableView),
    bottomLineTaskHBoxLayout(new QHBoxLayout),
    prevPushButton(new QPushButton(tr("Prev"))),
    nextPushButton(new QPushButton(tr("Next"))),
    homePushButton(new QPushButton(tr("Home"))),
    endPushButton(new QPushButton(tr("End"))),
    printVerTaskCommandLinkButton(new QCommandLinkButton(tr("Print"))),
    nextTaskBrowserLabel(new QLabel(tr("Next"))),
    nextTaskBrowserCommandLinkButton(new QCommandLinkButton),
    filterFlag(false),
    doneFilterFlag(false),
    ndoneFilterFlag(false),
    totalFilterFlag(false),
    filterPattern("")

{

    tdyLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    tdyValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 8pt;}");    
    curWeekLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    curWeekValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 8pt;}");    
    dateTaskGridLayout->addWidget(tdyLabel, 0, 0, Qt::AlignRight);
    dateTaskGridLayout->addWidget(tdyValLabel, 0, 1, Qt::AlignLeft);
    dateTaskGridLayout->addWidget(curWeekLabel, 1, 0, Qt::AlignRight);
    dateTaskGridLayout->addWidget(curWeekValLabel, 1, 1, Qt::AlignLeft);
    dateTaskGridLayout->setColumnStretch(1, 1);
    mainDialogVBoxLayout->addLayout(dateTaskGridLayout);

    taskLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt; font: bold;}");    
    ndoneTaskLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    doneTaskLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    totalTaskLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    ndoneTaskValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 10pt;}");
    ndoneTaskValLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(ndoneTaskValLabel, &QLabel::linkActivated, this, &MainDialog::ndoneTaskFilter);

    doneTaskValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 10pt;}");
    doneTaskValLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(doneTaskValLabel, &QLabel::linkActivated, this, &MainDialog::doneTaskFilter);

    totalTaskValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 10pt;}");
    totalTaskValLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(totalTaskValLabel, &QLabel::linkActivated, this, &MainDialog::totalTaskFilter);

    taskVolLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt; font: bold;}");    
    ndoneTaskVolLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    doneTaskVolLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    totalTaskVolLabel->setStyleSheet("QLabel {font-family: Arial; font-size: 8pt;}");    
    ndoneTaskVolValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 10pt;}");    
    doneTaskVolValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 10pt;}");    
    totalTaskVolValLabel->setStyleSheet("QLabel {color: blue; font-family: Arial; font-size: 10pt;}");

    taskGridLayout->addWidget(taskLabel, 0, 0, 1, 2, Qt::AlignLeft);
    taskGridLayout->addWidget(ndoneTaskLabel, 1, 0, Qt::AlignRight);
    taskGridLayout->addWidget(doneTaskLabel, 2, 0, Qt::AlignRight);
    taskGridLayout->addWidget(totalTaskLabel, 3, 0, Qt::AlignRight);
    taskGridLayout->addWidget(ndoneTaskValLabel, 1, 1, Qt::AlignLeft);
    taskGridLayout->addWidget(doneTaskValLabel, 2, 1, Qt::AlignLeft);
    taskGridLayout->addWidget(totalTaskValLabel, 3, 1, Qt::AlignLeft);

    taskGridLayout->setColumnMinimumWidth(2, 30);

    taskGridLayout->addWidget(taskVolLabel, 0, 3, 1, 2, Qt::AlignLeft);
    taskGridLayout->addWidget(ndoneTaskVolLabel, 1, 3, Qt::AlignRight);
    taskGridLayout->addWidget(doneTaskVolLabel, 2, 3, Qt::AlignRight);
    taskGridLayout->addWidget(totalTaskVolLabel, 3, 3, Qt::AlignRight);
    taskGridLayout->addWidget(ndoneTaskVolValLabel, 1, 4, Qt::AlignLeft);
    taskGridLayout->addWidget(doneTaskVolValLabel, 2, 4, Qt::AlignLeft);
    taskGridLayout->addWidget(totalTaskVolValLabel, 3, 4, Qt::AlignLeft);
    taskGridLayout->setColumnStretch(4, 1);

    mainDialogVBoxLayout->addLayout(taskGridLayout);
    taskFindLabel->setBuddy(taskFindLineEdit);
    taskFindLineEdit->setMinimumWidth(180);
    viewTaskFindLabel->setMinimumWidth(75);
    viewTaskFindLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    viewTaskFindLabel->setHidden(true);    
    countViewTaskFindComboBox->setHidden(true);    
    findTaskPushButton->setHidden(true);
    editActionPushButton->setDisabled(true);    
    delActionPushButton->setDisabled(true);    
    viewActionPushButton->setDisabled(true);

    taskFindHBoxLayout->addWidget(taskFindLabel);
    taskFindHBoxLayout->addWidget(taskFindLineEdit);
    taskFindHBoxLayout->addWidget(findTaskPushButton);
    taskFindHBoxLayout->addWidget(clearFindTaskPushButton);
    taskFindHBoxLayout->addWidget(viewTaskFindLabel);
    taskFindHBoxLayout->addWidget(countViewTaskFindComboBox);
    countViewTaskFindComboBox->addItem(tr("All"), 0);
    countViewTaskFindComboBox->addItem("5", 5);
    countViewTaskFindComboBox->addItem("10", 10);
    countViewTaskFindComboBox->addItem("15", 15);
    countViewTaskFindComboBox->addItem("20", 20);
    countViewTaskFindComboBox->addItem("25", 25);
    countViewTaskFindComboBox->addItem("30", 30);
    countViewTaskFindComboBox->setEditable(true);
    taskFindHBoxLayout->addStretch(1);
    taskFindHBoxLayout->addWidget(viewActionPushButton);
    taskFindHBoxLayout->addSpacing(20);
    taskFindHBoxLayout->addWidget(addActionPushButton);
    taskFindHBoxLayout->addWidget(editActionPushButton);
    taskFindHBoxLayout->addWidget(delActionPushButton);

    connect(addActionPushButton, SIGNAL(clicked(bool)), this, SLOT(addActionSlot()));
    connect(editActionPushButton, SIGNAL(clicked(bool)), this, SLOT(editActionSlot()));
    connect(delActionPushButton, SIGNAL(clicked(bool)), this, SLOT(delActionSlot()));
    connect(viewActionPushButton, SIGNAL(clicked(bool)), this, SLOT(viewActionSlot()));
    connect(countViewTaskFindComboBox, SIGNAL(editTextChanged(QString)), this, SLOT(maxPrintRowsChanged()));
    taskTableGroupBox->setStyleSheet("QGroupBox {color: brown; font-size: 18px; font-weight: bold; }");
    connect(taskTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(viewActionSlot()));
    deleteShortcut = new QShortcut(QKeySequence(QKeySequence::Delete), taskTableView);
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(delActionSlot()));
    taskTableVBoxLayout->addLayout(taskFindHBoxLayout);
    taskTableVBoxLayout->addWidget(taskTableView);
    nextTaskBrowserCommandLinkButton->setMaximumWidth(30);
    connect(prevPushButton, SIGNAL(clicked()), this, SLOT(prevPageTasks()));
    connect(nextPushButton, SIGNAL(clicked()), this, SLOT(nextPageTasks()));
    connect(homePushButton, SIGNAL(clicked()), this, SLOT(homePageTasks()));
    connect(endPushButton, SIGNAL(clicked()), this, SLOT(endPageTasks()));

    bottomLineTaskHBoxLayout->addWidget(printVerTaskCommandLinkButton);

    bottomLineTaskHBoxLayout->addStretch(1);

    bottomLineTaskHBoxLayout->addWidget(prevPushButton);
    bottomLineTaskHBoxLayout->addWidget(nextPushButton);
    bottomLineTaskHBoxLayout->addWidget(homePushButton);
    bottomLineTaskHBoxLayout->addWidget(endPushButton);
    taskTableVBoxLayout->addLayout(bottomLineTaskHBoxLayout);

    taskTableGroupBox->setLayout(taskTableVBoxLayout);
    mainDialogVBoxLayout->addWidget(taskTableGroupBox);

    mainSqlQueryModel = model;
    loadTable(query);
    connect(taskTableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortOperation(int)));

    connect(taskFindLineEdit, SIGNAL(returnPressed()), this, SLOT(setFilterTasks()));
    connect(taskFindLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setFilterTasks()));
    connect(findTaskPushButton, SIGNAL(clicked()), this, SLOT(setFilterTasks()));
    connect(clearFindTaskPushButton, SIGNAL(clicked()), this, SLOT(resetFilterTasks()));

    connect(printVerTaskCommandLinkButton, SIGNAL(clicked(bool)), this, SLOT(printPreview()));

    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

    this->setLayout(mainDialogVBoxLayout);
}

void MainDialog::setTdy(const QString &text)
{
    tdyValLabel->setText(text);
}

void MainDialog::setCurWeek(const QString &text)
{
    curWeekValLabel->setText(text);
}

void MainDialog::setNdoneTask(const QString &text)
{
    ndoneTaskValLabel->setText(text);
}

void MainDialog::setDoneTask(const QString &text)
{
    doneTaskValLabel->setText(text);
}

void MainDialog::setTotalTask(const QString &text)
{
    totalTaskValLabel->setText(text);
}

void MainDialog::setNdoneTaskVol(const QString &text)
{
    ndoneTaskVolValLabel->setText(text);
}

void MainDialog::setDoneTaskVol(const QString &text)
{
    doneTaskVolValLabel->setText(text);
}

void MainDialog::setTotalTaskVol(const QString &text)
{
    totalTaskVolValLabel->setText(text);
}

void MainDialog::loadTable(const QString &query)
{
    mainSqlQueryModel->setQuery(query);
    //mainProxyTableModel = new QSortFilterProxyModel;
    mainProxyTableModel = new MultiColumnSortFilterProxyModel;
    mainProxyTableModel->setSourceModel(mainSqlQueryModel);
    taskTableView->setModel(mainProxyTableModel);

    //mainSqlQueryModel->setQuery(mainSqlQueryModel->query().lastQuery());

    taskTableView->setSelectionMode(QAbstractItemView::SingleSelection);

}

QTableView *MainDialog::getTableView()
{
    return taskTableView;
}

SqlModel *MainDialog::getSqlQueryModel()
{
    return mainSqlQueryModel;
}

QSortFilterProxyModel *MainDialog::getProxyModel()
{
    return (QSortFilterProxyModel *) mainProxyTableModel;
}

void MainDialog::weekFilter()
{

}

void MainDialog::ndoneTaskFilter()
{
   taskTableView->setFocus();
   doneTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
   totalTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
   ndoneTaskLabel->setStyleSheet("QLabel {color: red; font-family: Arial; font-size: 8pt;}");
   doneFilterFlag = false;
   totalFilterFlag = false;
   ndoneFilterFlag = true;
   setFilterTasks();
   homePageTasks();
}

void MainDialog::doneTaskFilter()
{
    taskTableView->setFocus();
    ndoneTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
    totalTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
    doneTaskLabel->setStyleSheet("QLabel {color: red; font-family: Arial; font-size: 8pt;}");
    ndoneFilterFlag = false;
    totalFilterFlag = false;
    doneFilterFlag = true;
    setFilterTasks();
    homePageTasks();
}

void MainDialog::totalTaskFilter()
{
    taskTableView->setFocus();
    doneTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
    ndoneTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
    totalTaskLabel->setStyleSheet("QLabel {color: red; font-family: Arial; font-size: 8pt;}");
    ndoneFilterFlag = false;
    doneFilterFlag = false;
    totalFilterFlag = true;
    setFilterTasks();
    homePageTasks();
}

void MainDialog::addActionSlot()
{
    emit addAction();
}

void MainDialog::editActionSlot()
{
    emit editAction();
}

void MainDialog::delActionSlot()
{
    //if (taskTableView->selectionModel()->hasSelection()) {
        emit delAction();
    //}
}

void MainDialog::viewActionSlot()
{
    emit viewAction();
}

void MainDialog::maxPrintRowsChanged()
{
    qDebug() << "filter";
    mainProxyTableModel->setMaxPrintRows(countViewTaskFindComboBox->currentData().toInt());
    if (countViewTaskFindComboBox->currentData().toInt() != 0) {
        viewTaskFindLabel->setStyleSheet("QLabel {color: red;}");
    } else {
        viewTaskFindLabel->setStyleSheet("QLabel {color: black;}");
    }
    /*
    QRegExp regExp(mainProxyTableModel->filterRegExp().pattern(),\
                   mainProxyTableModel->filterRegExp().caseSensitivity(), QRegExp::RegExp);
    mainProxyTableModel->setFilterRegExp(regExp);
    */
    mainProxyTableModel->invalidate();
    //setFilterTasks();
    //emit countAction();
}

void MainDialog::selectionChanged()
{
    if (taskTableView->selectionModel()->hasSelection()) {
        //qDebug() << "selection true";
        viewActionPushButton->setEnabled(true);
        editActionPushButton->setEnabled(true);
        delActionPushButton->setEnabled(true);
    } else {
        //qDebug() << "selection false";
        viewActionPushButton->setDisabled(true);
        editActionPushButton->setDisabled(true);
        delActionPushButton->setDisabled(true);
    }
}

void MainDialog::sortOperation(int column)
{
    Qt::SortOrder order = mainProxyTableModel->sortOrder();
        if (mainProxyTableModel->sortColumn() == column)
            order = mainProxyTableModel->sortOrder() == Qt::AscendingOrder
                    ? Qt::DescendingOrder
                    : Qt::AscendingOrder;

        if (qApp->keyboardModifiers() & Qt::CTRL) {
            if (mainProxyTableModel->isColumnInSort(column)) {
                Qt::SortOrder order = mainProxyTableModel->sortOrder(column) == Qt::AscendingOrder
                                      ? Qt::DescendingOrder
                                      : Qt::AscendingOrder;
                mainProxyTableModel->changeSortedColumn(column, order);
            } else {
                mainProxyTableModel->addSortedColumn(column, mainProxyTableModel->sortOrder());
            }
        } else {
            Qt::SortOrder order = mainProxyTableModel->sortOrder();
            if (mainProxyTableModel->isColumnInSort(column)) {
                order = mainProxyTableModel->sortOrder(column) == Qt::AscendingOrder
                        ? Qt::DescendingOrder
                         : Qt::AscendingOrder;
            }
            mainProxyTableModel->clearSort();
            mainProxyTableModel->addSortedColumn(column, order);
        }
}

void MainDialog::setFilterTasks()
{
    QString pattern = taskFindLineEdit->displayText();

    if (pattern != "") {
        taskFindLabel->setStyleSheet("QLabel {color: red;}");
        filterFlag = true;
        //clearFindTaskPushButton->setEnabled(true);
    } else {
        taskFindLabel->setStyleSheet("QLabel {color: black;}");
        filterFlag = false;
        //clearFindTaskPushButton->setDisabled(true);
    }

    QString year = QString::number(QDate::currentDate().year());
    QString week = QString("%1").arg(QDate::currentDate().weekNumber(), 2, 10, QLatin1Char('0'));

    if (doneFilterFlag) {
        pattern += ".*\\s\\s" + week + year + "\\s\\s(" + tr("Completed", "sql query") + ")";
    } else if (ndoneFilterFlag) {
        pattern +=".*\\s\\s" + week + year + "\\s\\s(" + tr("Not completed", "sql query") + "|" + tr("In process", "sql query") + ")";
    } else if (totalFilterFlag) {
        pattern +=".*\\s\\s" + week + year;
    }

    QRegExp regExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp);
    mainProxyTableModel->setFilterRegExp(regExp);
    //emit countAction();
}

void MainDialog::resetFilterTasks()
{
    taskFindLineEdit->setText("");
    QRegExp regExp("", Qt::CaseInsensitive, QRegExp::RegExp);
    mainProxyTableModel->invalidate();
    mainProxyTableModel->setFilterRegExp(regExp);
    taskFindLabel->setStyleSheet("QLabel {color: black;}");
    doneTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
    ndoneTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
    totalTaskLabel->setStyleSheet("QLabel {color: black; font-family: Arial; font-size: 8pt;}");
    filterFlag = false;
    doneFilterFlag = false;
    ndoneFilterFlag = false;
    totalFilterFlag = false;

    //emit countAction();
    //clearFindTaskPushButton->setDisabled(true);
    //taskTableView->setFocus();
}

void MainDialog::prevPageTasks()
{
    taskTableView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
    //taskTableView->setFocus();
}

void MainDialog::nextPageTasks()
{
    taskTableView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
    //taskTableView->setFocus();
}

void MainDialog::homePageTasks()
{
    taskTableView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMinimum);
    //taskTableView->setFocus();
}

void MainDialog::endPageTasks()
{
    taskTableView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
    //taskTableView->setFocus();
}

void MainDialog::tablePrint()
{
    setFilterTasks();
    tablePrintDialog->tablePrint(taskTableView, printer);
}

void MainDialog::printPreview()
{

    QPrintPreviewDialog preview(printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(tablePrint()));
    preview.exec();
}
