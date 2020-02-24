#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "textedit.h"
#include "newconnectionwizard.h"
#include "connectionmanager.h"

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

#include "defaults.h"

// Driver not loaded
// Access denied for user 'root'@'172.19.0.1' (using password: YES)
// Unknown database 'retro_board'

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
    connectionFactory = new ConnectionFactory(this);
    connectionInfoFactory = new ConnectionInfoFactory;
    connectionInfoFactory->setConnections(&connections);
    setWindowTitle(QString("%1").arg("Database Manager"));

    QIcon executeIcon(":/icons/ausfuehren_schatten.png");
    ui->btnQueryExecute->setIcon(executeIcon);

    ui->queryResult->setSortingEnabled(true);

    loadConnectionInfos();

    ui->databaseList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->btnQueryExecute, SIGNAL(clicked(bool)), this, SLOT(onExecuteQueryClicked()));
    connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionEditConnection, SIGNAL(triggered(bool)), this, SLOT(openConnectionManagerWindow()));
    connect(ui->actionNewConnection, SIGNAL(triggered(bool)), this, SLOT(openNewConnectionWindow()));
    connect(ui->menuVerbinden, SIGNAL(triggered(QAction*)), this, SLOT(onEstablishNewConnection(QAction*)));
    connect(&this->connections, SIGNAL(changed()), this, SLOT(createConnectionSubMenu()));
    connect(&this->connections, SIGNAL(changed()), this, SLOT(saveConnectionInfos()));
}

void MainWindow::openNewConnectionWindow() {
    NewConnectionWizard wizard;

    if (wizard.exec()) {
        if (nullptr != dbConnection) {
            dbConnection->close();
        }

        connectionInfo = connectionInfoFactory->create(wizard);
        storeConnectionInfo(connectionInfo);
        dbConnection = establishNewConnection(connectionInfo);
        saveConnectionInfos();
    }
}

void MainWindow::openConnectionManagerWindow() {
    ConnectionManager manager(this, &connections);
    if (manager.exec()) {
    }
}

void MainWindow::createConnectionSubMenu() {
    ui->menuVerbinden->clear();
//    QMapIterator<QString, QMap<QString, ConnectionInfo*> > typeIterator(connections);
    QMap<QString, QMap<QString, ConnectionInfo*>>::iterator typeIterator = connections.begin();

//    while (typeIterator.hasNext()) {
    while(typeIterator != connections.end()) {
//        typeIterator.next();
        QMenu* typeMenu = new QMenu(typeIterator.key());
        QMapIterator<QString, ConnectionInfo*> connectionsIterator(typeIterator.value());
        while(connectionsIterator.hasNext()) {
            connectionsIterator.next();
            typeMenu->addAction(connectionsIterator.value()->getConnectionName());
        }
        ui->menuVerbinden->addMenu(typeMenu);
        ++typeIterator;
    }
}

void MainWindow::showResultTableContextMenu(const QPoint& point) {
    QMenu *menu=new QMenu();
//    QAction copyAction(tr("copy"), this);
    QAction copyAction(QCoreApplication::tr("copy"));
    connect(&copyAction, SIGNAL(triggered()), dbConnection, SLOT(copyResultViewSelection()));
    menu->addAction(&copyAction);

    QAction deleteAction(tr("delete"), this);
    connect(&deleteAction, SIGNAL(triggered()), dbConnection, SLOT(deleteResultViewSelection()));
    menu->addAction(&deleteAction);

    QAction pasteAction(tr("paste"), this);
    connect(&pasteAction, SIGNAL(triggered()), dbConnection, SLOT(pasteToResultView()));
    menu->addAction(&pasteAction);

    QAction insertNullAction(tr("insert NULL"));
    connect(&insertNullAction, SIGNAL(triggered()), dbConnection, SLOT(insertNullToResultView()));
    menu->addAction(&insertNullAction);

    if (menu->exec(ui->queryResult->viewport()->mapToGlobal(point))) {
//        this->currentContextMenuItem = nullptr;
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
//    QMapIterator<QString, QMap<QString, ConnectionInfo*> > typeIterator(connections);
    QMap<QString, QMap<QString, ConnectionInfo*>>::iterator typeIterator = connections.begin();

//    while (typeIterator.hasNext()) {
    while(typeIterator != connections.end()) {
//        typeIterator.next();
        QMapIterator<QString, ConnectionInfo*> connectionsIterator(typeIterator.value());
        while(connectionsIterator.hasNext()) {
            connectionsIterator.next();
            stream.writeStartElement("connection");
//            stream.writeAttribute("href", "url");
            if (!connectionsIterator.value()->getConnectionName().isEmpty()
                && !connectionsIterator.value()->isConnectionNameAutogenerated()
            ) {
                stream.writeTextElement("name", connectionsIterator.value()->getConnectionName());
            }
            stream.writeTextElement("type", connectionsIterator.value()->getConnectionType());
            if ("MYSQL" == connectionsIterator.value()->getConnectionType()) {
                if (!connectionsIterator.value()->getHost().isEmpty()) {
                    stream.writeTextElement("host", connectionsIterator.value()->getHost());
                }
                if (0 < connectionsIterator.value()->getPort()) {
                    stream.writeTextElement("port", QString::number(connectionsIterator.value()->getPort()));
                }
                if (!connectionsIterator.value()->getUser().isEmpty()) {
                    stream.writeTextElement("user", connectionsIterator.value()->getUser());
                }
                if (!connectionsIterator.value()->getPassword().isEmpty()) {
                    stream.writeTextElement("password", connectionsIterator.value()->getPassword());
                }
                if (!connectionsIterator.value()->getDatabaseName().isEmpty()) {
                    stream.writeTextElement("database", connectionsIterator.value()->getDatabaseName());
                }
            } else if ("SQLITE" == connectionsIterator.value()->getConnectionType()) {
                stream.writeTextElement("databasePath", connectionsIterator.value()->getDatabasePath());
            }
            stream.writeEndElement();
        }
        ++typeIterator;
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
        QXmlStreamReader::TokenType token = stream.readNext();
        if (QXmlStreamReader::StartElement == token
            && "connection" == stream.name()
        ) {
            ConnectionInfo* connectionInfo = connectionInfoFactory->create(stream);
            connections.insert(connectionInfo);
//            connections[connectionInfo->getConnectionType()][connectionInfo->getConnectionName()] = connectionInfo;
        }
    }
    createConnectionSubMenu();
    file.close();

    emit connections.changed();
}

void MainWindow::storeConnectionInfo(ConnectionInfo* connectionInfo) {
    connections.insert(connectionInfo);
    connectionsSaved = false;

    emit connections.changed();
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
    connect(dbConnection, SIGNAL(connectionError(QString)), this, SLOT(handleConnectionError(QString)));
    connect(ui->databaseList, SIGNAL(customContextMenuRequested(const QPoint&)), this->dbConnection, SLOT(showDatabaseContextMenu(const QPoint&)));
    ui->queryResult->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->queryResult, SIGNAL(customContextMenuRequested(QPoint)), this->dbConnection, SLOT(showResultTableContextMenu(QPoint)));

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

void MainWindow::handleConnectionError(QString errorMessage) {
    this->ui->information->append(errorMessage);
}

void MainWindow::onExecuteQueryClicked() {
    QString queryString = ui->queryRequest->toPlainText();

    if (nullptr == dbConnection) {
        this->ui->information->append(tr("No Connection established!"));
        return;
    }
    QSqlQuery query = dbConnection->sendQuery(queryString);

    QStandardItemModel* queryResultModel = new QStandardItemModel;

    if (query.isActive()) {
        QSqlRecord localRecord = query.record();
        qDebug() << "Local Record: " << localRecord;
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
    statusBar()->showMessage(tr("connected!"));
}

void MainWindow::onQueryResultHeaderClicked(QStandardItem* item) {

}

MainWindow::~MainWindow() {
    delete ui;
//    delete queryResultModel;
}
