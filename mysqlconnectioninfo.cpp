#include "mysqlconnectioninfo.h"

void MysqlConnectionInfo::setHost(QString host) {
    this->host = host;
}

QString MysqlConnectionInfo::getHost() const {
    return this->host;
}

void MysqlConnectionInfo::setUser(QString user) {
    this->user = user;
}

QString MysqlConnectionInfo::getUser() const {
    return this->user;
}

void MysqlConnectionInfo::setPassword(QString password) {
    this->password = password;
}

QString MysqlConnectionInfo::getPassword() const {
    return this->password;
}

void MysqlConnectionInfo::setPort(uint port) {
    this->port = port;
}

uint MysqlConnectionInfo::getPort() const {
    return this->port;
}

void MysqlConnectionInfo::setDatabaseName(QString databaseName) {
    this->databaseName = databaseName;
}

QString MysqlConnectionInfo::getDatabaseName() const {
    return this->databaseName;
}
