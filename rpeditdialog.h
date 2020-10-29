#ifndef RPEDITDIALOG_H
#define RPEDITDIALOG_H

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

class RPEditDialog : public ValDialog
{
    Q_OBJECT

public:
    explicit RPEditDialog(QTableView *table, QAbstractItemModel *model, QWidget *parent = nullptr);

    QAbstractItemModel *getModel();
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
    SqlModel *deviceModel;
    QFormLayout *taskEditFormLayout;
    QComboBox *periodComboBox;
    QComboBox *deviceComboBox;
    QLineEdit *partNumberLineEdit;
    QLineEdit *serialLineEdit;
    QLineEdit *manHourLineEdit;
    QLineEdit *planPeriodLineEdit;
    QComboBox *statusComboBox;
    QLineEdit *reportLineEdit;
    QTextEdit *commentTextEdit;

    QHBoxLayout *periodHBoxLayout;
    QHBoxLayout *deviceHBoxLayout;
    QHBoxLayout *statusHBoxLayout;
    QLineEdit *periodLineEdit;
    QLineEdit *deviceLineEdit;
    QLabel *statusLabel;
    QWidget *parentWidget;

    QString getDeviceModelVal(int rowNum) const;

private slots:
    void setEdited(ValDialog *dialog);
    void deviceUpdated();

signals:
    void dbChanged();

public slots:
    void addAct();
    void editAct();
    void delAct();
    void viewAct();

};

#endif // RPEDITDIALOG_H
