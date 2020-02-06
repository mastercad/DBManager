#include "connectioninfo.h"

void ConnectionInfo::setConnectionType(QString connectionType) {
    this->connectionType = connectionType;
}

QString ConnectionInfo::getConnectionType() const {
    return this->connectionType;
}

void ConnectionInfo::setConnectionName(QString connectionName) {
    this->connectionName = connectionName;
}

QString ConnectionInfo::getConnectionName() const {
    return this->connectionName;
}

void ConnectionInfo::setDatabasePath(QString databasePath) {
    this->databasePath = databasePath;
}

QString ConnectionInfo::getDatabasePath() const {
    return this->databasePath;
}

void ConnectionInfo::setHost(QString host) {
    this->host = host;
}

QString ConnectionInfo::getHost() const {
    return this->host;
}

void ConnectionInfo::setUser(QString user) {
    this->user = user;
}

QString ConnectionInfo::getUser() const {
    return this->user;
}

void ConnectionInfo::setPassword(QString password) {
    this->password = password;
}

QString ConnectionInfo::getPassword() const {
    return this->password;
}

void ConnectionInfo::setPort(uint port) {
    this->port = port;
}

uint ConnectionInfo::getPort() const {
    return this->port;
}

void ConnectionInfo::setDatabaseName(QString databaseName) {
    this->databaseName = databaseName;
}

QString ConnectionInfo::getDatabaseName() const {
    return this->databaseName;
}
