#-------------------------------------------------
#
# Project created by QtCreator 2017-09-20T16:24:01
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TOiK
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    rtodialog.cpp \
    sstsdialog.cpp \
    clocklabel.cpp \
    dbman.cpp \
    maindialog.cpp \
    rpdialog.cpp \
    rkdialog.cpp \
    rtotablemodel.cpp \
    rptablemodel.cpp \
    rktablemodel.cpp \
    logindialog.cpp \
    printdialog.cpp \
    usersdialog.cpp \
    valdialog.cpp \
    taskcodedialog.cpp \
    referencedialog.cpp \
    taskdialog.cpp \
    devicecodedialog.cpp \
    devicedialog.cpp \
    reftablemodel.cpp \
    weekcalc.cpp \
    sqlmodel.cpp \
    rtoeditdialog.cpp \
    rpeditdialog.cpp \
    rkeditdialog.cpp \
    multicolumnsortfilterproxymodel.cpp \
    trmessagebox.cpp

HEADERS += \
        mainwindow.h \
    rtodialog.h \
    sstsdialog.h \
    clocklabel.h \
    dbman.h \
    maindialog.h \
    rpdialog.h \
    rkdialog.h \
    rtotablemodel.h \
    rptablemodel.h \
    rktablemodel.h \
    logindialog.h \
    printdialog.h \
    usersdialog.h \
    userstype.h \
    valdialog.h \
    taskcodedialog.h \
    referencedialog.h \
    taskdialog.h \
    devicecodedialog.h \
    devicedialog.h \
    reftablemodel.h \
    weekcalc.h \
    sqlmodel.h \
    rtoeditdialog.h \
    rpeditdialog.h \
    rkeditdialog.h \
    multicolumnsortfilterproxymodel.h \
    trmessagebox.h

RESOURCES += \
    toik.qrc

TRANSLATIONS += \
    Translations/toik_ru.ts

tr.commands = lupdate \"$$_PRO_FILE_\" && lrelease \"$$_PRO_FILE_\"
    PRE_TARGETDEPS += tr
    QMAKE_EXTRA_TARGETS += tr
