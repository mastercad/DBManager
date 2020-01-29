#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "newconnectionwindow.h"
#include "connectionwithlogindialog.h"

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
    QSqlDatabase dbConnection;
    QStandardItemModel* databaseCollection = NULL;
    QString activeDatabase;
    QString activeTable;

    QMap<QString, bool> keywords;
    uint lastQueryTime = 0;
    QStandardItemModel* queryResultModel = NULL;
    QCompleter* completer = NULL;
//    QList<DatabaseConnection*>* databaseConnections = NULL;
};

#endif // MAINWINDOW_H
