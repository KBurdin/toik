#ifndef DEVICECODEDIALOG_H
#define DEVICECODEDIALOG_H

#include <QHeaderView>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVector>
#include <QTextEdit>
#include <QLineEdit>
#include <QSortFilterProxyModel>

#include "referencedialog.h"
#include "reftablemodel.h"
#include "valdialog.h"

#define  DEVICE_CODE_COMMENT_COLUMN 8

class DeviceCodeDialog: public ReferenceDialog
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

   ValDialog *deviceCodeDialog;

   QVector< QLineEdit* > deviceCodeLineEdits;

   QRegularExpression periodRegExp;
   QRegularExpressionValidator *periodValidator;

   QRegularExpression amountRegExp;
   QRegularExpressionValidator *amountValidator;

   QFormLayout *deviceCodeFormLayout;
   QLineEdit *partNumLineEdit;
   QLineEdit *nameLineEdit;
   QLineEdit *mnfLineEdit;
   QLineEdit *povPeriodLineEdit;
   QLineEdit *calPeriodLineEdit;
   QLineEdit *povAmountLineEdit;
   QLineEdit *calAmountLineEdit;
   QTextEdit *commentTextEdit;

public:
   explicit DeviceCodeDialog(QWidget *parent = nullptr, QTableView *table = nullptr);

   RefTableModel *getModel();

private slots:
   void setEdited(ValDialog *dialog);

signals:
   void dbChanged();

public slots:
   void addAct();
   void editAct();
   void delAct();
   void viewAct();

};

#endif // DEVICECODEDIALOG_H
