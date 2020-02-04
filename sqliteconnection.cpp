#include "sqliteconnection.h"

#include <QSqlRecord>
#include <QSqlField>
#include <QHeaderView>
#include <QFileInfo>

#include <QDebug>

SqliteConnection::SqliteConnection() {

}

void SqliteConnection::init() {
    database = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    database.setDatabaseName(this->getDatabasePath());
    QFileInfo fileInfo(this->getDatabasePath());
    activeDatabaseName = fileInfo.baseName();
}

void SqliteConnection::loadDatabaseList() {
    database.open();
    qDebug() << "SqliteConnection::loadDatabaseList";
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
}

void SqliteConnection::loadTablesList() {
    QSqlQuery query(database);
    query.exec("SELECT name FROM sqlite_master WHERE type='table';");

    if (query.isActive()) {
        emit databaseCollection->layoutAboutToBeChanged();
        databaseCollection->item(0, 0)->removeRows(0, databaseCollection->item(0)->rowCount());
        qDebug() << "Gefundene Tabellen: " << query.size();
        while (query.next()) {
            QString tableName = query.value(0).toString();
            QStandardItem* item = new QStandardItem(tableName);
            item->setToolTip(tableName);
            item->setEditable(false);
            databaseCollection->item(0, 0)->appendRow(item);
        }
        emit databaseCollection->layoutChanged();
    } else {
        qDebug() << "Problem Show Databases!";
        qDebug() << database.lastError();
    }
}

void SqliteConnection::handleTableClicked(QModelIndex index) {
    // 172.19.2.2
//    activeDatabase = QString(item->parent()->text());
    QStandardItem* item = databaseCollection->itemFromIndex(index);
    activeTableName = item->text();

    // make sure between multiple databases, the correct database to clicked table is connected
//    switchDatabase(activeDatabase);

    qDebug() << "************* TABLE " << activeTableName << " clicked!";
//    qDebug() << "datenbank name:" << activeDatabase;

    delete queryResultModel;
    queryResultModel = new QStandardItemModel;

    QSqlQuery query = sendQuery("SELECT * FROM "+activeTableName);

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

    queryResultView->setModel(queryResultModel);
    queryResultView->resizeColumnsToContents();
//  statusBar()->showMessage("connected!");
}

void SqliteConnection::onListViewDoubleClicked(const QModelIndex index) {
    qDebug() << "DOUBLE CLICK!";
    databaseListView->setItemsExpandable(true);
    QStandardItem* currentItem = databaseCollection->itemFromIndex(index);

    if (0 == currentItem) {
        return;
    }

    handleTableClicked(index);
}

void SqliteConnection::setDatabaseName(QString databaseName) {
    this->databaseName = databaseName;
}

QString SqliteConnection::getDatabaseName() const {
    return this->databaseName;
}

void SqliteConnection::setDatabasePath(QString databasePath) {
    this->databasePath = databasePath;
}

QString SqliteConnection::getDatabasePath() const {
    return this->databasePath;
}
