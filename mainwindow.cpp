#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "textedit.h"
#include "newconnectionwizard.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QElapsedTimer>
#include <QDateTime>
#include <QList>
#include <QCompleter>
#include <QWizard>
#include <QWizardPage>
#include <QMenu>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include <QDebug>

// Driver not loaded
// Access denied for user 'root'@'172.19.0.1' (using password: YES)
// Unknown database 'retro_board'

QString hostName;
QString userName;
QString password;
QString port;
QString databaseName;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::showMaximized();
    connectionFactory = new ConnectionFactory;
    connectionInfoFactory = new ConnectionInfoFactory;
    connectionInfoFactory->setConnections(&connections);
    setWindowTitle(QString("%1").arg("Database Manager"));

    ui->queryResult->setSortingEnabled(true);

    loadConnectionInfos();

    connect(ui->btnQueryExecute, SIGNAL(clicked(bool)), this, SLOT(onExecuteQueryClicked()));
    connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionNewConnection, SIGNAL(triggered(bool)), this, SLOT(openNewConnectionWindow()));
    connect(ui->menuVerbinden, SIGNAL(triggered(QAction*)), this, SLOT(onEstablishNewConnection(QAction*)));
}

void MainWindow::openNewConnectionWindow() {
    NewConnectionWizard wizard;
    if (wizard.exec()) {
        if (NULL != dbConnection) {
            dbConnection->close();
        }

        connectionInfo = connectionInfoFactory->create(wizard);
        storeConnectionInfo(connectionInfo);
        dbConnection = establishNewConnection(connectionInfo);
        createConnectionSubMenu();
        saveConnectionInfos();
    }
}

void MainWindow::createConnectionSubMenu() {
    ui->menuVerbinden->clear();
    QMapIterator<QString, QMap<QString, ConnectionInfo*> > typeIterator(connections);
    while (typeIterator.hasNext()) {
        typeIterator.next();
        QMenu* typeMenu = new QMenu(typeIterator.key());
        QMapIterator<QString, ConnectionInfo*> connectionsIterator(typeIterator.value());
        while(connectionsIterator.hasNext()) {
            connectionsIterator.next();
            typeMenu->addAction(connectionsIterator.key());
        }
        ui->menuVerbinden->addMenu(typeMenu);
    }
}

void MainWindow::saveConnectionInfos() {
    QFile file("DBManager.xml");
    file.open(QFile::ReadWrite);
    // delete all current content of file.
    file.resize(0);

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("connections");
    QMapIterator<QString, QMap<QString, ConnectionInfo*> > typeIterator(connections);

    while (typeIterator.hasNext()) {
        typeIterator.next();
        QMapIterator<QString, ConnectionInfo*> connectionsIterator(typeIterator.value());
        while(connectionsIterator.hasNext()) {
            connectionsIterator.next();
            stream.writeStartElement("connection");
//            stream.writeAttribute("href", "url");
            if (!connectionsIterator.value()->getConnectionName().isEmpty()) {
                stream.writeTextElement("name", connectionsIterator.value()->getConnectionName());
            }
            stream.writeTextElement("type", connectionsIterator.value()->getConnectionType());
            if ("MYSQL" == connectionsIterator.value()->getConnectionType()) {
                stream.writeTextElement("host", connectionsIterator.value()->getHost());
                stream.writeTextElement("port", QString::number(connectionsIterator.value()->getPort()));
                stream.writeTextElement("user", connectionsIterator.value()->getUser());
                stream.writeTextElement("password", connectionsIterator.value()->getPassword());
            } else if ("SQLITE" == connectionsIterator.value()->getConnectionType()) {
                stream.writeTextElement("databasePath", connectionsIterator.value()->getDatabasePath());
            }
            stream.writeEndElement();
        }
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    file.close();
}

void MainWindow::loadConnectionInfos() {
    QFile file("DBManager.xml");
    file.open(QFile::ReadOnly | QFile::Text);
    QXmlStreamReader stream(&file);
    connections.clear();
    while(!stream.atEnd()
        && !stream.hasError()
    ) {
//        while (stream.readNextStartElement()) {
//            qDebug() << stream.name();
//        }
        QXmlStreamReader::TokenType token = stream.readNext();
        if (QXmlStreamReader::StartElement == token
            && "connection" == stream.name()
        ) {
            ConnectionInfo* connectionInfo = connectionInfoFactory->create(stream);
            connections[connectionInfo->getConnectionType()][connectionInfo->getConnectionName()] = connectionInfo;
        }
    }
    createConnectionSubMenu();
    file.close();
}

void MainWindow::storeConnectionInfo(ConnectionInfo* connectionInfo) {
    QMap<QString, ConnectionInfo*>connectionInfos;
    if (connections.contains(connectionInfo->getConnectionType())) {
        connectionInfos = connections[connectionInfo->getConnectionType()];
    }
    connectionInfos[connectionInfo->getConnectionName()] = connectionInfo;
    connections[connectionInfo->getConnectionType()] = connectionInfos;
    connectionsSaved = false;
}

void MainWindow::onEstablishNewConnection(QAction *action) {
    QWidget* widget = action->parentWidget();

    if (widget) {
        QMenu* menu = dynamic_cast<QMenu* >(widget);
        establishNewConnection(connections[menu->title()][action->text()]);
    }
}

Connection* MainWindow::establishNewConnection(ConnectionInfo* connectionInfo) {
    dbConnection = connectionFactory->create(connectionInfo);
    dbConnection->setDatabaseListView(ui->databaseList);
    dbConnection->setQueryResultView(ui->queryResult);
    dbConnection->setQueryRequestView(ui->queryRequest);
    dbConnection->setInformationView(ui->information);
    dbConnection->loadDatabaseList();

    if ("SQLITE" == connectionInfo->getConnectionType()) {
        setWindowTitle(QString("%1 - %2").arg(connectionInfo->getDatabaseName()).arg("Database Manager"));
    } else {
        setWindowTitle(QString("%1@%2 - %3").arg(connectionInfo->getUser()).arg(connectionInfo->getHost()).arg("Database Manager"));
    }

    return dbConnection;
}

void MainWindow::onExecuteQueryClicked() {
    QString queryString = ui->queryRequest->toPlainText();
    QSqlQuery query = dbConnection->sendQuery(queryString);

    QStandardItemModel* queryResultModel = new QStandardItemModel;

    if (query.isActive()) {
        QSqlRecord localRecord = query.record();
        for (int currentPos = 0; currentPos < localRecord.count(); ++currentPos) {
            QStandardItem* headerItem = new QStandardItem(localRecord.fieldName(currentPos));
            queryResultModel->setHorizontalHeaderItem(currentPos, headerItem);
        }

        query.seek(-1);

        int currentRow = 0;
        while (query.next()) {
            QSqlRecord currentRecord = query.record();
            for (int currentColumn = 0; currentColumn < currentRecord.count(); ++currentColumn) {
                QStandardItem* value = new QStandardItem(currentRecord.field(currentColumn).value().toString());
                queryResultModel->setItem(currentRow, currentColumn, value);
            }
            ++currentRow;
        }
    }
    ui->queryResult->setModel(queryResultModel);
    ui->queryResult->resizeColumnsToContents();
    statusBar()->showMessage("connected!");
}

void MainWindow::onQueryResultHeaderClicked(QStandardItem* item) {

}

MainWindow::~MainWindow() {
    delete ui;
    delete newConnectionWindow;
//    delete queryResultModel;
}
