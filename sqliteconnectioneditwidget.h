#ifndef SQLITECONNECTIONEDITWIDGET_H
#define SQLITECONNECTIONEDITWIDGET_H

#include "connectioninfo.h"
#include "connections.h"
#include "connectionmanager.h"
#include "mainwindow.h"

#include <QWidget>

namespace Ui {
class SqliteConnectionEditWidget;
}

class SqliteConnectionEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SqliteConnectionEditWidget(ConnectionManager *parent = nullptr, Connections* connections = nullptr);
    ~SqliteConnectionEditWidget();

    void parseConnectionInfo(ConnectionInfo *connectionInfo);

public slots:
    void showFileDialog();
    void save();

private:
    Ui::SqliteConnectionEditWidget *ui;
    ConnectionManager* parent = nullptr;
    ConnectionInfo* currentConnectionInfo = nullptr;
    Connections* connections = nullptr;

};

#endif // SQLITECONNECTIONEDITWIDGET_H
