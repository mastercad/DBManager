#include "connectionmanager.h"
#include "ui_connectionmanager.h"

#include "connectioninfo.h"
#include "mysqlconnectioneditwidget.h"
#include "sqliteconnectioneditwidget.h"

#include <QStandardItem>
#include <QModelIndex>
#include <QVariant>
#include <QLineEdit>
#include <QLabel>

#include <QDebug>

/**
  * Class for Manage Connection Edit, Load and Save.
  */

//ConnectionManager::ConnectionManager(MainWindow *parent, QMap<QString, QMap<QString, ConnectionInfo*> >* connections) :
ConnectionManager::ConnectionManager(MainWindow *parent, Connections* connections) :
    QDialog(parent),
    ui(new Ui::ConnectionManager),
    parent(parent),
    connections(connections)
{
    ui->setupUi(this);

    loadConnections();

    connect(ui->btnNew, SIGNAL(clicked(bool)), parent, SLOT(openNewConnectionWindow()));
    connect(ui->btnSave, SIGNAL(clicked(bool)), this, SLOT(onSaveConnectionsClicked()));
    connect(ui->btnCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(this->connections, SIGNAL(changed()), this, SLOT(loadConnections()));
//    connect(this->connections, SIGNAL(changed()), this, SLOT(disableSaveButton())); macht glaube keinen sinn, da das changed event durch save im widget ausgelöst wird.
}

void ConnectionManager::loadConnections() {
    QMap<QString, QMap<QString, ConnectionInfo*> >::iterator typeIterator = connections->begin();
    model = new QStandardItemModel;

    while (typeIterator != connections->end()) {
        QMapIterator<QString, ConnectionInfo*> connectionsIterator(typeIterator.value());
        QStandardItem* categoryItem = new QStandardItem(typeIterator.key());
        categoryItem->setEditable(false);
        categoryItem->setToolTip(typeIterator.key());
        model->appendRow(categoryItem);

        while(connectionsIterator.hasNext()) {
            connectionsIterator.next();
            QStandardItem* connectionItem = new QStandardItem(connectionsIterator.value()->getConnectionName());
            ConnectionInfo* connectionInfo = connectionsIterator.value();
            connectionItem->setData(QVariant::fromValue(connectionInfo), Qt::UserRole + 1);
            if ("MYSQL" == connectionInfo->getConnectionType()) {
                connectionItem->setToolTip(QString("%1@%2").arg(connectionInfo->getUser()).arg(connectionInfo->getHost()));
            } else {
                connectionItem->setToolTip(QString("%1").arg(connectionInfo->getDatabasePath()));
            }
            connectionItem->setEditable(false);
            categoryItem->appendRow(connectionItem);
        }
        ++typeIterator;
    }

    connect(this->ui->connectionsTreeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onConnectionDoubleClicked(const QModelIndex)));
    connect(this, SIGNAL(invalid(bool)), this, SLOT(onValidationState(bool)));

    this->ui->connectionsTreeView->header()->hide();
    this->ui->connectionsTreeView->setModel(model);
}

void ConnectionManager::onValidationState(bool state) {
    this->ui->btnSave->setDisabled(state);
}

void ConnectionManager::onConnectionDoubleClicked(const QModelIndex &index) {
    QStandardItem* currentItem = model->itemFromIndex(index);

    if (nullptr != currentItem
        && nullptr != currentItem->parent()
    ) {
        currentConnectionInfo = index.data(Qt::UserRole + 1).value<ConnectionInfo*>();
        if ("MYSQL" == currentConnectionInfo->getConnectionType()) {
            showConnectionEditForMysql();
        } else if ("SQLITE" == currentConnectionInfo->getConnectionType()) {
            showConnectionEditForSqlite();
        }
    }
}

void ConnectionManager::showConnectionEditForMysql() {
    MysqlConnectionEditWidget* editWidget = new MysqlConnectionEditWidget(this, connections);
    delete lastEditWidget;
//    this->ui->btnSave->setDisabled(true);
    this->ui->mainContainer->addWidget(editWidget);
    editWidget->parseConnectionInfo(currentConnectionInfo);
    lastEditWidget = editWidget;
    connect(this->ui->btnSave, SIGNAL(clicked(bool)), editWidget, SLOT(save()));
}

void ConnectionManager::showConnectionEditForSqlite() {
    SqliteConnectionEditWidget* editWidget = new SqliteConnectionEditWidget(this, connections);
    delete lastEditWidget;
//    this->ui->btnSave->setDisabled(true);
    this->ui->mainContainer->addWidget(editWidget);
    editWidget->parseConnectionInfo(currentConnectionInfo);
    lastEditWidget = editWidget;
    connect(this->ui->btnSave, SIGNAL(clicked(bool)), editWidget, SLOT(save()));
}

void ConnectionManager::onSaveConnectionsClicked() {
}

ConnectionManager::~ConnectionManager()
{
    delete ui;
}
