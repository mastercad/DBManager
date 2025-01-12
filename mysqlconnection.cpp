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
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
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
        databaseCollection = nullptr;
        delete databaseCollection;
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
    tableNameCollection = nullptr;
    delete tableNameCollection;
    tableNameCollection = new QMap<QString, QString>;
    if (query.isActive()) {
        while (query.next()) {
            if ("BASE TABLE" == query.value(3)) {
                tableNameCollection->insert(query.value(1).toString(), query.value(2).toString());
            }
        }
    }

    query.exec("SELECT * FROM TABLE_CONSTRAINTS");

    query.exec("SELECT * FROM COLUMNS");

    query.exec("SELECT * FROM SCHEMATA");

    query.exec("SELECT * FROM KEYWORDS");

    keywords = nullptr;
    delete keywords;
    keywords = new QMap<QString, bool>();

    if (query.isActive()) {
        while (query.next()) {
            keywords->insert(query.value(0).toString(), query.value(1).toBool());
        }
    }
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

        QMessageBox msgBox;
        msgBox.setText(tr("Truncate Table"));
        msgBox.setInformativeText(tr("Press OK to truncate selected Table!"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);

        if (msgBox.exec()) {
            emit databaseCollection->layoutAboutToBeChanged();
            this->database.exec("TRUNCATE TABLE "+currentContextMenuItem->text());

            this->queryResultView->setModel(nullptr);
            delete this->queryResultModel;
            this->queryResultModel = nullptr;
            emit databaseCollection->layoutChanged();
        }
    }
}

void MysqlConnection::deleteTable() {
    if (nullptr != currentContextMenuItem) {

        QMessageBox msgBox;
        msgBox.setText(tr("Delete Table"));
        msgBox.setInformativeText(tr("Press OK to drop selected Table!"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);

        if (msgBox.exec()) {
            emit databaseCollection->layoutAboutToBeChanged();
            this->database.exec("DROP TABLE "+currentContextMenuItem->text());

            QSqlQuery query = sendQuery("SHOW TABLES");

            QFont font;
            font.setBold(true);
            currentDatabaseItem->setFont(font);
            currentDatabaseItem->removeRows(0, databaseCollection->item(currentDatabaseItem->row())->rowCount());
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
}

void MysqlConnection::renameTable() {
    if (nullptr != currentContextMenuItem) {
        bool ok;
        QString text = QInputDialog::getText(
            (QWidget*)this->parent,
            tr("QInputDialog::getText()"),
            tr("Table Name:"),
            QLineEdit::Normal,
            currentContextMenuItem->text(),
            &ok
        );

        if (ok
            && !text.isEmpty()
            && text != currentContextMenuItem->text()
        ) {
            emit databaseCollection->layoutAboutToBeChanged();
            this->database.exec("ALTER TABLE "+currentContextMenuItem->text()+" RENAME TO "+text+";");

            QSqlQuery query = sendQuery("SHOW TABLES");

            QFont font;
            font.setBold(true);
            currentDatabaseItem->setFont(font);
            currentDatabaseItem->removeRows(0, databaseCollection->item(currentDatabaseItem->row())->rowCount());
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
    queryResultModel = new QSqlRelationalTableModel(this, database);
    queryResultModel->setTable(activeTableName);
    queryResultModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    queryResultModel->select();

    // Create reference to validate data if changed
    origQueryResultModel = nullptr;
    delete origQueryResultModel;
    origQueryResultModel = new QSqlRelationalTableModel(this, database);
    origQueryResultModel->setTable(activeTableName);
    origQueryResultModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    origQueryResultModel->select();
//    origQueryResultModel->setData(queryResultModel->data());

    connect(queryResultModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)));

    queryResultView->setModel(queryResultModel);
    queryResultView->setVisible(false);
    queryResultView->resizeColumnsToContents();
//    queryResultView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    queryResultView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    queryResultView->setVisible(true);
}

void MysqlConnection::dataChanged(const QModelIndex& indexFrom, const QModelIndex& indexTo, const QVector<int>& role) {
    qDebug() << "MysqlConnection::dataChanged indexFrom: " << indexFrom << " indexTo: " << indexTo << " Role: " << role;

    if (nullptr != this->origQueryResultModel) {
        qDebug() << "OriginalResult Model: " << this->origQueryResultModel;
        qDebug() << "QueryResult Model: " << this->queryResultModel;

        bool changed = false;
        for (int row = indexFrom.row(); row <= indexTo.row(); row++) {
            for (int col = indexFrom.column(); col <= indexTo.column(); col++) {
                QVariant variant = queryResultModel->data(queryResultModel->index(row, col));
                QVariant variantOrig = origQueryResultModel->data(queryResultModel->index(row, col));
                qDebug() << "Inhalt " << row << ":" << col << ": " << variant.toString() << " Orig: " << variantOrig;

                if (variantOrig != variant) {
                    changed = true;
                }
            }
        }

        qDebug() << "Data changed: " << changed;
//    parent->emit(queryResultDataChanged());
    }
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

    QList<QModelIndex> indexes = queryResultView->selectionModel()->selection().indexes();
    foreach (QModelIndex index, indexes) {
        QVariant variant = queryResultModel->data(index);
        variant.setValue(clipboard->text());
        queryResultModel->setData(index, variant);
    }
}

void MysqlConnection::insertNullToResultView() {
    QList<QModelIndex> indexes = queryResultView->selectionModel()->selection().indexes();
    foreach (QModelIndex index, indexes) {
        QVariant variant = queryResultModel->data(index);
        variant.setValue(nullptr);
        queryResultModel->setData(index, variant);
    }
}

void MysqlConnection::cellEntered(int row, int cell) {
    qDebug() << "MysqlConnection::cellEntered: Row: " << row << " Cell: " << cell;
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

void MysqlConnection::saveQueryResultChanges() {
    qDebug() << "Save QueryResultChanges!";

    this->queryResultModel->database().transaction();

    if (this->queryResultModel->submitAll()) {
        this->queryResultModel->database().commit();
    } else {
        if (this->queryResultModel->lastError().isValid()) {
            this->informationView->append(this->queryResultModel->lastError().text());
            qCritical() << this->queryResultModel->lastError();
        }
        this->queryResultModel->database().rollback();
    }
}

void MysqlConnection::cancelQueryResultChanges() {
    qDebug() << "Cancel QueryResultChanges!";

    if (nullptr != this->queryResultModel) {
        this->queryResultModel->revertAll();
        this->queryResultModel->database().rollback();
    }
}

void MysqlConnection::addNewRow() {
    this->queryResultModel->insertRow(this->queryResultModel->rowCount());
}
