#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H

#include "connection.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>

class MysqlConnection : public Connection
{
private:
    bool switchDatabase(QString databaseName);
    void handleTableClicked(QStandardItem* item);
    void handleDatabaseClicked(QModelIndex index);

    QStandardItemModel* databaseCollection = NULL;

public slots:
    void onListViewDoubleClicked(const QModelIndex index);

public:
    void init();

    void collectTableInformations();
    void loadDatabaseList();
};

#endif // MYSQLCONNECTION_H
