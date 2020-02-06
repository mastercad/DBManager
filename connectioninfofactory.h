#ifndef CONNECTIONINFOFACTORY_H
#define CONNECTIONINFOFACTORY_H

#include "connectioninfo.h"
#include "newconnectionwizard.h"

#include <QWizard>
#include <QString>

class ConnectionInfoFactory
{
private:
    QString generateNewConnectionName();

public:
    ConnectionInfo* create(NewConnectionWizard& wizard);
};

#endif // CONNECTIONINFOFACTORY_H
