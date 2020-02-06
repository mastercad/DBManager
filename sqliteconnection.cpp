#include "sqliteconnection.h"

#include <QSqlRecord>
#include <QSqlField>
#include <QHeaderView>
#include <QFileInfo>

#include <QDebug>

void SqliteConnection::init() {
    QSqlDatabase::removeDatabase("sqlite");
    database = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    database.setDatabaseName(this->getConnectionInfo()->getDatabasePath());
    QFileInfo fileInfo(this->getConnectionInfo()->getDatabasePath());
    activeDatabaseName = fileInfo.baseName();
}

void SqliteConnection::loadDatabaseList() {
    database.open();
    databaseCollection = new QStandardItemModel(1, 1);

    QStandardItem* item = new QStandardItem(activeDatabaseName);
    item->setEditable(false);
    item->setToolTip(activeDatabaseName);
    databaseCollection->setItem(0, 0, item);
    getDatabaseListView()->header()->hide();
    getDatabaseListView()->setModel(databaseCollection);

    completer = new QCompleter();
//    completer->setModel(modelFromFile(":/resources/wordlist.txt"));
    completer->setModel(databaseCollection);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    getQueryRequestView()->setCompleter(completer);

    loadTablesList();

    connect(getDatabaseListView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListViewDoubleClicked(const QModelIndex)));
}

void SqliteConnection::loadTablesList() {
    QSqlQuery query(database);
    query.exec("SELECT name FROM sqlite_master WHERE type='table';");

    if (query.isActive()) {
        emit databaseCollection->layoutAboutToBeChanged();
        databaseCollection->item(0, 0)->removeRows(0, databaseCollection->item(0)->rowCount());
        while (query.next()) {
            QString tableName = query.value(0).toString();
            QStandardItem* item = new QStandardItem(tableName);
            item->setToolTip(tableName);
            item->setEditable(false);
            databaseCollection->item(0, 0)->appendRow(item);
        }
        emit databaseCollection->layoutChanged();
    } else {
        qWarning() << "Problem Show Databases!";
        qWarning() << database.lastError();
    }
}

void SqliteConnection::handleTableClicked(QModelIndex index) {
    // 172.19.2.2
//    activeDatabase = QString(item->parent()->text());
    QStandardItem* item = databaseCollection->itemFromIndex(index);
    activeTableName = item->text();

    delete queryResultModel;
    queryResultModel = new QStandardItemModel;

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

void SqliteConnection::onListViewDoubleClicked(const QModelIndex index) {
    databaseListView->setItemsExpandable(true);
    QStandardItem* currentItem = databaseCollection->itemFromIndex(index);

    if (0 == currentItem) {
        return;
    }

    handleTableClicked(index);
}

