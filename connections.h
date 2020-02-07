#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include "connectioninfo.h"

#include <QMap>
#include <QString>

class Connections
{
public:
    Connections();
    void insertConnection(QString&, QString&, ConnectionInfo*);
    void deleteConnection(ConnectionInfo*);
    void replaceConnection(ConnectionInfo*);

signals:
    void changed();

private:
    QMap<QString, QMap<QString, ConnectionInfo*> >* connections;
};

#endif // CONNECTIONS_H
