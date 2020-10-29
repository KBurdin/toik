#ifndef TASKCODEDIALOG_H
#define TASKCODEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QFormLayout>
#include <QLineEdit>
#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QShortcut>
#include <QSortFilterProxyModel>

#include "referencedialog.h"
#include "valdialog.h"
#include "reftablemodel.h"

#define  TASK_CODE_COMMENT_COLUMN 2

class TaskCodeDialog : public ReferenceDialog
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

   QFormLayout *taskCodeFormLayout;
   QLineEdit *taskCodeLineEdit;
   QRegularExpression taskCodeRegExp;
   QRegularExpressionValidator *taskCodeValidator;

   QVector< QLineEdit* > taskCodeLineEdits;

   ValDialog *addDialog;
   ValDialog *editDialog;

public:
   explicit TaskCodeDialog(QWidget *parent = nullptr, QTableView *table = nullptr);

   RefTableModel *getModel();

private slots:
   void setEdited(ValDialog *dialog);

signals:
   void dbChanged();

public slots:
   void addAct() override;
   void editAct() override;
   void delAct() override;

};

#endif // TASKCODEDIALOG_H
