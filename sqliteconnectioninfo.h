#ifndef SQLITECONNECTIONINFO_H
#define SQLITECONNECTIONINFO_H

#include "connectioninfo.h"

class SqliteConnectionInfo : public ConnectionInfo
{
private:
    QString databasePath;
    QString databaseName;

public:
    void setDatabaseName(QString databaseName);
    QString getDatabaseName() const;

    void setDatabasePath(QString databasePath);
    QString getDatabasePath() const;
};

#endif // SQLITECONNECTIONINFO_H
