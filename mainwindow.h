#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QStackedWidget>
#include <QDateTime>
#include <QTimer>

#include "dbman.h"
#include "rtodialog.h"
#include "rtotablemodel.h"
#include "rptablemodel.h"
#include "rktablemodel.h"
#include "sstsdialog.h"
#include "rpdialog.h"
#include "rkdialog.h"
#include "logindialog.h"
#include "usersdialog.h"
#include "taskcodedialog.h"
#include "taskdialog.h"
#include "devicecodedialog.h"
#include "devicedialog.h"
#include "rtoeditdialog.h"
#include "weekcalc.h"
#include "rpeditdialog.h"
#include "rkeditdialog.h"

#define  SOFT_VERSION   "1.07"

#define  RTO_TABLE  "rto_list_view"
#define  RP_TABLE   "rp_list_view"
#define  RK_TABLE   "rk_list_view"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const userType &user, DbManager *db);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void about();

    void selectRTOWindow();
    void selectSSTSWindow();
    void selectRPWindow();
    void selectRKWindow();
    void selectMainWindow(QWidget *workWindow);
    void updateRTOWindow();
    void updateRPWindow();
    void updateRKWindow();

    void updateTimeDate();

    void openUsersWindow();
    void openTaskCodeWindow();
    void openTaskWindow();
    void openDeviceCodeWindow();
    void openDeviceWindow();

    void updateTime();
    void nop();

    void setTableFocus();
    void debugSlot();

private:
    QWidget *curMainWindow = nullptr;

    userType userData;

    void init();
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool doExit();
    void updateMainWindowHeader(MainDialog *curWindow, const QString &table);

    DbManager *mainDB;

    QStackedWidget *mainWindows;

    UsersDialog *usersWindows;

    //QTextEdit *textEdit;

    RTOTableModel *rto_tableModel;
    RTODialog *rtoWindow;
    RTOEditDialog *rtoEditWindow;

    SSTSDialog *sstsWindow;

    RPTableModel *rp_tableModel;
    RPDialog *rpWindow;
    RPEditDialog *rpEditWindow;

    RKTableModel *rk_tableModel;
    RKDialog *rkWindow;
    RKEditDialog *rkEditWindow;


    TaskCodeDialog *taskCodeWindow;
    TaskDialog *taskWindow;
    DeviceCodeDialog *deviceCodeWindow;
    DeviceDialog *deviceWindow;

    QLabel *curTDLabel;

    //QPushButton *testButton1;
    QPushButton *testButton2;
    QPushButton *testButton3;
    QPushButton *testButton4;
    QPushButton *testButton5;

    //QHBoxLayout *testLayout1;
    QHBoxLayout *testLayout2;
    QHBoxLayout *testLayout3;
    QHBoxLayout *testLayout4;
    QHBoxLayout *testLayout5;

    QTableView *taskCodeTableView;
    QSqlQueryModel *taskCodeModel;

    QTableView *taskTableView;
    QSqlQueryModel *taskModel;

    QTableView *deviceCodeTableView;
    QSqlQueryModel *deviceCodeModel;

    QTableView *deviceTableView;
    QSqlQueryModel *deviceModel;
    QLabel *statusBarLabel;

    int curDay;
    QTimer *timer;

signals:
    void timerAlarm();

};

#endif
