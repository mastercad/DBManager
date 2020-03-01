#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connectionfactory.h"
#include "connection.h"
#include "connections.h"
#include "connectioninfofactory.h"
#include "connectioninfo.h"
#include "filedownloader.h"

#include <QMainWindow>
#include <QObjectData>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QNetworkAccessManager>
#include <QCompleter>
#include <QNetworkReply>
#include <QProgressDialog>
#include <QPoint>
#include <QList>

class QTreeModel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openNewConnectionWindow();
    void openConnectionManagerWindow();
    void onEstablishNewConnection(QAction* action);
    void onQueryResultHeaderClicked(QStandardItem* item);
    void onExecuteQueryClicked();
    void createConnectionSubMenu();
    void saveConnectionInfos();
    void handleConnectionError(QString errorMessage);
    void handleChangedQueryRequest();
    void handleUpdateInformationDownload(QNetworkReply*);
    void markAsUnsaved(bool);
    void saveQuery();
    void loadQuery();
    void onChangeTab(int);
    void closeTab(int);
    void checkUpdateAvailable();
    void updateCanceled();
    void updateTimedOut();
    void updateUpdateProgress(qint64 current, qint64 max);
    void updateError(QNetworkReply::NetworkError error);
    void updateFileFinished(QNetworkReply* reply);
    void adjustProgressValue(int value);
    void showAboutText();

private:
    Ui::MainWindow *ui;
    void handleDatabaseClicked(QModelIndex index);
    void handleTableClicked(QStandardItem* item);
    bool switchDatabase(QString databaseName);
    void showResultTableContextMenu(const QPoint& point);
    void collectTableInformations();
    void storeConnectionInfo(ConnectionInfo* connectionInfo);
    void loadConnectionInfos();
    bool isUnsaved() const;
    void setUnsaved(const bool);
    void showQuerySaveIcon();
    void hideQuerySaveIcon();
    void newTab();

    Connection* establishNewConnection(ConnectionInfo* connectionInfo);

    QString generateLastExecutedQuery(const QSqlQuery& query);
    QSqlQuery sendQuery(QSqlQuery query);
    QSqlQuery sendQuery(QString queryString);

    Connection* dbConnection = nullptr;
    ConnectionInfo* connectionInfo= nullptr;

    QString activeDatabase;
    QString activeTable;
    bool unsaved = false;
    int currentTabIndex = 0;

    ConnectionFactory* connectionFactory = nullptr;
    ConnectionInfoFactory* connectionInfoFactory = nullptr;

    QNetworkAccessManager networkManager;
    QProgressDialog* progressDialogUpdate = nullptr;

//    QMap<QString, QMap<QString, ConnectionInfo*> > connections;
    Connections connections;
    QMap<int, QString> currentQueryRequests;
    QMap<QString, bool> keywords;
    uint lastQueryTime = 0;
    QCompleter* completer = nullptr;
    bool connectionsSaved = true;
    QList<FileDownloader*> fileDownloaderCollection;
};

#endif // MAINWINDOW_H
