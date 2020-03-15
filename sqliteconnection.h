#ifndef SQLITECONNECTION_H
#define SQLITECONNECTION_H

#include "connection.h"

#include <QString>
#include <QPoint>
#include <QModelIndex>

class SqliteConnection : public Connection
{
    using Connection::Connection;

private:
    void handleTableClicked(QModelIndex index);
    void loadDatabaseList();
    void loadTablesList();

public:
    void init();

public slots:
    void addNewRow();
    void onListViewDoubleClicked(const QModelIndex index);
    void showDatabaseContextMenu(const QPoint& position);
    void showResultTableContextMenu(const QPoint& position);
    void truncateTable();
    void deleteTable();
    void renameTable();
    void copyResultViewSelection();
    void deleteResultViewSelection();
    void pasteToResultView();
    void insertNullToResultView();
    void cellEntered(int, int);
    void saveQueryResultChanges();
    void cancelQueryResultChanges();
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&);
};

#endif // SQLITECONNECTION_H
