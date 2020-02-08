#include "mysqlconnectionvalidator.h"

#include <QSqlDatabase>

#include <QDebug>

MysqlConnectionValidator::MysqlConnectionValidator() {

}

bool MysqlConnectionValidator::validate(Connection* connection) {
    connection->close();
    connection->open();

    return connection->isOpen();
}
