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
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QElapsedTimer>
#include <QNetworkRequest>
#include <QDateTime>
#include <QList>
#include <QUrl>
#include <QCompleter>
#include <QWizard>
#include <QWizardPage>
#include <QMenu>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QToolButton>
#include <QXmlStreamReader>
#include <QDir>
#include <QTimer>
#include <QProgressDialog>
#include <QProcess>

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
    ui->btnQuerySave->hide();

    this->currentQueryRequests = new QMap<int, QString>;
    this->updateManager = new UpdateManager(this);

    this->ui->queryResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QMenuBar* releaseNotesBar = new QMenuBar(ui->menuBar);
    QAction* releaseNotes = new QAction(QIcon(":/icons/release_notes.png"), "");
    releaseNotesBar->addAction(releaseNotes);
    releaseNotesBar->setVisible(true);
    this->ui->menuBar->setCornerWidget(releaseNotesBar);

/*
    QToolButton *tb = new QToolButton();
    tb->setText("+");
    // Add empty, not enabled tab to tabWidget
    ui->tabWidget->addTab(new QLabel("Add tabs by pressing \"+\""), QString());
    ui->tabWidget->setTabEnabled(0, false);
    // Add tab button to current tab. Button will be enabled, but tab -- not
    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::LeftSide, tb);
*/

    ui->tabWidget->clear();
    QIcon icon(":/icons/add_schatten.png");
    ui->tabWidget->addTab(new QLabel(""), icon, QString(""));
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setMovable(false);
    auto tabBar = ui->tabWidget->tabBar();
    tabBar->tabButton(0, QTabBar::RightSide)->hide();

    newTab();

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onChangeTab(int)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    ui->menuVerbinden->menuAction()->setVisible(false);
    ui->menuVerbinden->hideTearOffMenu();

    loadConnectionInfos();

    ui->databaseList->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->btnQueryExecute, SIGNAL(clicked(bool)), this, SLOT(onExecuteQueryClicked()));
    connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionEditConnection, SIGNAL(triggered(bool)), this, SLOT(openConnectionManagerWindow()));
    connect(ui->actionNewConnection, SIGNAL(triggered(bool)), this, SLOT(openNewConnectionWindow()));
    connect(ui->menuVerbinden, SIGNAL(triggered(QAction*)), this, SLOT(onEstablishNewConnection(QAction*)));
    connect(&this->connections, SIGNAL(changed()), this, SLOT(createConnectionSubMenu()));
    connect(&this->connections, SIGNAL(changed()), this, SLOT(saveConnectionInfos()));
    connect(ui->btnQuerySave, SIGNAL(clicked()), this, SLOT(saveQuery()));
    connect(ui->btnQueryLoad, SIGNAL(clicked()), this, SLOT(loadQuery()));
    connect(ui->actionAboutDBManager, SIGNAL(triggered()), this, SLOT(showAboutText()));

    connect(ui->actionCheckForUpdates, SIGNAL(triggered()), this->updateManager, SLOT(checkUpdateAvailable()));

    this->updateManager->checkUpdateAvailable(false);
}

void MainWindow::onChangeTab(int index) {
//    qDebug() << "OnChangeTab " << index;
    if (index == this->ui->tabWidget->count() - 1) {
        newTab();
    } else {
//        this->currentTabIndex = index;
        this->handleChangedQueryRequest();
    }
}

void MainWindow::closeTab(int index) {
//    qDebug() << "Tabs: " << this->ui->tabWidget->count();
//    qDebug() << "Index: " << index;

    if (2 < this->ui->tabWidget->count()
        && index < (this->ui->tabWidget->count() - 1)
    ) {
        this->ui->tabWidget->removeTab(index);
//        this->currentTabIndex = this->currentTabIndex - 1;

        if (this->currentQueryRequests->contains(index)) {
//            this->currentQueryRequests->remove(this->currentTabIndex);
            this->currentQueryRequests->remove(ui->tabWidget->currentIndex());
        }

        // reindex map
        QMap<int, QString>* newMap = new QMap<int, QString>();
        QMapIterator<int, QString> mapIterator(*this->currentQueryRequests);
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            (*newMap)[index] = mapIterator.value();
            index++;
        }
        this->currentQueryRequests = newMap;
    }
}

void MainWindow::newTab() {
    int position = ui->tabWidget->count() - 1;

    TextEdit* textEdit = new TextEdit(this);
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(handleChangedQueryRequest()));

    ui->tabWidget->insertTab(position, textEdit, QString(tr("New tab")));
    ui->tabWidget->setCurrentIndex(position);
}

void MainWindow::handleChangedQueryRequest() {
//    QString query = ui->queryRequest->toPlainText();
//    TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->currentTabIndex));
    TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->ui->tabWidget->currentIndex()));
    QString query = textEdit->toPlainText();

//    if (!query.isEmpty()
//        && ((this->currentQueryRequests->contains(this->currentTabIndex)
//            && query != (*this->currentQueryRequests)[this->currentTabIndex])
//            || false == this->currentQueryRequests->contains(this->currentTabIndex)
//        )
//    ) {
    if (!query.isEmpty()
        && ((this->currentQueryRequests->contains(this->ui->tabWidget->currentIndex())
            && query != (*this->currentQueryRequests)[this->ui->tabWidget->currentIndex()])
            || false == this->currentQueryRequests->contains(this->ui->tabWidget->currentIndex())
        )
    ) {
        markAsUnsaved(true);
        showQuerySaveIcon();
    } else {
        markAsUnsaved(isUnsaved());
        hideQuerySaveIcon();
    }
}

void MainWindow::showQuerySaveIcon() {
    ui->btnQuerySave->show();
}

void MainWindow::hideQuerySaveIcon() {
    ui->btnQuerySave->hide();
}

void MainWindow::saveQuery() {
//    TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->currentTabIndex));
    TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->ui->tabWidget->currentIndex()));
    QString query = textEdit->toPlainText();

    QString queryFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Current Query"),
        "",
        tr("Sql Files (*.sql);;All Files (*)")
    );

    if (queryFileName.isEmpty()) {
        return;
    } else {
//        qDebug() << "Save in file: " << queryFileName;
        QFile file(queryFileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(
                this,
                tr("Unable to open file"),
                file.errorString()
            );
            return;
        }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_5);
        out << query;

//        (*this->currentQueryRequests)[this->currentTabIndex] = query;
        (*this->currentQueryRequests)[this->ui->tabWidget->currentIndex()] = query;
        this->markAsUnsaved(false);
        this->hideQuerySaveIcon();
    }
}

void MainWindow::loadQuery() {
    QString queryFileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Address Book"),
        "",
        tr("Sql Files (*.sql);;All Files (*)")
    );

    if (queryFileName.isEmpty()) {
        return;
    } else {
        QFile file(queryFileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(
                this,
                tr("Unable to open file"),
                file.errorString()
            );
            return;
        }
        QString query;
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);
        in >> query;

//        (*this->currentQueryRequests)[this->currentTabIndex] = query;
        (*this->currentQueryRequests)[this->ui->tabWidget->currentIndex()] = query;
        hideQuerySaveIcon();
        markAsUnsaved(false);
//        TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->currentTabIndex));
        TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->ui->tabWidget->currentIndex()));
        textEdit->setText(query);
    }
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

    if (0 < connections.size()) {
        ui->menuVerbinden->menuAction()->setVisible(true);
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

    if (!QFileInfo::exists("DBManager.xml")) {
        return;
    }

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

    connect(ui->btnQueryResultSave, SIGNAL(clicked()), this->dbConnection, SLOT(saveQueryResultChanges()));
    connect(ui->btnQueryResultCancel, SIGNAL(clicked()), this->dbConnection, SLOT(cancelQueryResultChanges()));

    dbConnection->setDatabaseListView(ui->databaseList);
    dbConnection->setQueryResultView(ui->queryResult);
//    dbConnection->setQueryRequestView(ui->queryRequest);
//    TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->currentTabIndex));
    TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(this->ui->tabWidget->currentIndex()));
    dbConnection->setQueryRequestView(textEdit);
    dbConnection->setInformationView(ui->information);
    dbConnection->loadDatabaseList();

    if ("SQLITE" == connectionInfo->getConnectionType()) {
        setWindowTitle(QString("%1 - %2").arg(connectionInfo->getDatabaseName()).arg("Database Manager"));
    } else {
        setWindowTitle(QString("%1@%2 - %3").arg(Defaults::Resolver::resolve("user", connectionInfo->getUser())).arg(connectionInfo->getHost()).arg("Database Manager"));
    }

    return dbConnection;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return && event->modifiers() == Qt::ControlModifier) {
        this->onExecuteQueryClicked();
    }
}

void MainWindow::handleConnectionError(QString errorMessage) {
    this->ui->information->append(errorMessage);
}

void MainWindow::onExecuteQueryClicked() {
//    qDebug() << "Current Tab Index von QTabWidget::currentIndex: " << ui->tabWidget->currentIndex();

    TextEdit* textEdit = qobject_cast<TextEdit*>(ui->tabWidget->widget(ui->tabWidget->currentIndex()));

//    qDebug() << "MainWindow::onExecuteQueryClicked textEdit: " << textEdit;

    QString queryString = textEdit->toPlainText();
//    QString queryString = ui->queryRequest->toPlainText();

    if (nullptr == dbConnection) {
        this->ui->information->append(tr("No Connection established!"));
        return;
    }
    QSqlQuery query = dbConnection->sendQuery(queryString);

    QStandardItemModel* queryResultModel = new QStandardItemModel;

    if (query.isActive()) {
        QSqlRecord localRecord = query.record();
//        qDebug() << "Local Record: " << localRecord;
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

/**
  Public slot to get unsaved state for global application.

 * @brief MainWindow::markAsUnsaved
 * @param unsaved
 */
void MainWindow::markAsUnsaved(bool unsaved) {
    this->setUnsaved(unsaved);
}

bool MainWindow::isUnsaved() const {
    return unsaved;
}

void MainWindow::setUnsaved(const bool unsaved) {
    this->unsaved = unsaved;
}

void MainWindow::showAboutText() {
    QMessageBox messageBox(this);
    messageBox.setIcon(QMessageBox::Icon::Information);
    messageBox.setWindowTitle(tr("DBManager - about"));
    messageBox.setTextFormat(Qt::RichText);
    messageBox.setText(tr("DBManager in version %1.<br />This Program is Freeware and OpenSource.").arg(Application::BuildNo));
    messageBox.setInformativeText(tr("Future information and Projects under <a href='https://www.byte-artist.de'>www.byte-artist.de</a><br /><br />Found issues can tracked here: <a href='https://bitbucket.org/mastercad/dbmanager/issues'>DBManager Issue Tracker</a>"));

    messageBox.exec();
}

MainWindow::~MainWindow() {
    delete currentQueryRequests;
    delete updateManager;
    delete ui;
}
