#ifndef CONNECTION_H
#define CONNECTION_H

#include <QModelIndex>
#include <QTreeView>
#include <QTableView>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextEdit>

class Connection
{
private:
    uint lastQueryTime = 0;

protected:
    QSqlDatabase database;

    QSqlQuery sendQuery(QSqlQuery query);
    QSqlQuery sendQuery(QString queryString);

    QTreeView* databaseListView = NULL;
    QTableView* queryResultView = NULL;
    QTextEdit* informationView = NULL;

    QStandardItemModel* databaseCollection = NULL;

    QStandardItemModel* queryResultModel = NULL;

public:
    Connection();
    ~Connection();

//    virtual void init();

//    virtual void collectionTableInformations();

    void setDatabaseListView(QTreeView* databaseListView);
    QTreeView* getDatabaseListView() const;

    void setQueryResultView(QTableView* queryResultView);
    QTableView* getQueryResultView() const;

    void setInformationView(QTextEdit* informationView);
    QTextEdit* getInformationView() const;

    QString generateLastExecutedQuery(const QSqlQuery& query);

    QSqlError lastError();
    void close();
    bool open();

//public slots:
//    void onListViewDoubleClicked(QModelIndex index);
};

#endif // CONNECTION_H
