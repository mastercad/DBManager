#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "newconnectionwindow.h"
#include "connectionfactory.h"
#include "connection.h"

#include <QMainWindow>
#include <QObjectData>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QCompleter>

class QTreeModel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openNewConnectionWindow();
    void newConnectionData(QObjectData& connectionData);
    void onQueryResultHeaderClicked(QStandardItem* item);
    void onListViewClicked(const QModelIndex);
    void onListViewDoubleClicked(const QModelIndex);
    void onExecuteQueryClicked();

private:
    Ui::MainWindow *ui;
    void createToolbars();
    void handleDatabaseClicked(QModelIndex index);
    void handleTableClicked(QStandardItem* item);
    bool switchDatabase(QString databaseName);
    void collectTableInformations();
    QString generateLastExecutedQuery(const QSqlQuery& query);
    QSqlQuery sendQuery(QSqlQuery query);
    QSqlQuery sendQuery(QString queryString);

    NewConnectionWindow* newConnectionWindow = NULL;
    Connection* dbConnection = NULL;
    QString activeDatabase;
    QString activeTable;
    ConnectionFactory* connectionFactory = NULL;

    QMap<QString, bool> keywords;
    uint lastQueryTime = 0;
    QCompleter* completer = NULL;
};

#endif // MAINWINDOW_H
