#ifndef SQLITECONNECTIONEDITWIDGET_H
#define SQLITECONNECTIONEDITWIDGET_H

#include "connectioninfo.h"
#include "mainwindow.h"

#include <QWidget>

namespace Ui {
class SqliteConnectionEditWidget;
}

class SqliteConnectionEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SqliteConnectionEditWidget(MainWindow *parent = 0);
    ~SqliteConnectionEditWidget();

    void parseConnectionInfo(ConnectionInfo *connectionInfo);

public slots:
    void showFileDialog();
    void save();

private:
    Ui::SqliteConnectionEditWidget *ui;
    MainWindow* parent = NULL;
    ConnectionInfo* currentConnectionInfo = NULL;

};

#endif // SQLITECONNECTIONEDITWIDGET_H
