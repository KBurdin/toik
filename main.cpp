#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QFileInfo>
#include <QMessageBox>
#include <QStyleFactory>
#include <QStyle>
#include <QPalette>

#include "mainwindow.h"
#include "logindialog.h"
#include "trmessagebox.h"

#define  DB_FILE "C:\\Users\\User\\Documents\\Qt\\TOiK\\toik_db.sqlite"

bool fileExist(const QString);

int main(int argc, char *argv[])
{
    QString db_file;

    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));

    Q_INIT_RESOURCE(toik);    

    app.setWindowIcon(QIcon(":/images/aes1.png"));

    QTranslator translator;
    translator.load(":/Translations/toik_" + QLocale::system().name());
    app.installTranslator(&translator);

    QCoreApplication::setApplicationName("TOiK");
    QCoreApplication::setOrganizationName("AO RASU by Maks Eremin");
    QCoreApplication::setApplicationVersion(SOFT_VERSION);

    if (argc > 1) {
        db_file = argv[1];

    } else {
        db_file = "toik_db.sqlite";
    }

    if (true || (!fileExist(db_file)) ) {
        //db_file = DB_FILE;
        if (!fileExist(db_file)) {
            TrMessageBox::warning(nullptr, QObject::tr("TOiK"), QObject::tr("The database file (toik_db.sqlite) not found!"), QMessageBox::Ok);
            return EXIT_FAILURE;
        }
    }

    DbManager *DB = new DbManager(db_file);

    LoginDialog *loginWin = new LoginDialog;

    loginWin->setWindowFlags(loginWin->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    loginWin->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    loginWin->setMinimumSize(QSize(280, 150));
    loginWin->setMaximumSize(QSize(280, 150));

    for (int i = 0; i < 3; i++) {
        int resultDialog = loginWin->exec();

        if (resultDialog == QDialog::Accepted) {
            if ( loginWin->userValidate() ) {
                //qDebug() << loginWin->getUser().login;

                MainWindow *mainWin = new MainWindow(loginWin->getUser(), DB);

                mainWin->show();

                return app.exec();
            } else {
                loginWin->clearFields();
            }

        } else if (resultDialog == QDialog::Rejected) {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_SUCCESS;
}

bool fileExist(const QString path) {

    QFileInfo checkFile(path);

    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}
