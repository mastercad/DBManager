#ifndef MYSQLCONNECTIONVALIDATOR_H
#define MYSQLCONNECTIONVALIDATOR_H

#include "connection.h"

class MysqlConnectionValidator
{
public:
    MysqlConnectionValidator();
    bool static validate(Connection* connection);

};

#endif // MYSQLCONNECTIONVALIDATOR_H
