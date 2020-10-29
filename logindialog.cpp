#include <QDebug>
#include <QCryptographicHash>

#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent),
    formLayout(new QFormLayout()),
    buttonLayout(new QHBoxLayout),
    userNameLineEdit(new QLineEdit),
    userPassLineEdit(new QLineEdit),
    loginLabel(new QLabel(tr("Authorization"))),
    okBtn(new QPushButton(tr("OK"))),
    cancelBtn(new QPushButton(tr("Cancel")))

{

    userPassLineEdit->setEchoMode(QLineEdit::Password);

    formLayout->addWidget(loginLabel);
    formLayout->setSpacing(10);
    formLayout->addRow(tr("Login:"), userNameLineEdit);
    formLayout->addRow(tr("Password:"), userPassLineEdit);

    connect(okBtn, &QPushButton::clicked, this, &LoginDialog::accept);
    buttonLayout->addWidget(okBtn);

    connect(cancelBtn, &QPushButton::clicked, this, &LoginDialog::reject);
    buttonLayout->addWidget( cancelBtn );

    formLayout->addItem(buttonLayout);
    setLayout( formLayout );
}

bool LoginDialog::userValidate()
{
    QSqlQuery query;
    QByteArray passData;

    if (!query.exec("SELECT * FROM users")) {
        qDebug() << query.lastError().text();
    } else {
        while (query.next()) {
            if (query.value(1).toString() == userNameLineEdit->text()) {
                passData = (userPassLineEdit->text() + query.value(1).toString() + "@h81kE").toLocal8Bit();
                QString passMd5 = QString(QCryptographicHash::hash(passData, QCryptographicHash::Md5).toHex());
                //qDebug() << passMd5;
                if (passMd5 == query.value(2).toString()) {
                    userData.id = query.value(0).toInt();
                    userData.login = query.value(1).toString();
                    userData.pass = query.value(2).toString();
                    userData.f_name = query.value(3).toString();
                    userData.m_name = query.value(4).toString();
                    userData.l_name = query.value(5).toString();
                    userData.position = query.value(6).toString();
                    userData.field_1 = query.value(7).toString();
                    userData.field_2 = query.value(8).toString();
                    userData.field_3 = query.value(9).toString();
                    userData.level = query.value(10).toInt();
                    return true;
                }
            }
        }
    }

    return false;
}

void LoginDialog::clearFields()
{
    userPassLineEdit->setText("");
}

userType LoginDialog::getUser() const
{
    return userData;
}
