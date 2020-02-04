#include "connectionfactory.h"

#include "mysqlconnection.h"
#include "sqliteconnection.h"

#include <QDebug>

Connection* ConnectionFactory::create(NewConnectionWizard& wizard) {
    // MYSQL Connection
    if ("YES" == wizard.field("mysql.connection.valid").toString()) {
        qDebug() << "Mysql User Name" << wizard.field("mysql.user").toString();
        MysqlConnection* connection = new MysqlConnection;
        connection->setHost(!wizard.field("mysql.host").toString().isEmpty() ? wizard.field("mysql.host").toString() : "localhost");
        connection->setPort(!wizard.field("mysql.port").isNull() ? wizard.field("mysql.port").toInt() : 3306);
        connection->setUser(!wizard.field("mysql.user").toString().isEmpty() ? wizard.field("mysql.user").toString() : "root");
        connection->setPassword(!wizard.field("mysql.password").toString().isEmpty() ? wizard.field("mysql.password").toString() : "root");
        connection->setDatabaseName(wizard.field("mysql.database").toString());
        connection->init();

        return connection;
    // SQLITE Connection
    } else if ("YES" == wizard.field("sqlite.connection.valid").toString()) {
        SqliteConnection* connection = new SqliteConnection;
        connection->setDatabasePath(wizard.field("sqlite.path").toString());
        connection->setDatabaseName(wizard.field("sqlite.path").toString());
        connection->init();

        return connection;
    }
}

