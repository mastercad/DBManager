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
    void handleTableClicked(QModelIndex index);
    void loadDatabaseList();
    void loadTablesList();

public:
    SqliteConnection();
    void init();

    void setDatabaseName(QString databaseName);
    QString getDatabaseName() const;

    void setDatabasePath(QString databasePath);
    QString getDatabasePath() const;

public slots:
    void onListViewDoubleClicked(const QModelIndex index);
};

#endif // SQLITECONNECTION_H
