#include "mysqlconnectionvalidator.h"

#include <QSqlDatabase>

#include <QDebug>

MysqlConnectionValidator::MysqlConnectionValidator() {

}

bool MysqlConnectionValidator::validate(Connection* connection) {
    qDebug() << "Test Connection " << connection->getConnectionInfo()->getConnectionName() << " for " << connection->getConnectionInfo()->getConnectionType();
    connection->close();
    connection->open();

    return connection->isOpen();
}
