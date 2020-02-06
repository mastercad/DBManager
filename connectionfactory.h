#ifndef CONNECTIONFACTORY_H
#define CONNECTIONFACTORY_H

#include "connection.h"
#include "connectioninfo.h"

class ConnectionFactory
{
public:
    Connection* create(ConnectionInfo* connectionInfo);
};

#endif // CONNECTIONFACTORY_H
