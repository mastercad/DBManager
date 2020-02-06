#ifndef MYSQLCONNECTIONINFO_H
#define MYSQLCONNECTIONINFO_H

#include "connectioninfo.h"

#include <QString>

class MysqlConnectionInfo : public ConnectionInfo
{
private:
    QString host;
    QString user;
    QString password;
    uint port;
    QString databaseName;

public:
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
};

#endif // MYSQLCONNECTIONINFO_H
