#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    FileDownloader(QObject*);
    ~FileDownloader();
    void download(QString file);

    void setTargetPathName(const QString targetPathName);
    bool isFinished() const;
    void abort();

public slots:
    void finished(QNetworkReply*);

private:
    QString sourcePathName;
    QString downloadUrl;
    QString targetPathName;
    QTimer* timer = nullptr;
    bool downloadFinished = false;

    QObject* mainWindow = nullptr;
    QNetworkAccessManager networkManager;
    QNetworkReply* reply = nullptr;
};

#endif // FILEDOWNLOADER_H
