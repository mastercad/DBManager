#include "connectionfactory.h"

#include "mysqlconnection.h"
#include "sqliteconnection.h"

#include <QDebug>

ConnectionFactory::ConnectionFactory(QObject* parent)
    :parent(parent) {}


Connection* ConnectionFactory::create(ConnectionInfo* connectionInfo) {
    // MYSQL Connection
    if ("MYSQL" == connectionInfo->getConnectionType()) {
        MysqlConnection* connection = new MysqlConnection(parent);
        connection->setConnectionInfo(connectionInfo);
        connection->init();

        return connection;
    // SQLITE Connection
    } else if ("SQLITE" == connectionInfo->getConnectionType()) {
        SqliteConnection* connection = new SqliteConnection(parent);
        connection->setConnectionInfo(connectionInfo);
        connection->init();

        return connection;
    }
    throw std::invalid_argument("No valid Connection Type given!");
}
