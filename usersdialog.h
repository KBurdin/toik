#ifndef USERSDIALOG_H
#define USERSDIALOG_H

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
#include <QComboBox>
#include <QStackedWidget>
#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QShortcut>

#include "userstype.h"
//#include "rtotablemodel.h"
#include "dbman.h"
#include "valdialog.h"

class UsersDialog: public QWidget
{
    Q_OBJECT

public:
    explicit UsersDialog(const userType &user);

    QWidget *getUsersTable();


private:
    QShortcut *deleteShortcut;
    QShortcut *editShortcut;

    ValDialog *addUserWidget;
    ValDialog *changePassWidget;

    //bool editedFlag;

    DbManager *dbman;

    QVector< QLineEdit* > addUserLineEdits;
    QVector< QLineEdit* > changePassLineEdits;

    QString passHash;
    QString passData = "";

    QRegularExpression loginRegExp;
    QValidator *loginValidator;

    QRegularExpression passRegExp;
    QValidator *passValidator;

    QGroupBox *mainGroupBox;

    QVBoxLayout *mainVBoxLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *mainHBoxLayout;
    QHBoxLayout *bottomLayout;

    QTableView *usersTableView;
    QSqlQueryModel *usersModel;

    QPushButton *addUserPushButton;
    QPushButton *editUserPushButton;
    QPushButton *delUserPushButton;
    QPushButton *closePushButton;
    QItemSelectionModel *usersSelection;

    QGroupBox *addUserGroupBox;
    QFormLayout *addUserFormLayout;
    QVBoxLayout *addUserVBoxLayout;
    QHBoxLayout *addUserHBoxLayout;
    QLineEdit *loginAddUserLineEdit;
    //QLineEdit *passAddUserLineEdit;
    QPushButton *changePassPushButton;
    //QLineEdit *rePassAddUserLineEdit;
    QLineEdit *f_nameAddUserLineEdit;
    QLineEdit *m_nameAddUserLineEdit;
    QLineEdit *l_nameAddUserLineEdit;
    QLineEdit *positionAddUserLineEdit;
    QLineEdit *field_1_AddUserLineEdit;
    QLineEdit *field_2_AddUserLineEdit;
    QLineEdit *field_3_AddUserLineEdit;
    QComboBox *levelAddUserComboBox;

    QPushButton *okAddUserPushButton;
    QPushButton *cancelAddUserPushButton;

    QVBoxLayout *mainChangeVBoxLayout;
    QHBoxLayout *bottomChangeHBoxLayout;
    QFormLayout *changeFormLayout;
    QGroupBox *changeGroupBoxLayout;
    QLineEdit *oldPassLineEdit;
    QLineEdit *passLineEdit;
    QLineEdit *rePassLineEdit;
    QPushButton *okChangePassPushButton;
    QPushButton *cancelChangePassPushButton;

    QString getPassHash(QString login, QString pass);

    userType curUser;

signals:
    void dbChanged();

public slots:

private slots:
    void selectionChangedOper();
    void addOper();
    void editOper();
    void delOper();
    void changePass();
    void setEdited(ValDialog *dialog);

};

#endif // USERSDIALOG_H
