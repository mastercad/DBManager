#ifndef SQLITECONNECTION_H
#define SQLITECONNECTION_H

#include "connection.h"

#include <QString>
#include <QModelIndex>

class SqliteConnection : public Connection
{
private:
    QString databasePath;
    QString databaseName;
    QString activeTableName;
    void handleTableClicked(QModelIndex index);
public:
    SqliteConnection();
    void init();

    void setDatabaseName(QString databaseName);
    QString getDatabaseName() const;

    void setDatabasePath(QString databasePath);
    QString getDatabasePath() const;
};

#endif // SQLITECONNECTION_H
