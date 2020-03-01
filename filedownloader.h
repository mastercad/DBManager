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
    FileDownloader(QWidget*);
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
    QTimer* timer;
    bool downloadFinished = false;

    QWidget* mainWindow;
    QNetworkAccessManager* networkManager;
    QNetworkReply* reply;
};

#endif // FILEDOWNLOADER_H
