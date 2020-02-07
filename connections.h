#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include "connectioninfo.h"

#include <QMap>
#include <QString>
#include <QObject>

#include <QDebug>

class Connections : public QObject
{
    Q_OBJECT

public:
    void insert(ConnectionInfo*);
    void remove(ConnectionInfo*);
    void clear();
    bool contains(const QString&);
    void replace(ConnectionInfo*, ConnectionInfo*);
    int size() const;

    QMap<QString, QMap<QString, ConnectionInfo*>>::iterator begin();
    QMap<QString, QMap<QString, ConnectionInfo*>>::iterator end();

    QMap<QString, ConnectionInfo*> operator [] (QString connectionType) {
        return connections.value(connectionType);
    }

signals:
    void changed();

private:
    QMap<QString, QMap<QString, ConnectionInfo*>> connections;
};

#endif // CONNECTIONS_H
