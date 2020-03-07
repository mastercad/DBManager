#include "releasenotesdownloader.h"

#include "defaults.h"

#include <QDebug>

ReleaseNotesDownloader::ReleaseNotesDownloader(QObject *parent) : QObject(parent) {
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

QNetworkReply* ReleaseNotesDownloader::download() {
    QNetworkRequest request(Application::ReleaseNotesUrl);
    return this->networkManager.get(request);
}

void ReleaseNotesDownloader::finished(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError ) {
        qWarning() << "ErrorNo: " << reply->error() << "for url: " << reply->url().toString();
        qDebug() << "Request failed, " << reply->errorString();
        qDebug() << "Headers:"<<  reply->rawHeaderList()<< "content:" << reply->readAll();
        return;
    }

    QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    if(redirect.isValid()
        && reply->url() != redirect
    ) {
        if(redirect.isRelative()) {
            redirect = reply->url().resolved(redirect);
        }
        QNetworkRequest req(redirect);
        req.setRawHeader( "User-Agent" , "DBManager ReleaseNotes" );
        this->networkManager.get(req);
        return;
    }

    emit(onFinished(reply));
}
