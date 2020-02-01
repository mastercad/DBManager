#ifndef CONNECTIONFACTORY_H
#define CONNECTIONFACTORY_H

#include "connection.h"
#include "newconnectionwizard.h"

#include <QWizard>

class ConnectionFactory
{
public:
    Connection create(NewConnectionWizard& wizard);
};

#endif // CONNECTIONFACTORY_H
