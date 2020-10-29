#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "dbman.h"
#include "userstype.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    bool userValidate();
    void clearFields();

    userType getUser() const;

private:
    QFormLayout *formLayout;
    QHBoxLayout *buttonLayout;
    QLineEdit *userNameLineEdit;
    QLineEdit *userPassLineEdit;
    QLabel *loginLabel;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    userType userData;


signals:

public slots:

};

#endif // LOGINDIALOG_H
