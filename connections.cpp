#include "connections.h"

void Connections::insert(ConnectionInfo *connectionInfo) {
    connections[connectionInfo->getConnectionType()][connectionInfo->getConnectionName()] = connectionInfo;
}

void Connections::replace(ConnectionInfo* oldConnectionInfo, ConnectionInfo *newConnectionInfo) {
    connections[oldConnectionInfo->getConnectionType()].remove(oldConnectionInfo->getConnectionName());
//    delete oldConnectionInfo;
    connections[newConnectionInfo->getConnectionType()][newConnectionInfo->getConnectionName()] = newConnectionInfo;
}

void Connections::remove(ConnectionInfo *connectionInfo) {
    connections[connectionInfo->getConnectionType()].remove(connectionInfo->getConnectionName());
    delete connectionInfo;
}

QMap<QString, QMap<QString, ConnectionInfo *>>::iterator Connections::begin() {
    return connections.begin();
}

QMap<QString, QMap<QString, ConnectionInfo *>>::iterator Connections::end() {
    return connections.end();
}

bool Connections::contains(const QString &key) {
    return connections.contains(key);
}

void Connections::clear() {
    connections.clear();
}

int Connections::size() const {
    return connections.size();
}

//QMap<QString, ConnectionInfo*> &Connections::operator[] (QString &connectionType) {
//    return connections[connectionType];
//}
