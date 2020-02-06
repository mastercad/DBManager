#include "connectioninfofactory.h"

#include "mysqlconnectioninfo.h"
#include "sqliteconnectioninfo.h"

// needed for field actions...
#include <QVariant>
#include <QXmlStreamAttributes>
#include <QMapIterator>
#include <QDebug>

ConnectionInfo* ConnectionInfoFactory::create(QXmlStreamReader& stream) {
    ConnectionInfo* connectionInfo = new ConnectionInfo();

    connectionInfo->setHost("localhost");
    connectionInfo->setPort(3306);
    connectionInfo->setUser("root");
    connectionInfo->setPassword("root");

    if(stream.tokenType() != QXmlStreamReader::StartElement
        && stream.name() == "connection"
    ) {
        qFatal("could not parse connection information!");
        return connectionInfo;
    }

//    QXmlStreamAttributes attributes = stream.attributes();
    /* Let's check that person has id attribute. */
//    if(attributes.hasAttribute("name")) {
        /* We'll add it to the map. */
//        connectionInfo->setId["name"] = attributes.value("name").toString();
//    }

    stream.readNext();

    while (!(QXmlStreamReader::EndElement == stream.tokenType()
        && "connection" == stream.name())
    ) {
        if (QXmlStreamReader::StartElement == stream.tokenType()) {
            QString elementName = stream.name().toString();
            stream.readNext();
            if (QXmlStreamReader::Characters != stream.tokenType()) {
                continue;
            }
            if ("name" == elementName
                && !stream.text().toString().isEmpty()
            ) {
                connectionInfo->setConnectionName(stream.text().toString());
            } else if ("type" == elementName
                && !stream.text().toString().isEmpty()
            ) {
                connectionInfo->setConnectionType(stream.text().toString());
            } else if ("host" == elementName
                && !stream.text().toString().isEmpty()
            ) {
                connectionInfo->setHost(stream.text().toString());
            } else if ("port" == elementName
                && 0 < stream.text().toUInt()
            ) {
                connectionInfo->setPort(stream.text().toUInt());
            } else if ("user" == elementName
                && !stream.text().toString().isEmpty()
            ) {
                connectionInfo->setUser(stream.text().toString());
            } else if ("password" == elementName
                && !stream.text().toString().isEmpty()
            ) {
                connectionInfo->setPassword(stream.text().toString());
            } else if ("databasePath" == elementName
                && !stream.text().toString().isEmpty()
            ) {
                connectionInfo->setDatabasePath(stream.text().toString());
            }
        }
        stream.readNext();
    }

    if (connectionInfo->getConnectionName().isEmpty()) {
        connectionInfo->setConnectionName(generateNewConnectionName(connectionInfo));
    }
    return connectionInfo;
}

ConnectionInfo* ConnectionInfoFactory::create(NewConnectionWizard& wizard) {
    // MYSQL Connection
    if ("YES" == wizard.field("mysql.connection.valid").toString()) {
//        MysqlConnectionInfo* connectionInfo = new MysqlConnectionInfo();
        ConnectionInfo* connectionInfo = new ConnectionInfo();
        connectionInfo->setHost(!wizard.field("mysql.host").toString().isEmpty() ? wizard.field("mysql.host").toString() : "localhost");
        connectionInfo->setPort(!wizard.field("mysql.port").isNull() && 0 < wizard.field("mysql.port").toInt() ? wizard.field("mysql.port").toInt() : 3306);
        connectionInfo->setUser(!wizard.field("mysql.user").toString().isEmpty() ? wizard.field("mysql.user").toString() : "root");
        connectionInfo->setPassword(!wizard.field("mysql.password").toString().isEmpty() ? wizard.field("mysql.password").toString() : "root");
        connectionInfo->setDatabaseName(wizard.field("mysql.database").toString());
        connectionInfo->setConnectionType("MYSQL");
        connectionInfo->setConnectionName(!wizard.field("mysql.connection.name").toString().isEmpty() ? wizard.field("mysql.connection.name").toString() : generateNewConnectionName(connectionInfo));

        return connectionInfo;
    // SQLITE Connection
    } else if ("YES" == wizard.field("sqlite.connection.valid").toString()) {
//        SqliteConnectionInfo* connectionInfo = new SqliteConnectionInfo();
        ConnectionInfo* connectionInfo = new ConnectionInfo();
        connectionInfo->setDatabasePath(wizard.field("sqlite.path").toString());
//        connectionInfo->setDatabaseName(wizard.field("sqlite.path").toString());
        connectionInfo->setConnectionType("SQLITE");
        connectionInfo->setConnectionName(!wizard.field("sqlite.connection.name").toString().isEmpty() ? wizard.field("mysql.connection.name").toString() : generateNewConnectionName(connectionInfo));

        return connectionInfo;
    }
}

QString ConnectionInfoFactory::generateNewConnectionName(ConnectionInfo* connectionInfo, int currentNumber) {
    QString connectionName = "NEW CONNECTION";

    if (0 < currentNumber) {
        connectionName = QString(connectionName + " " + QString::number(currentNumber));
    }

    QMap<QString, QMap<QString, ConnectionInfo*> >::iterator typeIterator = connections->begin();

    while (typeIterator != connections->end()) {
        QMapIterator<QString, ConnectionInfo*> connectionsIterator(typeIterator.value());
        while(connectionsIterator.hasNext()) {
            connectionsIterator.next();
            if (connectionName == connectionsIterator.value()->getConnectionName()) {
                return generateNewConnectionName(connectionInfo, ++currentNumber);
            }
        }
        ++typeIterator;
    }

    return connectionName;
}

void ConnectionInfoFactory::setConnections(QMap<QString, QMap<QString, ConnectionInfo *> >* connections) {
    this->connections = connections;
}

QMap<QString, QMap<QString, ConnectionInfo*> >* ConnectionInfoFactory::getConnections() const {
    return this->connections;
}
