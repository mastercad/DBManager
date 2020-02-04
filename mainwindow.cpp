#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "textedit.h"
#include "newconnectionwindow.h"
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
    setWindowTitle(QString("%1").arg("Database Manager"));

    /*
    TextEdit* completingTextEdit = new TextEdit;
    */

    /*
    completer = new QCompleter(this);
//    completer->setModel(modelFromFile(":/resources/wordlist.txt"));
    completer->setModel(databaseCollection);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    ui->queryRequest->setCompleter(completer);
    */


/*
    setCentralWidget(completingTextEdit);
    */

//    ui->tabWidget->addTab(new GeneralTab(fileInfo), tr("General"));
//    ui->tabWidget->addTab(new PermissionsTab(fileInfo), tr("Permissions"));
//    ui->tabWidget->addTab(new ApplicationsTab(fileInfo), tr("Applications"));
/*
    dbConnection = QSqlDatabase::addDatabase("QMYSQL", "retro");

    dbConnection.setHostName("172.19.2.2");
    dbConnection.setDatabaseName("retro");
    dbConnection.setUserName("root");
    dbConnection.setPassword("XQW16asZ");

    QSqlDatabase mysqlDb = QSqlDatabase::addDatabase("QMYSQL", "mysql");
    mysqlDb.setHostName("172.19.2.2");
    mysqlDb.setDatabaseName("mysql");
    mysqlDb.setUserName("root");
    mysqlDb.setPassword("XQW16asZ");

    bool retroOk = retroDb.open();
    bool mysqlOk = mysqlDb.open();

    qDebug() << "OK: " << retroOk;
    qDebug() << "OK: " << mysqlOk;
    qDebug() << "Error: " << retroDb.lastError();
    qDebug() << "Error: " << mysqlDb.lastError();

    QSqlQuery retroQuery(retroDb);
    retroQuery.setForwardOnly(true);
    retroQuery.exec("SHOW DATABASES");

    if (retroQuery.first()) {
        while (retroQuery.next()) {
            qDebug() << retroQuery.value(0).toString();
        }
    }

    qDebug() << "SHOW TABLES für retro";
    retroQuery.exec("SHOW TABLES");

    if (retroQuery.first()) {
        while (retroQuery.next()) {
            qDebug() << retroQuery.value(0).toString();
        }
    }

    QSqlQuery mysqlQuery(mysqlDb);
    qDebug() << "SHOW TABLES für mysql";
    mysqlQuery.exec("SHOW TABLES");

    if (mysqlQuery.first()) {
        while (mysqlQuery.next()) {
            qDebug() << mysqlQuery.value(0).toString();
        }
    }
*/
    ui->queryResult->setSortingEnabled(true);
    connect(ui->btnQueryExecute, SIGNAL(clicked(bool)), this, SLOT(onExecuteQueryClicked()));
    connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionNewConnection, SIGNAL(triggered(bool)), this, SLOT(openNewConnectionWindow()));
}

void MainWindow::openNewConnectionWindow() {
    NewConnectionWizard wizard;
    if (wizard.exec()) {
        if (NULL != dbConnection) {
            dbConnection->close();
        }

        dbConnection = connectionFactory->create(wizard);
        dbConnection->setDatabaseListView(ui->databaseList);
        dbConnection->setQueryResultView(ui->queryResult);
        dbConnection->setQueryRequestView(ui->queryRequest);
        dbConnection->setInformationView(ui->information);
        dbConnection->loadDatabaseList();

        connect(ui->databaseList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListViewDoubleClicked(const QModelIndex)));
    }
}

void MainWindow::onListViewDoubleClicked(const QModelIndex index) {
    dbConnection->onListViewDoubleClicked(index);
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
//                qDebug() << "FieldName: " << localRecord.fieldName(currentPos);
//                qDebug() << "Field: " << localRecord.field(currentPos).name();
//                qDebug() << "Field: " << localRecord.field(currentPos).type();
//                qDebug() << "Field: " << localRecord.field(currentPos).typeID();
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

void MainWindow::newConnectionData(QObjectData& connectionData) {
    qDebug() << "BESTÄTIGT!";
}

void MainWindow::onListViewClicked(const QModelIndex index) {
    qDebug() << "SINGLE CLICK!";
    /*
    ui->databaseList->setItemsExpandable(true);
    QStandardItem* currentItem = databaseCollection->itemFromIndex(index);

    if (0 == currentItem) {
        return;
    }

    // habe kein parent => ist eine datenbank
    if (0 == currentItem->parent()) {
        handleDatabaseClicked(index);
    } else {
        handleTableClicked(currentItem);
    }
    */
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
