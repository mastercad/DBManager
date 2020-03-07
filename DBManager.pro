#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T19:07:25
#
#-------------------------------------------------

QT       += core gui sql network xml

#CONFIG -= import_plugins
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBManager
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

# LIBS    += -lGL
LIBS    += -L/usr/local/lib -L/usr/lib -L"D:\Programme\Qt\5.12.6\mingw73_64\lib" -L"C:\Program Files\MySQL\MySQL Connector C++ 1.0.5/lib/opt" -lmysqlcppconn
#LIBS    += -L/media/Austausch/Qt/5.15_static/lib -lmysqlcppconn

INCLUDEPATH += -I/usr/include -I/usr/local/include -I/usr/local/include/cppconn -I"D:\Programme\Qt\5.12.6\mingw73_64\include" -I"C:\Program Files\MySQL\MySQL Connector C++ 1.0.5\include"
#INCLUDEPATH += -I/media/Austausch/Qt/5.15_static/include -I/usr/local/include/cppconn

QT_PLUGIN_PATH=`pwd`

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    textedit.cpp \
    connectionfactory.cpp \
    newconnectionwizard.cpp \
    connectionwizardintropage.cpp \
    connectionwizardmysqlpage.cpp \
    connectionwizardsqlitepage.cpp \
    connection.cpp \
    mysqlconnection.cpp \
    sqliteconnection.cpp \
    connectioninfo.cpp \
    mysqlconnectioninfo.cpp \
    sqliteconnectioninfo.cpp \
    connectioninfofactory.cpp \
    connectionmanager.cpp \
    mysqlconnectioneditwidget.cpp \
    sqliteconnectioneditwidget.cpp \
    connections.cpp \
    mysqlconnectionvalidator.cpp \
    sqliteconnectionvalidator.cpp \
    updatemanager.cpp \
    filedownloader.cpp \
    defaults.cpp \
    sidebar.cpp \
    releasenote.cpp \
    releasenotesmanager.cpp \
    releasenotesparser.cpp \
    releasenotesdownloader.cpp

HEADERS += \
    mainwindow.h \
    textedit.h \
    connectionfactory.h \
    newconnectionwizard.h \
    connectionwizardintropage.h \
    connectionwizardmysqlpage.h \
    connectionwizardsqlitepage.h \
    connection.h \
    mysqlconnection.h \
    sqliteconnection.h \
    connectioninfo.h \
    mysqlconnectioninfo.h \
    sqliteconnectioninfo.h \
    connectioninfofactory.h \
    connectionmanager.h \
    mysqlconnectioneditwidget.h \
    sqliteconnectioneditwidget.h \
    connections.h \
    mysqlconnectionvalidator.h \
    sqliteconnectionvalidator.h \
    defaults.h \
    updatemanager.h \
    filedownloader.h \
    sidebar.h \
    releasenote.h \
    releasenotesmanager.h \
    releasenotesparser.h \
    releasenotesdownloader.h

FORMS += \
    mainwindow.ui \
    connectionmanager.ui \
    mysqlconnectioneditwidget.ui \
    sqliteconnectioneditwidget.ui

DISTFILES += \
    README.md \
    TODO.md \
    translations/dbManager_de_DE.qph \
    update.xml \
    dependencyResolver.bash \
    DBManager/DBManager.sh \
    releaseNotes.xml

RESOURCES += \
    resources.qrc

TRANSLATIONS += translations/dbManager_de.ts \
    translations/dbManager_en.ts

CODECFORSRC     = UTF-8
