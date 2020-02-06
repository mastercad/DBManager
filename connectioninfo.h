#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QString>

class ConnectionInfo {
private:
    QString connectionType;
    QString connectionName;

    QString databasePath;

    QString host;
    QString user;
    QString password;
    uint port;
    QString databaseName;

public:

    void setConnectionType(QString connectionType);
    QString getConnectionType() const;

    void setConnectionName(QString connectionName);
    QString getConnectionName() const;

    /*** MYSQL ***/
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

    void setDatabasePath(QString databasePath);
    QString getDatabasePath() const;

};

#endif // CONNECTIONINFO_H
