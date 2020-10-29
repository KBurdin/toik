#ifndef RTOEDITDIALOG_H
#define RTOEDITDIALOG_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QAbstractItemModel>
#include <QLineEdit>
#include <QComboBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVector>
#include <QTextEdit>
#include <QLabel>
#include <QWidget>

#include "valdialog.h"
#include "referencedialog.h"
#include "reftablemodel.h"
#include "rtotablemodel.h"
#include "sqlmodel.h"
#include "weekcalc.h"

#define  TASK_COMMENT_COLUMN 8

class RTOEditDialog : public ValDialog
{
    Q_OBJECT

public:
    explicit RTOEditDialog(QTableView *table, QAbstractItemModel *model, QWidget *parent = nullptr);

    QAbstractItemModel *getModel();
    SqlModel *getTaskModel();
    SqlModel *getDeviceModel();

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
    QAbstractItemModel *mainModel;

    ValDialog *taskEditDialog;

    QVector< QLineEdit* > taskEditLineEdits;

    SqlModel *periodModel;
    SqlModel *taskModel;
    SqlModel *deviceModel;
    QFormLayout *taskEditFormLayout;
    QComboBox *periodComboBox;
    QComboBox *taskComboBox;
    QComboBox *deviceComboBox;
    QComboBox *statusComboBox;
    QTextEdit *commentTextEdit;

    QHBoxLayout *periodHBoxLayout;
    QHBoxLayout *taskHBoxLayout;
    QHBoxLayout *deviceHBoxLayout;
    QHBoxLayout *statusHBoxLayout;
    QLineEdit *periodLineEdit;
    QLineEdit *taskLineEdit;
    QLineEdit *deviceLineEdit;
    QLabel *statusLabel;
    QWidget *parentWidget;

private slots:
   void setEdited(ValDialog *dialog);
   void taskModelChanged();
   void taskModelUpdate();

signals:
   void dbChanged();

public slots:
   void addAct();
   void editAct();
   void delAct();
   void viewAct();

};

#endif // RTOEDITDIALOG_H
