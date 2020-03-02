#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H

#include "connection.h"

#include <QStandardItem>
#include <QPoint>
#include <QWidget>
#include <QString>

class MysqlConnection : public Connection
{
    using Connection::Connection;

signals:
    void queryResultDataChanged();

private:
    bool switchDatabase(QString databaseName);
    void handleTableClicked(QStandardItem* item);
    void handleDatabaseClicked(QModelIndex index);
    QStandardItem* currentDatabaseItem = nullptr;
    QStandardItem* currentTableItem = nullptr;
    QStandardItem* currentContextMenuItem = nullptr;

public slots:
    void onListViewDoubleClicked(const QModelIndex index);
    void saveQueryResultChanges();
    void cancelQueryResultChanges();
    void showDatabaseContextMenu(const QPoint&);
    void showResultTableContextMenu(const QPoint&);
    void truncateTable();
    void deleteTable();
    void renameTable();
    void copyResultViewSelection();
    void deleteResultViewSelection();
    void pasteToResultView();
    void insertNullToResultView();
    void cellEntered(int, int);
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&);

public:
    void init();

    void collectTableInformations();
    void loadDatabaseList();
};

#endif // MYSQLCONNECTION_H
