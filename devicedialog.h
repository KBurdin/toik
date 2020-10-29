#ifndef DEVICEDIALOG_H
#define DEVICEDIALOG_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>
#include <QComboBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVector>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "referencedialog.h"
#include "reftablemodel.h"
#include "valdialog.h"
#include "sqlmodel.h"

#define  DEVICE_COMMENT_COLUMN 7

class DeviceDialog: public ReferenceDialog
{
    Q_OBJECT

private:
   QTableView *mainTableView;   
   RefTableModel *mainModel;
   QSortFilterProxyModel *proxyModel;
   ValDialog *deviceDialog;
   QVector< QLineEdit* > deviceLineEdits;

   QRegularExpression kksRegExp;
   QRegularExpressionValidator *kksValidator;

   SqlModel *deviceCodeModel;
   QFormLayout *deviceFormLayout;
   QLineEdit *kksLineEdit;
   QComboBox *deviceNameComboBox;
   QLineEdit *serialLineEdit;
   QCheckBox *ismsrCheckBox;
   QLabel *ismsrLabel;
   QComboBox *mntTypeComboBox;
   QTextEdit *commentTextEdit;
    QLabel *ismsrRowNameLabel;
    QHBoxLayout *deviceNameHBoxLayout;
    QHBoxLayout *ismsrHBoxLayout;
    QHBoxLayout * mntTypeHBLayout;
    QLineEdit *deviceNameLineEdit;
    QLineEdit *mntTypeLineEdit;

public:
   explicit DeviceDialog(QWidget *parent = nullptr, QTableView *table = nullptr);

   RefTableModel *getModel();
   SqlModel *getDeviceModel();

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

#endif // DEVICEDIALOG_H
