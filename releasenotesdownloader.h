#ifndef RELEASENOTESDOWNLOADER_H
#define RELEASENOTESDOWNLOADER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class ReleaseNotesDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ReleaseNotesDownloader(QObject *parent = nullptr);
    QNetworkReply* download();

signals:
    QNetworkReply* onFinished(QNetworkReply*);

public slots:
    void finished(QNetworkReply*);

private:
    QNetworkAccessManager networkManager;
};

#endif // RELEASENOTESDOWNLOADER_H
