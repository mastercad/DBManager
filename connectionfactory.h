#ifndef CONNECTIONFACTORY_H
#define CONNECTIONFACTORY_H

#include "connection.h"
#include "connectioninfo.h"

#include <QObject>

class ConnectionFactory
{
public:

    ConnectionFactory(QObject* parent);

    Connection* create(ConnectionInfo* connectionInfo);
    QObject* parent = nullptr;
};

#endif // CONNECTIONFACTORY_H
