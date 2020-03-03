#include "mainwindow.h"

#include <QTranslator>
#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QTranslator translator;
//    if (!translator.load(":/translations/dbManager_" + QLocale::system().name() + ".qph")) {
//    if (!translator.load(QLocale(), QLatin1String("dbManager"), QLatin1String("_"), QLatin1String(":/translations"))) {
      if (!translator.load(":/translations/dbManager_de.qm")) {
        qDebug() << "No Translation File found for " << QLocale::system().name() << " bzw: " << QLocale();
    } else {
        application.installTranslator(&translator);
    }

    MainWindow mainWindow;
    mainWindow.setWindowIcon(QIcon(":/icons/programm_icon.png"));
    mainWindow.show();

    for (int i = 0; i < argc; i++) {
        if ("showReleaseNotes" == QString(argv[i]).trimmed()) {
            mainWindow.enableReleaseNotesButton();
        }
//        qDebug() << "param" << i << " is " << argv[i];
    }

//    if (argc > 2 ) {
//      QString name(argv[1]);
//      int id = atoi(argv[2]);

//      qDebug() << "call func with" << id << " and " << name;
//    }


//    qDebug() << QLocale::system().name();

    return application.exec();
}
