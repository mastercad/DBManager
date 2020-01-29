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
    setWindowTitle(QString("%1").arg("Database Manager"));

    /*
    TextEdit* completingTextEdit = new TextEdit;
    */
    completer = new QCompleter(this);
//    completer->setModel(modelFromFile(":/resources/wordlist.txt"));
    completer->setModel(databaseCollection);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    ui->queryRequest->setCompleter(completer);
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
    connect(ui->actionNewConnection, SIGNAL(triggered(bool)), this, SLOT(openNewConnectionWindow()));
}

void MainWindow::openNewConnectionWindow() {
    /*
    QWizard wizard;
    QWizardPage* page = new QWizardPage();

    QPushButton* btnMySql = new QPushButton();
    QPixmap pixmap(":/logos/mysql.png");
    btnMySql->setIcon(pixmap);
    btnMySql->setIconSize(QSize(32, 32));

    page->setTitle("New Connection Wizard");
    page->setSubTitle("Select Connection Type:");
    page->registerField("ConnectionType", btnMySql);
    wizard.addPage(page);
    wizard.exec();
    */

    NewConnectionWizard* wizard = new NewConnectionWizard;
    if (wizard->exec()) {
        qDebug() << "New Connection established!";
    }
/*
    newConnectionWindow = new NewConnectionWindow();
//    connectionWithLoginDialogWindow->setHostName(hostName);

    if (NewConnectionWindow::Accepted == newConnectionWindow->exec()) {
        dbConnection = QSqlDatabase::addDatabase("QMYSQL", "mysql");
//        dbConnection.setHostName(connectionWithLoginDialogWindow->getHostName());
//        dbConnection.setDatabaseName(connectionWithLoginDialogWindow->getDatabase());
//        dbConnection.setPort(connectionWithLoginDialogWindow->getPort().toInt());
//        dbConnection.setUserName(connectionWithLoginDialogWindow->getUserName());
//        dbConnection.setPassword(connectionWithLoginDialogWindow->getPassword());

        if (!dbConnection.open()) {
            qDebug() << dbConnection.lastError().text();
        } else {
//            setWindowTitle(QString("%1@%2[*] - %3").arg(connectionWithLoginDialogWindow->getUserName()).arg(connectionWithLoginDialogWindow->getHostName()).arg("Database Manager"));

            QSqlQuery query(dbConnection);
            query.setForwardOnly(true);
            query.exec("SHOW DATABASES");

            if (query.isActive()) {
                databaseCollection = new QStandardItemModel(query.size(), 1, this);
                unsigned int row = 0;

                while (query.next()) {
                    QStandardItem* item = new QStandardItem(query.value(0).toString());
                    item->setEditable(false);
                    item->setToolTip(databaseName);
                    databaseCollection->setItem(row, 0, item);
                    ++row;
                    qDebug() << query.value(0).toString();
                }
                ui->databaseList->header()->hide();
                ui->databaseList->setModel(databaseCollection);
                completer->setModel(databaseCollection);
                connect(ui->databaseList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListViewDoubleClicked(const QModelIndex)));
                connect(ui->databaseList, SIGNAL(clicked(QModelIndex)), this, SLOT(onListViewClicked(const QModelIndex)));
            }

            collectTableInformations();
        }
    }
*/
}

void MainWindow::onExecuteQueryClicked() {
    QString queryString = ui->queryRequest->toPlainText();
    QSqlQuery query = sendQuery(queryString);

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

void MainWindow::collectTableInformations() {
    QSqlQuery query(dbConnection);
    switchDatabase("information_schema");

//    QList<QString> ignoredTables = {"mysql", "information_schema", "performance_schema", "sys"};

    // foreign key from
    // foreign key to
    // foreign key id

    // select foreign keys
    query.exec("SELECT * FROM INNODB_FOREIGN");
    query.exec("SELECT * FROM INNODB_FOREIGN_COLS");

    query.exec("SELECT * FROM STATISTICS");

    // detailierte informationen über keys der tabellen
    query.exec("SELECT * FROM KEY_COLUMN_USAGES");

    query.exec("SELECT * FROM TABLES");

    query.exec("SELECT * FROM TABLE_CONSTRAINTS");

    query.exec("SELECT * FROM COLUMNS");

    query.exec("SELECT * FROM SCHEMATA");

    query.exec("SELECT * FROM KEYWORDS");
    keywords.clear();
    if (query.isActive()) {
        while (query.next()) {
            QSqlRecord currentRecord = query.record();
            keywords.insert(currentRecord.field(0).value().toString(), currentRecord.field(1).value().toBool());
        }
    }
    qDebug() << keywords;

    switchDatabase(databaseName);
}

void MainWindow::onListViewDoubleClicked(const QModelIndex index) {
    qDebug() << "DOUBLE CLICK!";
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
}

void MainWindow::handleDatabaseClicked(QModelIndex index) {
    qDebug() << "############# DATABASE Clicked!";

    QString databaseName = databaseCollection->data(index).toString();
    if (!switchDatabase(databaseName)) {
        return;
    }

    QSqlQuery query = sendQuery("SHOW TABLES");

    int row = index.row();
    int column = index.column();

    // 172.19.2.2
    if (query.isActive()) {
        emit databaseCollection->layoutAboutToBeChanged();
        databaseCollection->item(row, column)->removeRows(0, databaseCollection->item(row)->rowCount());
        qDebug() << "Gefundene Tabellen: " << query.size();
        while (query.next()) {
            QString tableName = query.value(0).toString();
            QStandardItem* item = new QStandardItem(tableName);
            item->setToolTip(tableName);
            item->setEditable(false);
            databaseCollection->item(row, column)->appendRow(item);
        }
        emit databaseCollection->layoutChanged();
    }
}

QSqlQuery MainWindow::sendQuery(QSqlQuery query) {

    QElapsedTimer timer;
    timer.start();
    query.exec();
    lastQueryTime = timer.elapsed();

    if (query.isActive()) {
        ui->information->append(QString(query.lastQuery()+" in %1ms.").arg(lastQueryTime));
    } else {
        ui->information->append(query.lastError().text());
    }
    return query;
}

QSqlQuery MainWindow::sendQuery(QString queryString) {

    QSqlQuery query(dbConnection);
    query.prepare(queryString);
    return sendQuery(query);
}

bool MainWindow::switchDatabase(QString databaseName) {
    qDebug() << databaseName;

    if (dbConnection.isOpen()) {
        dbConnection.close();
    }

    dbConnection.setDatabaseName(databaseName);

    if (!dbConnection.open()) {
        qDebug() << dbConnection.lastError();
        return false;
    }
    return true;
}

void MainWindow::handleTableClicked(QStandardItem* item) {
    // 172.19.2.2
    activeDatabase = QString(item->parent()->text());
    activeTable = item->text();

    // make sure between multiple databases, the correct database to clicked table is connected
    switchDatabase(activeDatabase);

    qDebug() << "************* TABLE " << activeTable << " clicked!";
    qDebug() << "datenbank name:" << activeDatabase;

    delete queryResultModel;
    queryResultModel = new QStandardItemModel;

    QSqlQuery query = sendQuery("SELECT * FROM "+activeTable);

//    qDebug() << "Time: " << QDateTime::fromMSecsSinceEpoch(timer.elapsed()).toUTC().toString("hh:mm:ss");

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
    statusBar()->showMessage("connected!");
}

void MainWindow::onQueryResultHeaderClicked(QStandardItem* item) {

}

QString MainWindow::generateLastExecutedQuery(const QSqlQuery& query) {
    /*
    QString sql = query.executedQuery();
    int nbBindValues = query.boundValues().size();

    for(int i = 0, j = 0; j < nbBindValues;) {
        int s = sql.indexOf(QLatin1Char('\''), i);
        i = sql.indexOf(QLatin1Char('?'), i);
        if (i < 1) {
            break;
        }

        if (s < i && s > 0) {
            i = sql.indexOf(QLatin1Char('\''), s + 1) + 1;
            if(i < 2) {
                break;
            }
        } else {
            const QVariant &var = query.boundValue(j);
            QSqlField field(QLatin1String(""), var.type());
            if (var.isNull()) {
                field.clear();
            } else {
                field.setValue(var);
            }
            QString formatV = query.driver()->formatValue(field);
            sql.replace(i, 1, formatV);
            i += formatV.length();
            ++j;
        }
    }

    return sql;
    */
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());

    while (it.hasNext()) {
        it.next();
        str.replace(it.key(), it.value().toString());
    }
    return str;
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
    delete queryResultModel;
}
