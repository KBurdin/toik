#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTableView>
#include <QCommandLinkButton>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QShortcut>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include "printdialog.h"
#include "sqlmodel.h"
#include "multicolumnsortfilterproxymodel.h"

class MainDialog: public QWidget
{
    Q_OBJECT

private slots:
    void ndoneTaskFilter();
    void doneTaskFilter();
    void totalTaskFilter();
    void addActionSlot();
    void editActionSlot();
    void delActionSlot();
    void viewActionSlot();
    void maxPrintRowsChanged();


public slots:
    void setFilterTasks();
    void resetFilterTasks();

    void prevPageTasks();
    void nextPageTasks();
    void homePageTasks();
    void endPageTasks();
    void tablePrint();
    void printPreview();
    void selectionChanged();
    void sortOperation(int column);

signals:
    void addAction();
    void editAction();
    void delAction();
    void viewAction();
    void countAction();

public:
    explicit MainDialog(const QString &query, SqlModel *model, const QString &title);

    QPrinter *printer;
    QPrintPreviewDialog *preview;

    void setTdy(const QString &text);
    void setCurWeek(const QString &text);
    void setNdoneTask(const QString &text);
    void setDoneTask(const QString &text);
    void setTotalTask(const QString &text);
    void setNdoneTaskVol(const QString &text);
    void setDoneTaskVol(const QString &text);
    void setTotalTaskVol(const QString &text);
    void loadTable(const QString &query);
    QTableView *getTableView();
    SqlModel *getSqlQueryModel();
    QSortFilterProxyModel *getProxyModel();
    //void printTask(QTableView *table);

private:
    PrintDialog *tablePrintDialog;

    QVBoxLayout *mainDialogVBoxLayout;

    QGridLayout *dateTaskGridLayout;
    QLabel *tdyLabel;
    QLabel *curWeekLabel;

    QGridLayout *taskGridLayout;
    QLabel *taskLabel;
    QLabel *ndoneTaskLabel;
    QLabel *doneTaskLabel;
    QLabel *totalTaskLabel;

    QLabel *taskVolLabel;
    QLabel *ndoneTaskVolLabel;
    QLabel *doneTaskVolLabel;
    QLabel *totalTaskVolLabel;

    QLabel *tdyValLabel;
    QLabel *curWeekValLabel;
    QLabel *ndoneTaskValLabel;
    QLabel *doneTaskValLabel;
    QLabel *totalTaskValLabel;
    QLabel *ndoneTaskVolValLabel;
    QLabel *doneTaskVolValLabel;
    QLabel *totalTaskVolValLabel;

    QHBoxLayout *taskFindHBoxLayout;
    QLabel *taskFindLabel;
    QLineEdit *taskFindLineEdit;
    QLabel *viewTaskFindLabel;
    QComboBox *countViewTaskFindComboBox;
    QPushButton *findTaskPushButton;
    QPushButton *clearFindTaskPushButton;
    QLabel *formatTaskFindLabel;
    QComboBox *formatTaskFindComboBox;

    QPushButton *addActionPushButton;
    QPushButton *editActionPushButton;
    QPushButton *delActionPushButton;
    QPushButton *viewActionPushButton;

    QGroupBox *taskTableGroupBox;
    QVBoxLayout *taskTableVBoxLayout;
    QTableView *taskTableView;

    QHBoxLayout *bottomLineTaskHBoxLayout;
    QPushButton *prevPushButton;
    QPushButton *nextPushButton;
    QPushButton *homePushButton;
    QPushButton *endPushButton;

    QCommandLinkButton *printVerTaskCommandLinkButton;
    QPushButton *printPreviewTaskPushButton;
    QComboBox *taskBrowserComboBox;
    QLabel *nextTaskBrowserLabel;
    QCommandLinkButton *nextTaskBrowserCommandLinkButton;

    SqlModel *mainSqlQueryModel;
    //QSortFilterProxyModel *mainProxyTableModel;
    MultiColumnSortFilterProxyModel *mainProxyTableModel;

    QShortcut *deleteShortcut;

    bool filterFlag;
    bool doneFilterFlag;
    bool ndoneFilterFlag;
    bool totalFilterFlag;
    QString filterPattern;

    void nop();
    void weekFilter();
};

#endif // MAINDIALOG_H
