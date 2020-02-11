#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug() << "create Application!";
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    qDebug() << "Start Application!";
    return application.exec();
}
