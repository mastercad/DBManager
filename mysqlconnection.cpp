 #include "mysqlconnection.h"

#include "defaults.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QModelIndex>

#include <QDebug>

void MysqlConnection::init() {
    if (QSqlDatabase::contains("mysql")) {
        QSqlDatabase::removeDatabase("mysql");
    }
    database = QSqlDatabase::addDatabase("QMYSQL", "mysql");
    database.setHostName(this->getConnectionInfo()->getHost().isEmpty() ? Defaults::MYSQL::HOST : this->getConnectionInfo()->getHost());
    database.setPort(0 >= this->getConnectionInfo()->getPort() ? Defaults::MYSQL::PORT : this->getConnectionInfo()->getPort());
    database.setUserName(this->getConnectionInfo()->getUser().isEmpty() ? Defaults::MYSQL::USER : this->getConnectionInfo()->getUser());
    database.setPassword(this->getConnectionInfo()->getPassword().isEmpty() ? Defaults::MYSQL::PASSWORD : this->getConnectionInfo()->getPassword());
    database.setDatabaseName(this->getConnectionInfo()->getDatabaseName());
}

void MysqlConnection::loadDatabaseList() {
    database.open();
    QSqlQuery query(database);
//            query.setForwardOnly(true);
    query.exec("SHOW DATABASES");

    if (query.isActive()) {
        databaseCollection = new QStandardItemModel(query.size(), 1);
        int row = 0;

        while (query.next()) {
            QStandardItem* item = new QStandardItem(query.value(0).toString());
            item->setEditable(false);
            item->setToolTip(activeDatabaseName);
            databaseCollection->setItem(row, 0, item);
            ++row;
        }
        getDatabaseListView()->header()->hide();
        getDatabaseListView()->setModel(databaseCollection);

        completer = new QCompleter();
    //    completer->setModel(modelFromFile(":/resources/wordlist.txt"));
        completer->setModel(databaseCollection);
        completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setWrapAround(false);
        getQueryRequestView()->setCompleter(completer);

//        completer->setModel(databaseCollection);
//        QObject::connect(getDatabaseListView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListViewDoubleClicked(const QModelIndex)));
//        connect(getDatabaseListView(), SIGNAL(clicked(QModelIndex)), this, SLOT(onListViewClicked(const QModelIndex)));

        collectTableInformations();

        connect(getDatabaseListView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListViewDoubleClicked(const QModelIndex)));
    } else {
        qWarning() << "Problem Show Databases!!!!!";
        qWarning() << database.lastError();
        emit connectionError(database.lastError().text());
    }
}

void MysqlConnection::onListViewDoubleClicked(const QModelIndex index) {
    databaseListView->setItemsExpandable(true);
    QStandardItem* currentItem = databaseCollection->itemFromIndex(index);

    if (nullptr == currentItem) {
        return;
    }

    // habe kein parent => ist eine datenbank
    if (nullptr == currentItem->parent()) {
        handleDatabaseClicked(index);
    } else {
        handleTableClicked(currentItem);
    }
}

void MysqlConnection::collectTableInformations() {
    database.open();
    QSqlQuery query(database);
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
    /*
    keywords.clear();
    if (query.isActive()) {
        while (query.next()) {
            QSqlRecord currentRecord = query.record();
            keywords.insert(currentRecord.field(0).value().toString(), currentRecord.field(1).value().toBool());
        }
    }
    qDebug() << keywords;
    */
//    switchDatabase(databaseName);
}

void MysqlConnection::handleDatabaseClicked(QModelIndex index) {
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

void MysqlConnection::handleTableClicked(QStandardItem* item) {
    // 172.19.2.2
    activeDatabaseName = QString(item->parent()->text());
    activeTableName = item->text();

    // make sure between multiple databases, the correct database to clicked table is connected
    switchDatabase(activeDatabaseName);

    delete queryResultModel;
    queryResultModel = new QStandardItemModel;

    database.open();
    QSqlQuery query = sendQuery("SELECT * FROM "+activeTableName);

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
    queryResultView->setModel(queryResultModel);
    queryResultView->resizeColumnsToContents();
}

bool MysqlConnection::switchDatabase(QString databaseName) {
    if (database.isOpen()) {
        database.close();
    }

    database.setDatabaseName(databaseName);

    if (!database.open()) {
        qWarning() << database.lastError();
        return false;
    }
    return true;
}
