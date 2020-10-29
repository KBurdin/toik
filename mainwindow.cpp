#include <QtWidgets>
#include <QDateTime>
#include <QDate>
#include <QTimer>
#include <QTime>
#include <QAction>

#include "mainwindow.h"
#include "rtodialog.h"
#include "sstsdialog.h"
#include "rpdialog.h"
#include "rkdialog.h"
#include "dbman.h"
#include "trmessagebox.h"

MainWindow::MainWindow(const userType &user, DbManager *db):
    timer(new QTimer(this))

{
    curDay = QDate::currentDate().day();

    timer->setInterval(1000);    

    mainDB = db;
    userData = user;
    init();

    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimeDate);
    timer->start();


}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (doExit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::about()
{
   TrMessageBox::about(this, tr("About TOiK"), QString(tr("<b>TOiK</b> <left>Maintenance and calibration workstation.</left>" \
                                                          "<br><left>LAES-2 powerunit 1, ASRK</left>" \
                                                          "<br><left>AO RASU 2017 version ") + SOFT_VERSION + "</left>") +
                                                          "<br><left>Разработал Макс Еремин mec@mail.ru</left>");
}

void MainWindow::selectRTOWindow()
{
    mainWindows->setCurrentWidget(rtoWindow);
}

void MainWindow::selectSSTSWindow()
{
   mainWindows->setCurrentWidget(sstsWindow);
}

void MainWindow::selectRPWindow()
{
    mainWindows->setCurrentWidget(rpWindow);
}

void MainWindow::selectRKWindow()
{
    mainWindows->setCurrentWidget(rkWindow);
}

void MainWindow::selectMainWindow(QWidget *workWindow)
{
    mainWindows->setCurrentWidget(workWindow);
}

void MainWindow::updateRTOWindow()
{
    updateMainWindowHeader(rtoWindow, RTO_TABLE);
    rtoWindow->setFilterTasks();
}

void MainWindow::updateRPWindow()
{
    updateMainWindowHeader(rpWindow, RP_TABLE);
    rpWindow->setFilterTasks();
}

void MainWindow::updateRKWindow()
{
    updateMainWindowHeader(rkWindow, RK_TABLE);
}

void MainWindow::updateTimeDate()
{
    if (QDate::currentDate().day() != curDay) {
        curDay = QDate::currentDate().day();
        //emit curDayChanged();
        updateRTOWindow();
        updateRPWindow();
        updateRKWindow();
    }
    emit timerAlarm();
}

void MainWindow::openUsersWindow()
{
    usersWindows->show();
    usersWindows->getUsersTable()->setFocus();
}

void MainWindow::openTaskCodeWindow()
{
    taskCodeWindow->show();
    usersWindows->getUsersTable()->setFocus();

}

void MainWindow::openTaskWindow()
{
    taskWindow->show();
    usersWindows->getUsersTable()->setFocus();
}

void MainWindow::openDeviceCodeWindow()
{
    deviceCodeWindow->show();
    usersWindows->getUsersTable()->setFocus();
}

void MainWindow::openDeviceWindow()
{
    deviceWindow->show();
    usersWindows->getUsersTable()->setFocus();
}

void MainWindow::updateMainWindowHeader(MainDialog *curWindow, const QString &table)
{
    int ndoneTaskCount;
    int doneTaskCount;
    int totalTaskCount;
    int ndoneTaskVolSum;
    int doneTaskVolSum;
    int totalTaskVolSum;

    ndoneTaskCount = mainDB->columnProc("SELECT COUNT(id) FROM " + table + " WHERE (STATUS_VAL = 0 OR STATUS_VAL = 2) AND " + \
                                        "(week = " + QString::number(QDate::currentDate().weekNumber()) + " AND " + \
                                        "year = " + QString::number(QDate::currentDate().year()) + ")");
    doneTaskCount = mainDB->columnProc("SELECT COUNT(id) FROM " + table + " WHERE STATUS_VAL = 1 AND " + \
                                       "(week = " + QString::number(QDate::currentDate().weekNumber()) + " AND " + \
                                       "year = " + QString::number(QDate::currentDate().year()) + ")");
    totalTaskCount = mainDB->columnProc("SELECT COUNT(id) FROM " + table + " WHERE week = " + \
                                        QString::number(QDate::currentDate().weekNumber()) + " AND " + \
                                        "year = " + QString::number(QDate::currentDate().year()));

    ndoneTaskVolSum = mainDB->columnProc("SELECT SUM(amount) FROM " + table + " WHERE (STATUS_VAL = 0 OR STATUS_VAL = 2) AND " + \
                                         "(week = " + QString::number(QDate::currentDate().weekNumber()) + " AND " + \
                                         "year = " + QString::number(QDate::currentDate().year()) + ")");
    doneTaskVolSum = mainDB->columnProc("SELECT SUM(amount) FROM " + table + " WHERE STATUS_VAL = 1 AND " + \
                                        "(week = " + QString::number(QDate::currentDate().weekNumber()) + " AND " + \
                                        "year = " + QString::number(QDate::currentDate().year()) + ")");
    totalTaskVolSum = mainDB->columnProc("SELECT SUM(amount) FROM " + table + " WHERE week = " + \
                                         QString::number(QDate::currentDate().weekNumber()) + " AND " + \
                                         "year = " + QString::number(QDate::currentDate().year()));

    curWindow->setTdy(QDate::currentDate().toString("dd.MM.yyyy, dddd"));

    int yearOfWeek;
    QDate::currentDate().weekNumber(&yearOfWeek);
    curWindow->setCurWeek( QDate::currentDate().toString("yyyy, ") + QString::number(QDate::currentDate().weekNumber()) + \
                           " : " + WeekCalc::getWeekRange(QDate::currentDate().weekNumber(), yearOfWeek, QDate::currentDate().year()));

    curWindow->setNdoneTask("<a href=\"whatever\">" + QString::number(ndoneTaskCount) + tr(" pcs</a>"));
    curWindow->setDoneTask("<a href=\"whatever\">" + QString::number(doneTaskCount) + tr(" pcs</a>"));
    curWindow->setTotalTask("<a href=\"whatever\">" + QString::number(totalTaskCount) + tr(" pcs</a>"));
    curWindow->setNdoneTaskVol(QString::number(ndoneTaskVolSum) + tr(" man-hour"));
    curWindow->setDoneTaskVol(QString::number(doneTaskVolSum) + tr(" man-hour"));
    curWindow->setTotalTaskVol(QString::number(totalTaskVolSum) + tr(" man-hour"));
}

void MainWindow::updateTime()
{
    setWindowTitle(tr("TOiK ASRK LenAES-2") + " — " + QTime::currentTime().toString());
}

void MainWindow::nop()
{
    ;
}

void MainWindow::setTableFocus()
{
    ((MainDialog *) mainWindows->currentWidget())->getTableView()->setFocus();
}

void MainWindow::debugSlot()
{
    //qDebug() << "signal: dataChanged";
}

void MainWindow::init()
{
    QString sql_query;
    setAttribute(Qt::WA_DeleteOnClose);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(850, 550);

    statusBarLabel = new QLabel(tr("User: ") + userData.l_name + " " + userData.f_name + " " + userData.m_name);



    statusBar()->addPermanentWidget(statusBarLabel);

    mainWindows = new QStackedWidget;

    rto_tableModel = new RTOTableModel;
    sql_query = "SELECT id, week, '' AS week_name, year, task_code, dev_kks, dev_part_num, task_id, task_name, dev_id, amount, period, " \
            "CASE WHEN status_val = 0 THEN \"" + tr("Not completed") + "\" " + \
                 "WHEN status_val = 1 THEN \"" + tr("Completed") + "\" " + \
                 "ELSE \"\" END AS status, " + \
            "status_val, comment, task_code || dev_kks || dev_part_num || '  ' || " + \
            "CASE WHEN week < 10 THEN '0' || week ELSE week END || year || '  ' || " + \
            "CASE WHEN status_val = 0 THEN \"" + tr("Not completed", "SQL query") + "\" " + \
                 "WHEN status_val = 1 THEN \"" + tr("Completed", "SQl query") + "\" " + \
                 "ELSE \"\" END " + \
            "AS find_field FROM rto_list_view ORDER BY year, week, id DESC";
    rtoWindow = new RTODialog(sql_query, rto_tableModel, tr("Task schedule"));
    connect(rtoWindow->getTableView()->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), \
            rtoWindow, SLOT(selectionChanged()));

    sstsWindow = new SSTSDialog;

    rp_tableModel = new RPTableModel;
    sql_query = "SELECT id, week, '' AS week_name, year, kks, part_num, serial_num, amount, period, " \
            "CASE WHEN status_val = 0 THEN \"" + tr("Not completed") + "\" " + \
                 "WHEN status_val = 1 THEN \"" + tr("Completed") + "\" " + \
                 "WHEN status_val = 2 THEN \"" + tr("In process") + "\" " + \
                 "ELSE \"\" END AS status, " + \
            "report, status_val, comment, kks || part_num || serial_num || " + \
            "CASE WHEN report IS NULL THEN \"\" ELSE report END || '  ' || " + \
            "CASE WHEN week < 10 THEN '0' || week ELSE week END || year || '  ' || " + \
            "CASE WHEN status_val = 0 THEN \"" + tr("Not completed") + "\" " + \
                 "WHEN status_val = 1 THEN \"" + tr("Completed") + "\" " + \
                 "WHEN status_val = 2 THEN \"" + tr("In process") + "\" " + \
                 "ELSE \"\" END " + \
            "AS find_field FROM rp_list_view " + \
            "ORDER BY year, week, id DESC";
    rpWindow = new RPDialog(sql_query, rp_tableModel, tr("Verify schedule"));
    connect(rpWindow->getTableView()->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), \
            rpWindow, SLOT(selectionChanged()));

    rk_tableModel = new RKTableModel;
    sql_query = "SELECT id, week, '' AS week_name, year, kks, part_num, serial_num, amount, period, " \
            "CASE WHEN status_val = 0 THEN \"" + tr("Not completed") + "\" " + \
                 "WHEN status_val = 1 THEN \"" + tr("Completed") + "\" " + \
                 "WHEN status_val = 2 THEN \"" + tr("In process") + "\" " + \
                 "ELSE \"\" END AS status, " + \
            "report, status_val, comment, kks || part_num || serial_num || " + \
            "CASE WHEN report IS NULL THEN \"\" ELSE report END || '  ' || " + \
            "CASE WHEN week < 10 THEN '0' || week ELSE week END || year || '  ' || " + \
            "CASE WHEN status_val = 0 THEN \"" + tr("Not completed") + "\" " + \
                 "WHEN status_val = 1 THEN \"" + tr("Completed") + "\" " + \
                 "WHEN status_val = 2 THEN \"" + tr("In process") + "\" " + \
                 "ELSE \"\" END " + \
            "AS find_field FROM rk_list_view " + \
            "ORDER BY year, week, id DESC";
    rkWindow = new RKDialog(sql_query, rk_tableModel, tr("Calibrate schedule"));
    connect(rkWindow->getTableView()->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), \
            rkWindow, SLOT(selectionChanged()));

    mainWindows->addWidget(rtoWindow);
    mainWindows->addWidget(sstsWindow);
    mainWindows->addWidget(rpWindow);
    mainWindows->addWidget(rkWindow);
    //mainWindows->addWidget(refWindow);
    setCentralWidget(mainWindows);

    usersWindows = new UsersDialog(userData);

    rtoEditWindow = new RTOEditDialog(rtoWindow->getTableView(), rtoWindow->getProxyModel(), rtoWindow);
    connect(rtoWindow, SIGNAL(addAction()), rtoEditWindow, SLOT(addAct()));
    connect(rtoWindow, SIGNAL(editAction()), rtoEditWindow, SLOT(editAct()));
    connect(rtoWindow, SIGNAL(delAction()), rtoEditWindow, SLOT(delAct()));
    connect(rtoWindow, SIGNAL(viewAction()), rtoEditWindow, SLOT(viewAct()));


    rpEditWindow = new RPEditDialog(rpWindow->getTableView(), rpWindow->getProxyModel(), rpWindow);
    connect(rpWindow, SIGNAL(addAction()), rpEditWindow, SLOT(addAct()));
    connect(rpWindow, SIGNAL(editAction()), rpEditWindow, SLOT(editAct()));
    connect(rpWindow, SIGNAL(delAction()), rpEditWindow, SLOT(delAct()));
    connect(rpWindow, SIGNAL(viewAction()), rpEditWindow, SLOT(viewAct()));

    rkEditWindow = new RKEditDialog(rkWindow->getTableView(), rkWindow->getProxyModel(), rkWindow);
    connect(rkWindow, SIGNAL(addAction()), rkEditWindow, SLOT(addAct()));
    connect(rkWindow, SIGNAL(editAction()), rkEditWindow, SLOT(editAct()));
    connect(rkWindow, SIGNAL(delAction()), rkEditWindow, SLOT(delAct()));
    connect(rkWindow, SIGNAL(viewAction()), rkEditWindow, SLOT(viewAct()));

    taskCodeTableView = new QTableView;
    taskCodeWindow = new TaskCodeDialog(Q_NULLPTR, taskCodeTableView);
    taskCodeWindow->hideViewButton();

    taskTableView = new QTableView;
    taskWindow = new TaskDialog(Q_NULLPTR, taskTableView);    

    deviceCodeTableView = new QTableView;
    deviceCodeWindow = new DeviceCodeDialog(Q_NULLPTR, deviceCodeTableView);

    deviceTableView = new QTableView;
    deviceWindow = new DeviceDialog(Q_NULLPTR, deviceTableView);
    connect(deviceWindow, SIGNAL(dbChanged()), deviceWindow->getModel(), SLOT(updateModel()));
    connect(deviceWindow, SIGNAL(dbChanged()), rtoEditWindow, SLOT(taskModelUpdate()));

    connect(taskCodeWindow, SIGNAL(dbChanged()), taskCodeWindow->getModel(), SLOT(updateModel()));
    connect(taskCodeWindow, SIGNAL(dbChanged()), taskWindow->getModel(), SLOT(updateModel()));
    connect(taskCodeWindow, SIGNAL(dbChanged()), taskWindow->getTaskModel(), SLOT(updateModel()));

    connect(taskWindow, SIGNAL(dbChanged()), taskWindow->getModel(), SLOT(updateModel()));

    connect(deviceCodeWindow, SIGNAL(dbChanged()), deviceCodeWindow->getModel(), SLOT(updateModel()));
    connect(deviceCodeWindow, SIGNAL(dbChanged()), deviceWindow->getModel(), SLOT(updateModel()));
    connect(deviceCodeWindow, SIGNAL(dbChanged()), deviceWindow->getDeviceModel(), SLOT(updateModel()));
    connect(deviceCodeWindow, SIGNAL(dbChanged()), taskWindow->getDeviceModel(), SLOT(updateModel()));

    connect(taskCodeWindow, SIGNAL(dbChanged()), rtoWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(taskWindow, SIGNAL(dbChanged()), rtoWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(deviceWindow, SIGNAL(dbChanged()), rtoWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(deviceCodeWindow, SIGNAL(dbChanged()), rtoWindow->getSqlQueryModel(), SLOT(updateModel()));

    connect(deviceWindow, SIGNAL(dbChanged()), rpWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(deviceCodeWindow, SIGNAL(dbChanged()), rpWindow->getSqlQueryModel(), SLOT(updateModel()));

    connect(deviceWindow, SIGNAL(dbChanged()), rkWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(deviceCodeWindow, SIGNAL(dbChanged()), rkWindow->getSqlQueryModel(), SLOT(updateModel()));

    connect(rtoEditWindow, SIGNAL(dbChanged()), rtoWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(rtoEditWindow, SIGNAL(dbChanged()), this, SLOT(updateRTOWindow()));

    connect(rpEditWindow, SIGNAL(dbChanged()), rpWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(rpEditWindow, SIGNAL(dbChanged()), this, SLOT(updateRPWindow()));

    connect(rkEditWindow, SIGNAL(dbChanged()), rkWindow->getSqlQueryModel(), SLOT(updateModel()));
    connect(rkEditWindow, SIGNAL(dbChanged()), this, SLOT(updateRKWindow()));

    connect(rtoWindow->getProxyModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(debugSlot()));

    createActions();
    createStatusBar();

    readSettings();

    setUnifiedTitleAndToolBarOnMac(true);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    emit updateTime();

    updateMainWindowHeader(rtoWindow, RTO_TABLE);
    updateMainWindowHeader(rpWindow, RP_TABLE);
    updateMainWindowHeader(rkWindow, RK_TABLE);
    connect(mainWindows, SIGNAL(currentChanged(int)), this, SLOT(setTableFocus()));
    //rtoWindow->getTableView()->setFocus();
}

void MainWindow::createActions()
{
    QMenu *taskMenu = menuBar()->addMenu(tr("&Maintenance"));
    QToolBar *taskToolBar = addToolBar(tr("Maintenance"));
    taskToolBar->setIconSize(QSize(32, 32));
    taskToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    const QIcon taskIcon = QIcon::fromTheme("task-to", QIcon(":/images/to2.png"));
    QAction *taskAct = new QAction(taskIcon, tr("&Tasks"), this);
    taskAct->setShortcut(QKeySequence("CTRL+T"));
    taskAct->setStatusTip(tr("Task schedule"));
    taskAct->setChecked(true);
    connect(taskAct, &QAction::triggered, this, &MainWindow::selectRTOWindow);
    taskMenu->addAction(taskAct);
    taskToolBar->addAction(taskAct);

    const QIcon signalIcon = QIcon::fromTheme("task-verify", QIcon(":/images/aes1.png"));
    QAction *signalAct = new QAction(signalIcon, tr("&Signals"), this);
    signalAct->setShortcut(QKeySequence("CTRL+S"));
    signalAct->setStatusTip(tr("View signals"));
    connect(signalAct, &QAction::triggered, this, &MainWindow::selectSSTSWindow);
    taskMenu->addAction(signalAct);
    taskToolBar->addAction(signalAct);
    signalAct->setVisible(false);

    const QIcon povIcon = QIcon::fromTheme("task-verify", QIcon(":/images/verify1.png"));
    QAction *povAct = new QAction(povIcon, tr("&Verify"), this);
    povAct->setShortcut(QKeySequence("CTRL+V"));
    povAct->setStatusTip(tr("Verify schedule"));
    connect(povAct, &QAction::triggered, this, &MainWindow::selectRPWindow);
    taskMenu->addAction(povAct);
    taskToolBar->addAction(povAct);

    const QIcon calIcon = QIcon::fromTheme("task-calibrate", QIcon(":/images/calibrate1.png"));
    QAction *calAct = new QAction(calIcon, tr("Cali&brate"), this);
    calAct->setShortcut(QKeySequence("CTRL+B"));
    calAct->setStatusTip(tr("Calibrate schedule"));
    connect(calAct, &QAction::triggered, this, &MainWindow::selectRKWindow);
    taskMenu->addAction(calAct);
    taskToolBar->addAction(calAct);

    /*
    const QIcon refIcon = QIcon::fromTheme("task-ref", QIcon(":/images/sprav3.png"));
    QAction *refAct = new QAction(refIcon, tr("&Reference"), this);
    refAct->setShortcut(QKeySequence("CTRL+V"));
    refAct->setStatusTip(tr("References"));
    connect(refAct, &QAction::triggered, this, &MainWindow::selectREFWindow);
    taskMenu->addAction(refAct);
    taskToolBar->addAction(refAct);
    */


    taskMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("task-quit", QIcon(":/images/exit.png"));
    QAction *exitAct = new QAction(exitIcon, tr("&Quit"), qApp);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Quit the program"));
    connect(exitAct, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    taskMenu->addAction(exitAct);


    QMenu *editTaskMenu = menuBar()->addMenu(tr("&Reference"));

    const QIcon editDeviceCodeIcon = QIcon::fromTheme("editdevicecode", QIcon(":/images/shed_to.png"));
    QAction *editDeviceCodeAct = new QAction(editDeviceCodeIcon, tr("Device co&de list"), this);
    editDeviceCodeAct->setShortcut(QKeySequence("CTRL+D"));
    editDeviceCodeAct->setStatusTip(tr("Device code list operation"));
    connect(editDeviceCodeAct, &QAction::triggered, this, &MainWindow::openDeviceCodeWindow);
    editTaskMenu->addAction(editDeviceCodeAct);

    const QIcon editDeviceIcon = QIcon::fromTheme("editdevice", QIcon(":/images/shed_to.png"));
    QAction *editDeviceAct = new QAction(editDeviceIcon, tr("Device &list"), this);
    editDeviceAct->setShortcut(QKeySequence("CTRL+L"));
    editDeviceAct->setStatusTip(tr("Device list operation"));
    connect(editDeviceAct, &QAction::triggered, this, &MainWindow::openDeviceWindow);
    editTaskMenu->addAction(editDeviceAct);

    const QIcon editTaskCodeIcon = QIcon::fromTheme("edittaskcode-to", QIcon(":/images/shed_to.png"));
    QAction *editTaskCodeAct = new QAction(editTaskCodeIcon, tr("Ta&sk code list"), this);
    editTaskCodeAct->setShortcut(QKeySequence("CTRL+S"));
    editTaskCodeAct->setStatusTip(tr("Task code list operation"));
    connect(editTaskCodeAct, &QAction::triggered, this, &MainWindow::openTaskCodeWindow);
    editTaskMenu->addAction(editTaskCodeAct);

    const QIcon editTaskIcon = QIcon::fromTheme("edittask-to", QIcon(":/images/shed_to.png"));
    QAction *editTaskAct = new QAction(editTaskIcon, tr("T&ask list"), this);
    editTaskAct->setShortcut(QKeySequence("CTRL+A"));
    editTaskAct->setStatusTip(tr("Task list operation"));
    connect(editTaskAct, &QAction::triggered, this, &MainWindow::openTaskWindow);
    editTaskMenu->addAction(editTaskAct);

    QMenu *setupMenu = menuBar()->addMenu(tr("S&ettings"));
    const QIcon setupIcon = QIcon::fromTheme("setup", QIcon(":/images/setup.png"));
    QAction *setupAct = new QAction(setupIcon, tr("Pro&gram settings"), this);
    setupAct->setShortcut(QKeySequence("CTRL+G"));
    setupAct->setStatusTip(tr("Change the program settings"));
    connect(setupAct, &QAction::triggered, this, &MainWindow::nop);
    setupMenu->setHidden(true);


    const QIcon userIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/users.png"));
    QAction *userAct = new QAction(userIcon, tr("&User accounts"), this);
    userAct->setShortcut(QKeySequence("CTRL+U"));
    userAct->setStatusTip(tr("Change user accounts settings"));
    connect(userAct, &QAction::triggered, this, &MainWindow::openUsersWindow);
    setupMenu->addAction(userAct);

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = new QAction(tr("&About"), this);
            //helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Display the program help"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(aboutAct);

    QAction *aboutQtAct = new QAction(tr("About &Qt"), qApp);
        //helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Display the Qt library help"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    helpMenu->addAction(aboutQtAct);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}


bool MainWindow::doExit()
{
    TrMessageBox *trm = new TrMessageBox;
    /*
    trm->setButtonText(TrMessageBox::Yes, tr("Yes"));
    trm->setButtonText(TrMessageBox::No, tr("No"));
    trm->setButtonText(TrMessageBox::Cancel, tr("Cancel"));
    trm->setButtonText(TrMessageBox::Ok, tr("OK"));
    */
    const TrMessageBox::StandardButton ret = trm->warning(this, tr("TOiK"),
                               tr("Do you realy want to quit the program?"),
                               TrMessageBox::Yes | TrMessageBox::No);
    switch (ret) {
    case TrMessageBox::Yes:
        return true;
    case TrMessageBox::No:
        return false;
    default:
        break;
    }

    return true;
}
