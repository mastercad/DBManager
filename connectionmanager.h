#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QStandardItemModel>
#include <QDialog>
#include <QString>
#include <QWidget>
#include <QMap>

#include "mainwindow.h"
#include "connections.h"
#include "connectioninfo.h"

namespace Ui {
    class ConnectionManager;
}

class ConnectionManager : public QDialog
{
    Q_OBJECT

public:
//    explicit ConnectionManager(MainWindow *parent = 0, QMap<QString, QMap<QString, ConnectionInfo* > >* connections = nullptr);
    explicit ConnectionManager(MainWindow *parent = 0, Connections* connections = nullptr);
    ~ConnectionManager();

public slots:
    void onSaveConnectionsClicked();
    void loadConnections();
    void onConnectionDoubleClicked(const QModelIndex &index);

private:
    void showConnectionEdit(QStandardItem* currentItem);
    void showConnectionEditForMysql();
    void showConnectionEditForSqlite();

    QStandardItemModel* model = NULL;
    ConnectionInfo* currentConnectionInfo = NULL;
    QWidget* lastEditWidget = NULL;
    Ui::ConnectionManager *ui;
    MainWindow* parent;
//    QMap<QString, QMap<QString, ConnectionInfo *> >* connections = NULL;
    Connections* connections = NULL;
};

#endif // CONNECTIONMANAGER_H
