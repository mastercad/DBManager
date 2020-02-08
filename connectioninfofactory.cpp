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

    if(stream.tokenType() != QXmlStreamReader::StartElement
        && "connection" == stream.name()
    ) {
        qFatal("could not parse connection information!");
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
            if ("name" == elementName) {
                connectionInfo->setConnectionName(stream.text().toString());
            } else if ("type" == elementName) {
                connectionInfo->setConnectionType(stream.text().toString());
            } else if ("host" == elementName) {
                connectionInfo->setHost(stream.text().toString());
            } else if ("port" == elementName) {
                connectionInfo->setPort(stream.text().toInt());
            } else if ("user" == elementName) {
                connectionInfo->setUser(stream.text().toString());
            } else if ("password" == elementName) {
                connectionInfo->setPassword(stream.text().toString());
            } else if ("databasePath" == elementName) {
                connectionInfo->setDatabasePath(stream.text().toString());
            } else if ("database" == elementName) {
                connectionInfo->setDatabaseName(stream.text().toString());
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
        connectionInfo->setHost(wizard.field("mysql.host").toString());
        connectionInfo->setPort(wizard.field("mysql.port").toInt());
        connectionInfo->setUser(wizard.field("mysql.user").toString());
        connectionInfo->setPassword(wizard.field("mysql.password").toString());
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
    return nullptr;
}

ConnectionInfo* ConnectionInfoFactory::create(ConnectionWizardMysqlPage* wizardPage) {
    ConnectionInfo* connectionInfo = new ConnectionInfo();
    connectionInfo->setHost(wizardPage->getHostEdit()->text());
    connectionInfo->setPort(wizardPage->getPortEdit()->text().toInt());
    connectionInfo->setUser(wizardPage->getUserEdit()->text());
    connectionInfo->setPassword(wizardPage->getPasswordEdit()->text());
    connectionInfo->setDatabaseName(wizardPage->getDatabaseEdit()->text());
    connectionInfo->setConnectionType("MYSQL");
    qDebug() << "Generate Connection Name!";
    connectionInfo->setConnectionName(!wizardPage->getNameEdit()->text().isEmpty() ? wizardPage->getNameEdit()->text() : generateNewConnectionName(connectionInfo));
    qDebug() << "Finish generate Connection Name!";

    return connectionInfo;
}

QString ConnectionInfoFactory::generateNewConnectionName(ConnectionInfo* connectionInfo, int currentNumber) {
    QString connectionName = "NEW CONNECTION";

    if (0 < currentNumber) {
        connectionName = QString(connectionName + " " + QString::number(currentNumber));
    }

    if (nullptr == connections) {
        return connectionName;
    }
    qDebug() << "Habe connections!";

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
    connectionInfo->setConnectionNameAutoGenerated(true);

    return connectionName;
}

//void ConnectionInfoFactory::setConnections(QMap<QString, QMap<QString, ConnectionInfo *> >* connections) {
void ConnectionInfoFactory::setConnections(Connections* connections) {
    this->connections = connections;
}

//QMap<QString, QMap<QString, ConnectionInfo*> >* ConnectionInfoFactory::getConnections() const {
Connections* ConnectionInfoFactory::getConnections() const {
    return this->connections;
}
