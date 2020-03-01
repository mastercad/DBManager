#ifndef CONNECTION_H
#define CONNECTION_H

#include "textedit.h"
#include "connectioninfo.h"

#include <QObject>
#include <QModelIndex>
#include <QTreeView>
#include <QTableView>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextEdit>
#include <QPoint>
#include <QCompleter>

class Connection: public QObject
{
    Q_OBJECT
//    Q_DECLARE_TR_FUNCTIONS Connection;

private:
    long lastQueryTime = 0;
    ConnectionInfo* connectionInfo = nullptr;

protected:
    QSqlDatabase database;

    QString activeTableName;
    QString activeDatabaseName;

    QObject* parent = nullptr;
    QTreeView* databaseListView = nullptr;
    QTableView* queryResultView = nullptr;
    TextEdit* queryRequestView = nullptr;
    QTextEdit* informationView = nullptr;
    QCompleter* completer = nullptr;
    QStandardItemModel* databaseCollection = nullptr;
//    QStandardItemModel* queryResultModel = nullptr;
//    QSqlTableModel* queryResultModel = nullptr;
    QSqlRelationalTableModel* queryResultModel = nullptr;
//    QSqlRelationalTableModel* queryResultModel = nullptr;

signals:
    void connectionError(QString);

public:
    explicit Connection(QObject* parent);
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
    virtual void showDatabaseContextMenu(const QPoint& position) = 0;
    virtual void showResultTableContextMenu(const QPoint& position) = 0;
    virtual void truncateTable() = 0;
    virtual void deleteTable() = 0;
    virtual void renameTable() = 0;
    virtual void copyResultViewSelection() = 0;
    virtual void deleteResultViewSelection() = 0;
    virtual void pasteToResultView() = 0;
    virtual void insertNullToResultView() = 0;
    virtual void cellEntered(int, int) = 0;
    virtual void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&) = 0;
};

#endif // CONNECTION_H
