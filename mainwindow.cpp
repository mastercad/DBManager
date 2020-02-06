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
    setWindowTitle(QString("%1").arg("Database Manager"));

    ui->queryResult->setSortingEnabled(true);
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

void MainWindow::storeConnectionInfo(ConnectionInfo* connectionInfo) {
    if (!connections.contains(connectionInfo->getConnectionType())) {
    }
    QMap<QString, ConnectionInfo*>connectionInfos;
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

void MainWindow::createToolbars() {
//    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
//    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
//    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
//    connect(openAct, &QAction::triggered, this, &MainWindow::open);
//    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

//    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    //aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

    statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow() {
    delete ui;
    delete newConnectionWindow;
//    delete queryResultModel;
}
