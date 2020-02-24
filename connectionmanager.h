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
//    explicit ConnectionManager(MainWindow *parent = nullptr, QMap<QString, QMap<QString, ConnectionInfo* > >* connections = nullptr);
    explicit ConnectionManager(MainWindow *parent = nullptr, Connections* connections = nullptr);
    ~ConnectionManager();

public slots:
    void onSaveConnectionsClicked();
    void loadConnections();
    void onConnectionDoubleClicked(const QModelIndex &index);
    void onValidationState(bool);

signals:
    void invalid(bool);

private:
    void showConnectionEdit(QStandardItem* currentItem);
    void showConnectionEditForMysql();
    void showConnectionEditForSqlite();

    QStandardItemModel* model = nullptr;
    ConnectionInfo* currentConnectionInfo = nullptr;
    QWidget* lastEditWidget = nullptr;
    Ui::ConnectionManager *ui;
    MainWindow* parent;
//    QMap<QString, QMap<QString, ConnectionInfo *> >* connections = nullptr;
    Connections* connections = nullptr;
};

#endif // CONNECTIONMANAGER_H
