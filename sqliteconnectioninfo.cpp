#include "sqliteconnectioninfo.h"

void SqliteConnectionInfo::setDatabaseName(QString databaseName) {
    this->databaseName = databaseName;
}

QString SqliteConnectionInfo::getDatabaseName() const {
    return this->databaseName;
}

void SqliteConnectionInfo::setDatabasePath(QString databasePath) {
    this->databasePath = databasePath;
}

QString SqliteConnectionInfo::getDatabasePath() const {
    return this->databasePath;
}
