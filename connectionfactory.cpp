#include "connectionfactory.h"

#include "mysqlconnection.h"
#include "sqliteconnection.h"

Connection ConnectionFactory::create(NewConnectionWizard& wizard) {
    // MYSQL Connection
    if ("YES" == wizard.field("mysql.connection.valid").toString()) {
        MysqlConnection connection;
        connection.setHost(wizard.field("mysql.host").toString());
        connection.setDatabaseName(wizard.field("mysql.database").toString());
        connection.setPort(wizard.field("mysql.port").toInt());
        connection.setUser(wizard.field("mysql.user").toString());
        connection.setPassword(wizard.field("mysql.password").toString());
        connection.init();

        return connection;
    // SQLITE Connection
    } else if ("YES" == wizard.field("sqlite.connection.valid").toString()) {
        SqliteConnection connection;
        connection.setDatabaseName(wizard.field("sqlite.path").toString());
        connection.init();

        return connection;
    }
}

