#include "connectioninfofactory.h"

#include "mysqlconnectioninfo.h"
#include "sqliteconnectioninfo.h"

// needed for field actions...
#include <QVariant>

ConnectionInfo* ConnectionInfoFactory::create(NewConnectionWizard& wizard) {
    // MYSQL Connection
    if ("YES" == wizard.field("mysql.connection.valid").toString()) {
//        MysqlConnectionInfo* connectionInfo = new MysqlConnectionInfo();
        ConnectionInfo* connectionInfo = new ConnectionInfo();
        connectionInfo->setHost(!wizard.field("mysql.host").toString().isEmpty() ? wizard.field("mysql.host").toString() : "localhost");
        connectionInfo->setPort(!wizard.field("mysql.port").isNull() ? wizard.field("mysql.port").toInt() : 3306);
        connectionInfo->setUser(!wizard.field("mysql.user").toString().isEmpty() ? wizard.field("mysql.user").toString() : "root");
        connectionInfo->setPassword(!wizard.field("mysql.password").toString().isEmpty() ? wizard.field("mysql.password").toString() : "root");
        connectionInfo->setConnectionName(!wizard.field("mysql.connection.name").toString().isEmpty() ? wizard.field("mysql.connection.name").toString() : generateNewConnectionName());
        connectionInfo->setDatabaseName(wizard.field("mysql.database").toString());
        connectionInfo->setConnectionType("MYSQL");

        return connectionInfo;
    // SQLITE Connection
    } else if ("YES" == wizard.field("sqlite.connection.valid").toString()) {
//        SqliteConnectionInfo* connectionInfo = new SqliteConnectionInfo();
        ConnectionInfo* connectionInfo = new ConnectionInfo();
        connectionInfo->setDatabasePath(wizard.field("sqlite.path").toString());
//        connectionInfo->setDatabaseName(wizard.field("sqlite.path").toString());
        connectionInfo->setConnectionName(!wizard.field("sqlite.connection.name").toString().isEmpty() ? wizard.field("mysql.connection.name").toString() : generateNewConnectionName());
        connectionInfo->setConnectionType("SQLITE");

        return connectionInfo;
    }
}

QString ConnectionInfoFactory::generateNewConnectionName() {
    return "NEW CONNECTION 2";
}
