#include "mysqlconnection.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

MysqlConnection::MysqlConnection() {

}

void MysqlConnection::setHost(QString host) {
    this->host = host;
}

QString MysqlConnection::getHost() const {
    return this->host;
}

void MysqlConnection::setUser(QString user) {
    this->user = user;
}

QString MysqlConnection::getUser() const {
    return this->user;
}

void MysqlConnection::setPassword(QString password) {
    this->password = password;
}

QString MysqlConnection::getPassword() const {
    return this->password;
}

void MysqlConnection::setPort(uint port) {
    this->port = port;
}

uint MysqlConnection::getPort() const {
    return this->port;
}

void MysqlConnection::setDatabaseName(QString databaseName) {
    this->databaseName = databaseName;
}

QString MysqlConnection::getDatabaseName() const {
    return this->databaseName;
}

void MysqlConnection::init() {
    database = QSqlDatabase::addDatabase("QMYSQL", "mysql");
    database.setHostName(this->getHost());
    database.setPort(this->getPort());
    database.setUserName(this->getUser());
    database.setPassword(this->getPassword());
}

void MysqlConnection::onListViewDoubleClicked(const QModelIndex index) {
    qDebug() << "DOUBLE CLICK!";
    databaseListView->setItemsExpandable(true);
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

void MysqlConnection::collectTableInformations() {
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

void MysqlConnection::handleTableClicked(QStandardItem* item) {
    // 172.19.2.2
    activeDatabaseName = QString(item->parent()->text());
    activeTableName = item->text();

    // make sure between multiple databases, the correct database to clicked table is connected
    switchDatabase(activeDatabaseName);

    qDebug() << "************* TABLE " << activeTableName << " clicked!";
    qDebug() << "datenbank name:" << activeDatabaseName;

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
//    statusBar()->showMessage("connected!");
}

bool MysqlConnection::switchDatabase(QString databaseName) {
    qDebug() << databaseName;

    if (database.isOpen()) {
        database.close();
    }

    database.setDatabaseName(databaseName);

    if (!database.open()) {
        qDebug() << database.lastError();
        return false;
    }
    return true;
}
