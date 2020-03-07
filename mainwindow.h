#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connectionfactory.h"
#include "connection.h"
#include "connections.h"
#include "connectioninfofactory.h"
#include "connectioninfo.h"
#include "filedownloader.h"
#include "updatemanager.h"
#include "releasenotesmanager.h"
#include "sidebar.h"

#include <QMainWindow>
#include <QObjectData>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QCompleter>
#include <QKeyEvent>
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

    void enableReleaseNotesButton();

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
    void markAsUnsaved(bool);
    void saveQuery();
    void loadQuery();
    void onChangeTab(int);
    void closeTab(int);
    void showAboutText();
    void showReleaseNotes();

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
    void keyPressEvent(QKeyEvent* event);
    UpdateManager* updateManager = nullptr;

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

//    QMap<QString, QMap<QString, ConnectionInfo*> > connections;
    Connections connections;
    QMap<int, QString>* currentQueryRequests = nullptr;
    QMap<QString, bool> keywords;
    uint lastQueryTime = 0;
    QCompleter* completer = nullptr;
    bool connectionsSaved = true;
    SideBar* sidebar = nullptr;
    bool sideBarVisible = false;
    ReleaseNotesManager* releaseNotesManager = nullptr;
};

#endif // MAINWINDOW_H
