#ifndef CONNECTION_H
#define CONNECTION_H

#include "textedit.h"

#include <QObject>
#include <QModelIndex>
#include <QTreeView>
#include <QTableView>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextEdit>
#include <QCompleter>

class Connection
{
private:
    uint lastQueryTime = 0;

protected:
    QSqlDatabase database;

    QString activeTableName;
    QString activeDatabaseName;

    QTreeView* databaseListView = NULL;
    QTableView* queryResultView = NULL;
    TextEdit* queryRequestView = NULL;
    QTextEdit* informationView = NULL;
    QCompleter* completer = NULL;
    QStandardItemModel* databaseCollection = NULL;
    QStandardItemModel* queryResultModel = NULL;

public:
    Connection();
    ~Connection();

    virtual void init() = 0;
    virtual void loadDatabaseList() = 0;

//    virtual void collectionTableInformations();

    void setDatabaseListView(QTreeView* databaseListView);
    QTreeView* getDatabaseListView() const;

    void setQueryResultView(QTableView* queryResultView);
    QTableView* getQueryResultView() const;

    void setQueryRequestView(TextEdit* queryRequestView);
    TextEdit* getQueryRequestView() const;

    void setInformationView(QTextEdit* informationView);
    QTextEdit* getInformationView() const;

    QString generateLastExecutedQuery(const QSqlQuery& query);

    QSqlError lastError();
    void close();
    bool open();

    QSqlQuery sendQuery(QSqlQuery query);
    QSqlQuery sendQuery(QString queryString);

//public slots:
    virtual void onListViewDoubleClicked(const QModelIndex index) = 0;
};

#endif // CONNECTION_H
