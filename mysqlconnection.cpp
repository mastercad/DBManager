 #include "mysqlconnection.h"

#include "defaults.h"

#include <QStandardItemModel>
#include <QSqlRelationalTableModel>
#include <QGuiApplication>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QModelIndex>
#include <QMimeData>
#include <QList>
#include <QMenu>
#include <QFont>

#include <QCoreApplication>

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

void MysqlConnection::showDatabaseContextMenu(const QPoint& position) {
    QString clickedItem = this->databaseCollection->data(this->databaseListView->indexAt(position)).toString();
    currentContextMenuItem = this->databaseCollection->itemFromIndex(this->databaseListView->indexAt(position));

    QMenu menu;
//    QAction actionDeleteTable(tr("Delete Table"), this);
//    QAction actionDeleteTable(parent->tr("Delete Table"), parent);
    QAction actionDeleteTable(QCoreApplication::translate("MysqlConnection", "Delete Table"), parent);
    menu.addAction(&actionDeleteTable);
    QAction actionTruncateTable(QCoreApplication::translate("MysqlConnection", "Truncate Table"));
    menu.addAction(&actionTruncateTable);
    QAction actionRenameTable(QCoreApplication::translate("MysqlConnection", "Rename Table"));
    menu.addAction(&actionRenameTable);

    connect(&actionTruncateTable, SIGNAL(triggered()), this, SLOT(truncateTable()));
    connect(&actionDeleteTable, SIGNAL(triggered()), this, SLOT(deleteTable()));
    connect(&actionRenameTable, SIGNAL(triggered()), this, SLOT(renameTable()));

//    QAction *selected = menu.exec(mapToGlobal(evet->pos()));
    if (menu.exec(this->databaseListView->mapToGlobal(position))) {
        this->currentContextMenuItem = nullptr;
    }
}

void MysqlConnection::truncateTable() {
    if (nullptr != currentContextMenuItem) {
        qDebug() << "Truncate!";
        qDebug() << currentContextMenuItem->text();
    }
}

void MysqlConnection::deleteTable() {
    if (nullptr != currentContextMenuItem) {
        qDebug() << "Delete!";
        qDebug() << currentContextMenuItem->text();
    }
}

void MysqlConnection::renameTable() {
    if (nullptr != currentContextMenuItem) {
        qDebug() << "Rename!";
        qDebug() << currentContextMenuItem->text();
    }
}

void MysqlConnection::handleDatabaseClicked(QModelIndex index) {
    QString databaseName = databaseCollection->data(index).toString();
    if (!switchDatabase(databaseName)) {
        return;
    }

    QSqlQuery query = sendQuery("SHOW TABLES");

    int row = index.row();
    int column = index.column();
    currentDatabaseItem = databaseCollection->item(row, column);

    // 172.19.2.2
    if (query.isActive()) {
        emit databaseCollection->layoutAboutToBeChanged();
        QFont font;
        font.setBold(true);
        currentDatabaseItem->setFont(font);
        currentDatabaseItem->removeRows(0, databaseCollection->item(row)->rowCount());
        while (query.next()) {
            QString tableName = query.value(0).toString();
            QStandardItem* item = new QStandardItem(tableName);
            item->setToolTip(tableName);
            item->setEditable(false);
            currentDatabaseItem->appendRow(item);
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

    if (nullptr != currentTableItem) {
        QFont font;
        font.setBold(false);
        currentTableItem->setFont(font);
    }

    currentTableItem = item;
    currentDatabaseItem = item->parent();

    QFont font;
    font.setBold(true);
    currentTableItem->setFont(font);
    currentDatabaseItem->setFont(font);

    queryResultModel = nullptr;
    delete queryResultModel;
    queryResultModel = new QStandardItemModel;
//    queryResultModel = new QSqlRelationalTableModel();

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
                QVariant variant = currentRecord.field(currentColumn).value();
                QStandardItem* cellItem = new QStandardItem(variant.isNull() ? "NULL" : variant.toString());
                queryResultModel->setItem(currentRow, currentColumn, cellItem);
            }
            ++currentRow;
        }
    }
    queryResultView->setModel(queryResultModel);
    queryResultView->resizeColumnsToContents();
}

void MysqlConnection::showResultTableContextMenu(const QPoint& point) {
    currentContextMenuItem = this->databaseCollection->itemFromIndex(this->databaseListView->indexAt(point));
    QMenu *menu=new QMenu();
//    QAction copyAction(tr("copy"), this);
    QAction copyAction(QCoreApplication::translate("MysqlConnection", "copy"));
    connect(&copyAction, SIGNAL(triggered()), this, SLOT(copyResultViewSelection()));
    menu->addAction(&copyAction);

    QAction deleteAction(QCoreApplication::translate("MysqlConnection", "delete"));
    connect(&deleteAction, SIGNAL(triggered()), this, SLOT(deleteResultViewSelection()));
    menu->addAction(&deleteAction);

    QAction pasteAction(QCoreApplication::translate("MysqlConnection", "paste"));
    connect(&pasteAction, SIGNAL(triggered()), this, SLOT(pasteToResultView()));
    menu->addAction(&pasteAction);

    QAction insertNullAction(QCoreApplication::translate("MysqlConnection", "insert NULL"));
    connect(&insertNullAction, SIGNAL(triggered()), this, SLOT(insertNullToResultView()));
    menu->addAction(&insertNullAction);

    if (menu->exec(queryResultView->viewport()->mapToGlobal(point))) {
        this->currentContextMenuItem = nullptr;
    }
}

void MysqlConnection::copyResultViewSelection() {
    int minRow = queryResultModel->rowCount();
    int maxRow = -1;
    int minCol = queryResultModel->columnCount();
    int maxCol = -1;

    QList<QModelIndex> indexes = queryResultView->selectionModel()->selection().indexes();

    foreach (QModelIndex index, indexes) {
        minRow = qMin(minRow, index.row());
        maxRow = qMax(maxRow, index.row());
        minCol = qMin(minCol, index.column());
        maxCol = qMax(maxCol, index.column());
    }

    if (0 > maxCol
        || 0 > maxRow
    ) {
        return;
    }

    // only one cell selected
    if (minRow == maxRow
        && minCol == maxCol
    ) {
        QVariant variant = queryResultModel->data(queryResultModel->index(minRow, minCol));
        QApplication::clipboard()->setText(variant.toString());
        return;
    }

    QString data = "<!--StartFragment-->\n";
    QString plainData = "";
    data += "<table>";

    for (int row = minRow; row <= maxRow; row++) {
        data += "<tr>\n";
        for (int col = minCol; col <= maxCol; col++) {
            QVariant variant = queryResultModel->data(queryResultModel->index(row, col));
            if (variant.canConvert(QVariant::Double)) {
                data += "  <td x:num>";
                if (col != minCol) {
                    plainData += "\t";
                }
            } else {
                data += "  <td>";
                if (col != minCol) {
                    plainData += "\t";
                }
            }

            data += variant.toString();
            data += "</td>\n";
            plainData += variant.toString();
        }
        data += "</tr>\n";
        plainData += "\n";
    }

    data += "</table>";
    data += "<!--EndFragment-->\n";

    QMimeData* mimeData = new QMimeData;
    mimeData->setText(plainData);
    mimeData->setHtml(data);
    QApplication::clipboard()->setMimeData(mimeData);
}

void MysqlConnection::deleteResultViewSelection() {
    int minRow = queryResultModel->rowCount();
    int maxRow = -1;
    int minCol = queryResultModel->columnCount();
    int maxCol = -1;

    QList<QModelIndex> indexes = queryResultView->selectionModel()->selection().indexes();

    foreach (QModelIndex index, indexes) {
        minRow = qMin(minRow, index.row());
        maxRow = qMax(maxRow, index.row());
        minCol = qMin(minCol, index.column());
        maxCol = qMax(maxCol, index.column());
    }
    if (0 > maxCol
        || 0 > maxRow
    ) {
        return;
    }

    emit queryResultModel->layoutAboutToBeChanged();
    queryResultModel->removeRows(minRow, (maxRow - minRow) + 1);
    emit queryResultModel->layoutChanged();

    return;
}

void MysqlConnection::pasteToResultView() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    int minRow = queryResultModel->rowCount();
    int maxRow = -1;
    int minCol = queryResultModel->columnCount();
    int maxCol = -1;

    QList<QModelIndex> indexes = queryResultView->selectionModel()->selection().indexes();

    foreach (QModelIndex index, indexes) {
        minRow = qMin(minRow, index.row());
        maxRow = qMax(maxRow, index.row());
        minCol = qMin(minCol, index.column());
        maxCol = qMax(maxCol, index.column());
    }

    if (0 > maxCol
        || 0 > maxRow
    ) {
        return;
    }

    for (int row = minRow; row <= maxRow; row++) {
        for (int col = minCol; col <= maxCol; col++) {
            queryResultModel->itemFromIndex(queryResultModel->index(row, col))->setText(clipboard->text());
        }
    }
}

void MysqlConnection::insertNullToResultView() {
    int minRow = queryResultModel->rowCount();
    int maxRow = -1;
    int minCol = queryResultModel->columnCount();
    int maxCol = -1;

    QList<QModelIndex> indexes = queryResultView->selectionModel()->selection().indexes();

    foreach (QModelIndex index, indexes) {
        minRow = qMin(minRow, index.row());
        maxRow = qMax(maxRow, index.row());
        minCol = qMin(minCol, index.column());
        maxCol = qMax(maxCol, index.column());
    }

    if (0 > maxCol
        || 0 > maxRow
    ) {
        return;
    }

    for (int row = minRow; row <= maxRow; row++) {
        for (int col = minCol; col <= maxCol; col++) {
            queryResultModel->itemFromIndex(queryResultModel->index(row, col))->setText("NULL");
        }
    }
}

bool MysqlConnection::switchDatabase(QString databaseName) {

    if (nullptr != currentDatabaseItem
        && currentDatabaseItem->text() == databaseName
    ) {
        return true;
    }

    if (nullptr != currentTableItem) {
        QFont font;
        font.setBold(false);
        currentTableItem->setFont(font);
    }

    if (database.isOpen()) {
        database.close();
    }

    database.setDatabaseName(databaseName);

    if (nullptr != currentDatabaseItem) {
        QFont font;
        font.setBold(false);
        currentDatabaseItem->setFont(font);
    }

    if (!database.open()) {
        qWarning() << database.lastError();
        return false;
    }
    return true;
}
