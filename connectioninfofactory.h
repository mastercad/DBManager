#ifndef CONNECTIONINFOFACTORY_H
#define CONNECTIONINFOFACTORY_H

#include "connectioninfo.h"
#include "newconnectionwizard.h"

#include <QXmlStreamReader>
#include <QMap>
#include <QString>

class ConnectionInfoFactory
{
private:
    QString generateNewConnectionName(ConnectionInfo* connectionInfo, int = 0);
    QMap<QString, QMap<QString, ConnectionInfo*> >* connections;

public:
    ConnectionInfo* create(NewConnectionWizard& wizard);
    ConnectionInfo* create(QXmlStreamReader& stream);

    void setConnections(QMap<QString, QMap<QString, ConnectionInfo*> >* connections);
    QMap<QString, QMap<QString, ConnectionInfo*> >* getConnections() const;
};

#endif // CONNECTIONINFOFACTORY_H
