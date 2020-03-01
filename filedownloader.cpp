#include "filedownloader.h"

#include "defaults.h"

#include <QNetworkRequest>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include <QDebug>

FileDownloader::FileDownloader(QObject* mainWindow) :
    QObject(mainWindow),
    mainWindow(mainWindow),
    networkManager(this)
{

}

void FileDownloader::download(QString file) {
    timer = new QTimer(mainWindow);
    timer->start(0);

    QString downloadUrl = Application::UpdateDonwloadUrl+file;
//    connect(timer, SIGNAL(timeout()), mainWindow, SLOT(updateTimedOut()));
//    qDebug() << "Download " << downloadUrl;
//    networkManager = new QNetworkAccessManager(this);

    QNetworkRequest request(downloadUrl);
    reply = networkManager.get(request);

    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), mainWindow, SLOT(updateUpdateProgress(qint64, qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), mainWindow, SLOT(updateError(QNetworkReply::NetworkError)));
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), mainWindow, SLOT(updateFileFinished(QNetworkReply*)));
}

void FileDownloader::finished(QNetworkReply* reply) {
//    qDebug() << reply->readAll();
    timer->stop();

    if (reply->error() != QNetworkReply::NoError ) {
        qWarning() << "ErrorNo: " << reply->error() << "for url: " << reply->url().toString();
        qDebug() << "Request failed, " << reply->errorString();
        qDebug() << "Headers:"<<  reply->rawHeaderList()<< "content:" << reply->readAll();
//        runningreplies.erase(reply);
        return;
    }

    QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    if(redirect.isValid()
        && reply->url() != redirect
    ) {
        if(redirect.isRelative()) {
            redirect = reply->url().resolved(redirect);
        }
        timer->start(0);
        QNetworkRequest req(redirect);
        req.setRawHeader( "User-Agent" , "DBManager Update" );
        this->reply = this->networkManager.get(req);

        connect(this->reply, SIGNAL(downloadProgress(qint64, qint64)), mainWindow, SLOT(updateUpdateProgress(qint64, qint64)));
        connect(this->reply, SIGNAL(error(QNetworkReply::NetworkError)), mainWindow, SLOT(updateError(QNetworkReply::NetworkError)));

//        qDebug() << "Redirect Download!";
//        runningreplies.insert(std::make_pair(reply, id));
        return;
    }

    QString filePathName = QFileInfo(this->targetPathName).absolutePath();
//    qDebug() << "TargetPath: " << filePathName;
    // ensure filepath exists
    QDir().mkpath(filePathName);

    QFile file(targetPathName);
    file.open(QIODevice::WriteOnly);
    file.write(this->reply->readAll());
    file.close();

//    qDebug() << "FileDownloader::finished!";
    this->downloadFinished = true;
}

void FileDownloader::abort() {
    this->reply->abort();
    this->reply->close();
}

void FileDownloader::setTargetPathName(const QString targetPathName) {
    this->targetPathName = targetPathName;
}

bool FileDownloader::isFinished() const {
    return this->downloadFinished;
//    return this->reply->isFinished();
}

FileDownloader::~FileDownloader() {
    delete timer;
    delete reply;
}
