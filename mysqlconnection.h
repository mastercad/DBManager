#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H

#include "connection.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>

class MysqlConnection : public Connection
{
private:
    bool switchDatabase(QString databaseName);
    void handleTableClicked(QStandardItem* item);
    void handleDatabaseClicked(QModelIndex index);

    QString host;
    QString user;
    QString password;
    uint port;
    QString databaseName;

    QString activeTableName;
    QString activeDatabaseName;

    QStandardItemModel* databaseCollection = NULL;

public slots:
    void onListViewDoubleClicked(const QModelIndex index);

public:
    MysqlConnection();
    void init();

    void setHost(QString host);
    QString getHost() const;

    void setUser(QString user);
    QString getUser() const;

    void setPassword(QString password);
    QString getPassword() const;

    void setPort(uint port);
    uint getPort() const;

    void setDatabaseName(QString databaseName);
    QString getDatabaseName() const;

    void collectTableInformations();
};

#endif // MYSQLCONNECTION_H
