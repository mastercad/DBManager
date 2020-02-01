#include "sqliteconnection.h"

#include <QSqlRecord>
#include <QSqlField>

#include <QDebug>

SqliteConnection::SqliteConnection()
{

}

void SqliteConnection::init() {
    database = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    database.setDatabaseName(this->getDatabasePath());
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
