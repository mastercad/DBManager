#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H

#include "connection.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QPoint>
#include <QString>

class MysqlConnection : public Connection
{
private:
    bool switchDatabase(QString databaseName);
    void handleTableClicked(QStandardItem* item);
    void handleDatabaseClicked(QModelIndex index);
    QStandardItem* currentDatabaseItem = nullptr;
    QStandardItem* currentTableItem = nullptr;
    QStandardItem* currentContextMenuItem = nullptr;

public slots:
    void onListViewDoubleClicked(const QModelIndex index);
    void handleDatabaseContextMenuClicked(const QPoint&);
    void handleResultTableContextMenuClicked(const QPoint&);
    void truncateTable();
    void deleteTable();
    void renameTable();
    void copyResultViewSelection();
    void deleteResultViewSelection();
    void pasteToResultView();
    void insertNullToResultView();

public:
    void init();

    void collectTableInformations();
    void loadDatabaseList();
};

#endif // MYSQLCONNECTION_H
