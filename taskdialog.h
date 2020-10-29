#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QLineEdit>
#include <QComboBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVector>
#include <QTextEdit>
#include <QSortFilterProxyModel>

#include "valdialog.h"
#include "referencedialog.h"
#include "reftablemodel.h"
#include "sqlmodel.h"

#define  TASK_COMMENT_COLUMN 8

class TaskDialog : public ReferenceDialog
{
    Q_OBJECT

private:
   QGroupBox *mainGroupBox;
   QVBoxLayout *mainVBoxLayout;
   QVBoxLayout *buttonVBoxLayout;
   QHBoxLayout *mainHBoxLayout;
   QHBoxLayout *closeHBoxLayout;
   QPushButton *closePushButton;
   QPushButton *addPushButton;
   QPushButton *editPushButton;
   QPushButton *delPushButton;
   QTableView *mainTableView;
   //QSqlQueryModel *mainModel;
   RefTableModel *mainModel;
   QSortFilterProxyModel *proxyModel;

   ValDialog *taskDialog;

   QVector< QLineEdit* > taskLineEdits;

   QRegularExpression periodRegExp;
   QRegularExpressionValidator *periodValidator;
   QRegularExpression amountRegExp;
   QRegularExpressionValidator *amountValidator;

   SqlModel *taskCodeModel;
   SqlModel *deviceCodeModel;
   QFormLayout *taskFormLayout;
   QComboBox *taskCodeComboBox;
   QComboBox *deviceCodeComboBox;
   QLineEdit *taskNameLineEdit;
   QLineEdit *periodLineEdit;
   QLineEdit *amountLineEdit;
   QTextEdit *commentTextEdit;

   QHBoxLayout *taskCodeHBoxLayout;
   QHBoxLayout *deviceCodeHBoxLayout;
   QLineEdit *taskCodeLineEdit;
   QLineEdit *deviceCodeLineEdit;

   bool editedTaskName;

public:
   explicit TaskDialog(QWidget *parent = nullptr, QTableView *table = nullptr);

   RefTableModel *getModel();
   SqlModel *getTaskModel();
   SqlModel *getDeviceModel();

private slots:
   void setEdited(ValDialog *dialog);
   void setEditedTaskName();
   void updateTaskName();

signals:
   void dbChanged();

public slots:
   void addAct();
   void editAct();
   void delAct();
   void viewAct();

};

#endif // TASKDIALOG_H
