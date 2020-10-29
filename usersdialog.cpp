#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QKeySequence>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "usersdialog.h"
#include "maindialog.h"
#include "valdialog.h"
#include "trmessagebox.h"

UsersDialog::UsersDialog(const userType &user):
    addUserWidget(new ValDialog),
    changePassWidget(new ValDialog(ValDialog::NoConfirmSave)),
    loginValidator(new QRegularExpressionValidator(loginRegExp, this)),
    mainGroupBox(new QGroupBox(tr("User accounts"))),
    mainVBoxLayout(new QVBoxLayout),
    rightLayout(new QVBoxLayout),
    mainHBoxLayout(new QHBoxLayout),
    bottomLayout(new QHBoxLayout),
    usersTableView(new QTableView),
    usersModel(new QSqlQueryModel),
    addUserPushButton(new QPushButton(tr("Add"))),
    editUserPushButton(new QPushButton(tr("Edit"))),
    delUserPushButton(new QPushButton(tr("Delete"))),
    closePushButton(new QPushButton(tr("Close"))),

    addUserGroupBox(new QGroupBox(tr("Add user"))),
    addUserFormLayout(new QFormLayout),
    addUserVBoxLayout(new QVBoxLayout),
    addUserHBoxLayout(new QHBoxLayout),
    loginAddUserLineEdit(new QLineEdit),
    changePassPushButton(new QPushButton(tr("Change"))),
    f_nameAddUserLineEdit(new QLineEdit),
    m_nameAddUserLineEdit(new QLineEdit),
    l_nameAddUserLineEdit(new QLineEdit),
    positionAddUserLineEdit(new QLineEdit),
    field_1_AddUserLineEdit(new QLineEdit),
    field_2_AddUserLineEdit(new QLineEdit),
    field_3_AddUserLineEdit(new QLineEdit),
    levelAddUserComboBox(new QComboBox),
    okAddUserPushButton(new QPushButton(tr("OK"))),
    cancelAddUserPushButton(new QPushButton(tr("Cancel"))),
    mainChangeVBoxLayout(new QVBoxLayout),
    bottomChangeHBoxLayout(new QHBoxLayout),
    changeFormLayout(new QFormLayout),
    changeGroupBoxLayout(new QGroupBox(tr("Change password"))),
    oldPassLineEdit(new QLineEdit),
    passLineEdit(new QLineEdit),
    rePassLineEdit(new QLineEdit),
    okChangePassPushButton(new QPushButton(tr("OK"))),
    cancelChangePassPushButton(new QPushButton(tr("Cancel"))),
    curUser(user)

{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(850, 400));
    //setMaximumSize(QSize(850, 400));

    loginRegExp = QRegularExpression("(^[a-z0-9_-]{4,16}$)");

    editUserPushButton->setDisabled(true);
    delUserPushButton->setDisabled(true);

    deleteShortcut = new QShortcut(QKeySequence(QKeySequence::Delete), usersTableView);
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(delOper()));

    loginAddUserLineEdit->setPlaceholderText(tr("4-16 lowercase characters"));
    loginAddUserLineEdit->setValidator(loginValidator);
    addUserLineEdits << loginAddUserLineEdit;

    levelAddUserComboBox->addItem(tr("User"), (1 << 6));
    levelAddUserComboBox->addItem(tr("Admin"), (1 << 7));

    connect(changePassPushButton, SIGNAL(clicked(bool)), this, SLOT(changePass()));

    loginAddUserLineEdit->setFixedWidth(150);
    levelAddUserComboBox->setFixedWidth(150);
    changePassPushButton->setFixedWidth(150);
    f_nameAddUserLineEdit->setFixedWidth(240);
    m_nameAddUserLineEdit->setFixedWidth(240);
    l_nameAddUserLineEdit->setFixedWidth(240);

    addUserFormLayout->addRow(tr("Login:"), loginAddUserLineEdit);
    addUserFormLayout->addRow(tr("Level:"), levelAddUserComboBox);
    addUserFormLayout->addRow(tr("Password"), changePassPushButton);
    addUserFormLayout->addRow(tr("Name:"), f_nameAddUserLineEdit);
    addUserFormLayout->addRow(tr("Middle name:"), m_nameAddUserLineEdit);
    addUserFormLayout->addRow(tr("Surname:"), l_nameAddUserLineEdit);
    addUserFormLayout->addRow(tr("Position:"), positionAddUserLineEdit);
    addUserFormLayout->addRow(tr("Field 1:"), field_1_AddUserLineEdit);
    addUserFormLayout->addRow(tr("Field 2:"), field_2_AddUserLineEdit);
    addUserFormLayout->addRow(tr("Field 3:"), field_3_AddUserLineEdit);

    addUserWidget->setMainLayout(addUserFormLayout);

    addUserWidget->setValidatorFields(addUserLineEdits);

    connect(loginAddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );
    connect(levelAddUserComboBox, &QComboBox::currentTextChanged, [this](){this->setEdited(addUserWidget);} );
    connect(f_nameAddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );
    connect(m_nameAddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );
    connect(l_nameAddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );
    connect(positionAddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );
    connect(field_1_AddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );
    connect(field_2_AddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );
    connect(field_3_AddUserLineEdit, &QLineEdit::textChanged, [this](){this->setEdited(addUserWidget);} );

    addUserWidget->setWindowFlags((addUserWidget->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    addUserWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    addUserWidget->setMinimumSize(QSize(500, 400));
    addUserWidget->setMaximumSize(QSize(500, 400));   

    passRegExp = QRegularExpression("^(?=\\S+$).{1,}$");
    passValidator = new QRegularExpressionValidator(passRegExp, this);

    oldPassLineEdit->setEchoMode(QLineEdit::Password);    
    passLineEdit->setEchoMode(QLineEdit::Password);    
    rePassLineEdit->setEchoMode(QLineEdit::Password);

    passLineEdit->setValidator(passValidator);
    changePassLineEdits << passLineEdit;

    changeFormLayout->addRow(tr("Old password:"), oldPassLineEdit);
    changeFormLayout->addRow(tr("New password:"), passLineEdit);
    changeFormLayout->addRow(tr("Retype password:"), rePassLineEdit);

    changePassWidget->setMainLayout(changeFormLayout);

    changePassWidget->setValidatorFields(changePassLineEdits);

    connect(oldPassLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(changePassWidget);} );
    connect(passLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(changePassWidget);} );
    connect(rePassLineEdit, &QLineEdit::textChanged, [=](){this->setEdited(changePassWidget);} );

    changePassWidget->setWindowFlags((addUserWidget->windowFlags() & ~Qt::WindowContextHelpButtonHint));
    changePassWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    changePassWidget->setMinimumSize(QSize(300, 180));
    changePassWidget->setMaximumSize(QSize(300, 180));
    connect(okChangePassPushButton, SIGNAL(clicked(bool)), changePassWidget, SLOT(accept()));
    connect(cancelChangePassPushButton, SIGNAL(clicked(bool)), changePassWidget, SLOT(reject()));

    mainHBoxLayout->addWidget(usersTableView);

    rightLayout->addWidget(addUserPushButton);
    rightLayout->addWidget(editUserPushButton);
    rightLayout->addWidget(delUserPushButton);
    rightLayout->addStretch();
    mainHBoxLayout->addLayout(rightLayout);
    mainGroupBox->setLayout(mainHBoxLayout);
    mainVBoxLayout->addWidget(mainGroupBox);
    bottomLayout->addStretch();
    bottomLayout->addWidget(closePushButton);
    bottomLayout->addStretch();
    mainVBoxLayout->addLayout(bottomLayout);

    this->setLayout(mainVBoxLayout);

    usersModel->setQuery("SELECT id, login, CASE WHEN (level & 128) = 128 THEN \"Admin\" ELSE \"User\" END AS role, " \
                         "level, pass, f_name, m_name, l_name, position, field_1, field_2, field_3 FROM users");

    usersTableView->setModel(usersModel);
    usersTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Fixed);
    usersTableView->horizontalHeader()->setSectionResizeMode(11, QHeaderView::Fixed);
    usersModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    usersModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Login"));
    usersModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Role"));
    usersModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Level"));
    usersModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Password"));
    usersModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Name"));
    usersModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Middle name"));
    usersModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Surname"));
    usersModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Position"));
    usersModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Field 1"));
    usersModel->setHeaderData(10, Qt::Horizontal, QObject::tr("Field 2"));
    usersModel->setHeaderData(11, Qt::Horizontal, QObject::tr("Field 3"));
    usersTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    usersTableView->setColumnHidden(0, true);
    usersTableView->setColumnHidden(3, true);
    usersTableView->setColumnHidden(4, true);
    usersTableView->setColumnHidden(9, true);
    usersTableView->setColumnHidden(10, true);
    usersTableView->setColumnHidden(11, true);

    usersTableView->verticalHeader()->setVisible(false);
    usersTableView->horizontalHeader()->setStretchLastSection(true);
    usersTableView->horizontalHeader()->resizeSection(1, 100); // Login
    usersTableView->horizontalHeader()->resizeSection(2, 90); // Role
    usersTableView->horizontalHeader()->resizeSection(5, 120); // Name
    usersTableView->horizontalHeader()->resizeSection(6, 120); // Middle name
    usersTableView->horizontalHeader()->resizeSection(7, 120); // Surname
    usersTableView->horizontalHeader()->resizeSection(8, 60); // Position

    usersTableView->sortByColumn(0, Qt::AscendingOrder); // id

    usersTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTableView->verticalHeader()->setDefaultSectionSize(22);

    usersSelection = usersTableView->selectionModel();

    connect(addUserPushButton, SIGNAL(clicked(bool)), this, SLOT(addOper()));
    connect(editUserPushButton, SIGNAL(clicked(bool)), this, SLOT(editOper()));
    connect(usersTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editOper()));
    connect(delUserPushButton, SIGNAL(clicked(bool)), this, SLOT(delOper()));
    connect(closePushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(usersSelection, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChangedOper()));
}

QWidget *UsersDialog::getUsersTable()
{
    return usersTableView;
}

QString UsersDialog::getPassHash(QString login, QString pass)
{
    QByteArray passHash;
    passHash = QString(pass + login + "@h81kE").toLocal8Bit();
    return QString(QCryptographicHash::hash(passHash, QCryptographicHash::Md5).toHex());
}

void UsersDialog::selectionChangedOper()
{
    if (usersSelection->hasSelection()) {
        //qDebug() << "has selection";
        editUserPushButton->setEnabled(true);
        delUserPushButton->setEnabled(true);
    } else {
        //qDebug() << "hasn't selection";
        editUserPushButton->setDisabled(true);
        delUserPushButton->setDisabled(true);
    }
}

void UsersDialog::addOper()
{
    int result;
    QSqlQuery query;
    QString sql_query;
    QModelIndex curIdx = usersTableView->selectionModel()->currentIndex();

    if ( !(curUser.level & 0x80))  {
        TrMessageBox::warning(this, tr("Attention"), tr("Your have no accsess to add new users"));
        return;
    }

    //oldPassLineEdit->setDisabled(true);

    changeFormLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setHidden(true);
    changeFormLayout->itemAt(0, QFormLayout::FieldRole)->widget()->setHidden(true);

    loginAddUserLineEdit->setText("");
    levelAddUserComboBox->setCurrentIndex(0);
    f_nameAddUserLineEdit->setText("");
    m_nameAddUserLineEdit->setText("");
    l_nameAddUserLineEdit->setText("");
    positionAddUserLineEdit->setText("");
    field_1_AddUserLineEdit->setText("");
    field_2_AddUserLineEdit->setText("");
    field_3_AddUserLineEdit->setText("");

    addUserWidget->setWindowTitle(tr("TOiK"));

    loginAddUserLineEdit->setFocus();
    result = addUserWidget->exec();
    //oldPassLineEdit->setEnabled(true);

    changeFormLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setVisible(true);
    changeFormLayout->itemAt(0, QFormLayout::FieldRole)->widget()->setVisible(true);

    if (result == QDialog::Accepted && addUserWidget->isEdited()) {
        if (passLineEdit->text() == "") {
            TrMessageBox::warning(this, tr("Attention"), tr("You have not typed password\nWill set default: 1"));
            passLineEdit->setText("1");
        }
        sql_query = "SELECT COUNT(id) FROM users WHERE login LIKE \"" + loginAddUserLineEdit->text() + "\"";
        //qDebug() << sql_query;
        if (!query.exec(sql_query)) {
            //qDebug() << sql_query;
            qDebug() << query.lastError().text();
        } else {
            while (query.next()) {
                result = query.value(0).toInt();
                //qDebug() << result;
            }
            if (result == 0) {
                sql_query = "INSERT INTO users (login, pass, f_name, m_name, l_name, position, field_1, field_2, field_3, level) VALUES (\"" + \
                    loginAddUserLineEdit->text() + "\", \"" + \
                    getPassHash(loginAddUserLineEdit->text(), passLineEdit->text()) + "\", \"" + \
                    f_nameAddUserLineEdit->text() + "\", \"" + \
                    m_nameAddUserLineEdit->text() + "\", \"" + \
                    l_nameAddUserLineEdit->text() + "\", \"" + \
                    positionAddUserLineEdit->text() + "\", \"" + \
                    field_1_AddUserLineEdit->text() + "\", \"" + \
                    field_2_AddUserLineEdit->text() + "\", \"" + \
                    field_3_AddUserLineEdit->text() + "\", " + \
                    levelAddUserComboBox->itemData(levelAddUserComboBox->currentIndex()).toString() + ")";
                //qDebug() << sql_query;
                result = query.exec(sql_query);

                if (!result) {
                    qDebug() << query.lastError().text();
                } else {
                    //qDebug() << "restart model";
                    usersModel->setQuery(usersModel->query().lastQuery());
                    passLineEdit->setText("");
                    //usersTableView->selectionModel()->setCurrentIndex(curIdx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
                }

            } else { // if (result == 0)
                TrMessageBox::warning(this, tr("Attention"), tr("User ") + loginAddUserLineEdit->text() + tr(" already exist"));
            }
        }
    } // if (result == QDialog::Accepted)

    if (addUserWidget->isEdited()) {
        addUserWidget->setWindowTitle(tr("TOiK"));
    }
    //usersTableView->setFocus();
}

void UsersDialog::editOper()
{
    int rowidx = usersTableView->selectionModel()->currentIndex().row();
    int result;
    QModelIndex curIdx = usersTableView->selectionModel()->currentIndex();
    QSqlQuery query;
    QString sql_query;

    if (!usersTableView->selectionModel()->hasSelection()) {
        return;
    }

    if ( !(curUser.level & 0x80) && (curUser.id != usersModel->index(rowidx, 0).data().toInt()) ) {
        TrMessageBox::warning(this, tr("Attention"), tr("Your have no accsess to change this user"));
        return;
    }

    levelAddUserComboBox->setCurrentIndex((usersModel->index(rowidx, 3).data().toInt() & 128) ? 1 : 0);
    loginAddUserLineEdit->setText(usersModel->index(rowidx, 1).data().toString());
    //passAddUserLineEdit->setText("");
    //rePassAddUserLineEdit->setText("");
    passHash = usersModel->index(rowidx, 4).data().toString();
    f_nameAddUserLineEdit->setText(usersModel->index(rowidx, 5).data().toString());
    m_nameAddUserLineEdit->setText(usersModel->index(rowidx, 6).data().toString());
    l_nameAddUserLineEdit->setText(usersModel->index(rowidx, 7).data().toString());
    positionAddUserLineEdit->setText(usersModel->index(rowidx, 8).data().toString());
    field_1_AddUserLineEdit->setText(usersModel->index(rowidx, 9).data().toString());
    field_2_AddUserLineEdit->setText(usersModel->index(rowidx, 10).data().toString());
    field_3_AddUserLineEdit->setText(usersModel->index(rowidx, 11).data().toString());

    addUserWidget->setWindowTitle(tr("TOiK"));
    result = addUserWidget->exec();

    if ((result == QDialog::Accepted) && addUserWidget->isEdited()) {
        sql_query = "UPDATE users SET login = \"" + loginAddUserLineEdit->text() + "\", " + \
                "f_name = \"" + f_nameAddUserLineEdit->text() + "\", " + \
                "m_name = \"" + m_nameAddUserLineEdit->text() + "\", " + \
                "l_name = \"" + l_nameAddUserLineEdit->text() + "\", ";
        if (changePassWidget->isEdited()) {
            sql_query += "pass = \"" + getPassHash(loginAddUserLineEdit->text(), passLineEdit->text()) + "\", ";
        }
        sql_query += "position = \"" + positionAddUserLineEdit->text() + "\", " + \
                     "field_1 = \"" + field_1_AddUserLineEdit->text() + "\", " + \
                     "field_2 = \"" + field_2_AddUserLineEdit->text() + "\", " + \
                     "field_3 = \"" + field_3_AddUserLineEdit->text() + "\", " + \
                     "level = " + levelAddUserComboBox->itemData(levelAddUserComboBox->currentIndex()).toString() + \
                     " WHERE id = " + usersModel->index(rowidx, 0).data().toString();
        //qDebug() << sql_query;
        result = query.exec(sql_query);
        if (!result) {
            qDebug() << query.lastError().text();
        } else {
            usersModel->setQuery(usersModel->query().lastQuery());
            usersTableView->selectionModel()->setCurrentIndex(curIdx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
    }

    if (addUserWidget->isEdited()) {
        addUserWidget->setWindowTitle(tr("TOiK"));
    }
    //usersTableView->setFocus();
}

void UsersDialog::delOper()
{
    int rowidx = usersTableView->selectionModel()->currentIndex().row();
    int result;
    QSqlQuery query;
    QString sql_query;

    if (!usersTableView->selectionModel()->hasSelection()) {
        return;
    }

    if ( usersModel->index(rowidx, 1).data().toString() == "root")  {
        TrMessageBox::warning(this, tr("Attention"), tr("Cannot to delete root superuser"));
        return;
    }

    if ( !(curUser.level & 0x80))  {
        TrMessageBox::warning(this, tr("Attention"), tr("Your have no accsess to delete users"));
        return;
    }

    if (usersModel->index(rowidx, 1).data().toString() == curUser.login) {
        TrMessageBox::warning(this, tr("Attention"), tr("Cannot delete current user"));
    } else {
        const TrMessageBox::StandardButton ret = QMessageBox::warning(this, tr("TOiK"),
                                                                     tr("Do you realy want to delete user ") + usersModel->index(rowidx, 1).data().toString() + "?",
                                                                     QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);
        if (ret == QMessageBox::Yes) {
            sql_query = "DELETE FROM users WHERE id = " + usersModel->index(rowidx, 0).data().toString();
            //qDebug() << sql_query;
            result = query.exec(sql_query);
            if (!result) {
                qDebug() << query.lastError().text();
            } else {
                usersModel->setQuery(usersModel->query().lastQuery());
                if (usersModel->rowCount() == 0) {
                    editUserPushButton->setDisabled(true);
                    delUserPushButton->setDisabled(true);
                }
            }
        }
    }
    usersTableView->setFocus();
}

void UsersDialog::changePass()
{
    int result;

    oldPassLineEdit->setText("");

    if (!oldPassLineEdit->isHidden()) {
        oldPassLineEdit->setFocus();

    } else {
        passLineEdit->setFocus();
    }

    passLineEdit->setText("");
    rePassLineEdit->setText("");
    changePassWidget->setWindowTitle(tr("TOiK"));
    result = changePassWidget->exec();

    if ((result == QDialog::Accepted) && changePassWidget->isEdited()) {
        if ( (passLineEdit->text() == rePassLineEdit->text()) && \
            (!oldPassLineEdit->isHidden() ? (getPassHash(loginAddUserLineEdit->text(), oldPassLineEdit->text()) == passHash) : true) ) {
            addUserWidget->setEdited();
            addUserWidget->setWindowTitle(tr("TOiK") + "*");
            passData = passLineEdit->text();
        } else {
            TrMessageBox::warning(this, tr("Attention"), tr("Your new password and confirmation\n" \
                                                           "password does not match. Please confirm and try again."));
            passLineEdit->setText("");
            rePassLineEdit->setText("");
        }
    }

    usersTableView->setFocus();
}

void UsersDialog::setEdited(ValDialog *dialog)
{
    dialog->setEdited();
    dialog->setWindowTitle(tr("TOiK") + "*");
}
