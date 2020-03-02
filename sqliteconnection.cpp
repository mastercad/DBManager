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
    this->getConnectionInfo()->setDatabaseName(activeDatabaseName);
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

void SqliteConnection::showDatabaseContextMenu(const QPoint& position) {

}

void SqliteConnection::truncateTable() {

}

void SqliteConnection::deleteTable() {

}

void SqliteConnection::renameTable() {

}

void SqliteConnection::showResultTableContextMenu(const QPoint& position) {

}

void SqliteConnection::copyResultViewSelection() {

}

void SqliteConnection::deleteResultViewSelection() {

}

void SqliteConnection::pasteToResultView() {

}

void SqliteConnection::insertNullToResultView() {

}

void SqliteConnection::cellEntered(int row, int cell) {

}

void SqliteConnection::dataChanged(const QModelIndex& fromIndex, const QModelIndex& toIndex, const QVector<int>& role) {

}

void SqliteConnection::saveQueryResultChanges() {

}

void SqliteConnection::cancelQueryResultChanges() {

}

void SqliteConnection::handleTableClicked(QModelIndex index) {
    QStandardItem* item = databaseCollection->itemFromIndex(index);
    activeTableName = item->text();

    delete queryResultModel;

    queryResultModel = new QSqlRelationalTableModel(this, database);
    queryResultModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    queryResultModel->setTable(activeTableName);
    queryResultModel->select();

    queryResultView->setModel(queryResultModel);
    queryResultView->resizeColumnsToContents();
}

void SqliteConnection::onListViewDoubleClicked(const QModelIndex index) {
    databaseListView->setItemsExpandable(true);
    QStandardItem* currentItem = databaseCollection->itemFromIndex(index);

    if (nullptr == currentItem) {
        return;
    }

    handleTableClicked(index);
}

