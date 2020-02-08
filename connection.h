#ifndef CONNECTION_H
#define CONNECTION_H

#include "textedit.h"
#include "connectioninfo.h"

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

class Connection : public QObject
{
    Q_OBJECT

private:
    long lastQueryTime = 0;
    ConnectionInfo* connectionInfo = nullptr;

protected:
    QSqlDatabase database;

    QString activeTableName;
    QString activeDatabaseName;

    QTreeView* databaseListView = nullptr;
    QTableView* queryResultView = nullptr;
    TextEdit* queryRequestView = nullptr;
    QTextEdit* informationView = nullptr;
    QCompleter* completer = nullptr;
    QStandardItemModel* databaseCollection = nullptr;
    QStandardItemModel* queryResultModel = nullptr;

signals:
    void connectionError(QString);

public:
    ~Connection();

    virtual void init() = 0;
    virtual void loadDatabaseList() = 0;

    void setDatabaseListView(QTreeView* databaseListView);
    QTreeView* getDatabaseListView() const;

    void setQueryResultView(QTableView* queryResultView);
    QTableView* getQueryResultView() const;

    void setQueryRequestView(TextEdit* queryRequestView);
    TextEdit* getQueryRequestView() const;

    void setInformationView(QTextEdit* informationView);
    QTextEdit* getInformationView() const;

    void setConnectionInfo(ConnectionInfo* connectionInfo);
    ConnectionInfo* getConnectionInfo() const;

    QString generateLastExecutedQuery(const QSqlQuery& query);

    QSqlError lastError();
    void close();
    bool open();
    bool isOpen();

    QSqlQuery sendQuery(QSqlQuery query);
    QSqlQuery sendQuery(QString queryString);

public slots:
    virtual void onListViewDoubleClicked(const QModelIndex index) = 0;
};

#endif // CONNECTION_H
