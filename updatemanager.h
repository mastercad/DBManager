#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include "filedownloader.h"

#include <QList>
#include <QWidget>
#include <QNetworkReply>
#include <QProgressDialog>
#include <QNetworkAccessManager>

#include <QObject>

class UpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit UpdateManager(QWidget *parent = nullptr);
    ~UpdateManager();

signals:

public slots:
    void handleUpdateInformationDownload(QNetworkReply*);
    void checkUpdateAvailable(bool manualTriggered = true);
    void updateCanceled();
    void updateTimedOut();
    void updateUpdateProgress(qint64 current, qint64 max);
    void updateError(QNetworkReply::NetworkError error);
    void updateFileFinished(QNetworkReply* reply);
    void adjustProgressValue(int value);

private:
    QWidget* parent = nullptr;
    QProgressDialog* progressDialogUpdate = nullptr;
    QNetworkAccessManager networkManager;
    bool updateManualExecuted = true;
    QList<FileDownloader*> fileDownloaderCollection;
};

#endif // UPDATEMANAGER_H
