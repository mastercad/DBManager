#ifndef SQLITECONNECTION_H
#define SQLITECONNECTION_H

#include "connection.h"

#include <QString>
#include <QModelIndex>

class SqliteConnection : public Connection
{
private:
    void handleTableClicked(QModelIndex index);
    void loadDatabaseList();
    void loadTablesList();

public:
    void init();

public slots:
    void onListViewDoubleClicked(const QModelIndex index);
    void handleContextMenuClicked(const QPoint&);
};

#endif // SQLITECONNECTION_H
