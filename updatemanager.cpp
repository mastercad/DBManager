#include "updatemanager.h"

#include "defaults.h"

#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>
#include <QXmlStreamReader>

#include <QDebug>

UpdateManager::UpdateManager(QWidget *parent) :
    QObject(parent),
    parent(parent),
    networkManager(parent)
{
//    this->networkManager = new QNetworkAccessManager(this->parent);

    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleUpdateInformationDownload(QNetworkReply*)));
}

void UpdateManager::checkUpdateAvailable(bool manualTriggered) {
    this->updateManualExecuted = manualTriggered;
    QNetworkRequest request(Application::UpdateUrl);
    this->networkManager.get(request);
}

void UpdateManager::handleUpdateInformationDownload(QNetworkReply* reply) {
//    qDebug() << "UpdateManager::handleUpdateInformationDownload";

    if ( reply->error() != QNetworkReply::NoError ) {
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
        QNetworkRequest req(redirect);
        req.setRawHeader( "User-Agent" , "DBManager Update" );
        this->networkManager.get(req);
//        runningreplies.insert(std::make_pair(reply, id));
        return;
    }

    QByteArray data = reply->readAll();
    QXmlStreamReader xmlReader(data);
    QString xmlBuildNo = "";

    QMap<QString, QString> xmlUpdateInformations;
    QStringList updateFiles;
    QString lastElementName;
    qint64 maximumFileSize = 0;
    qint64 fileSize = 0;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        QString elementName = xmlReader.name().toString();
        if ("BuildNo" == lastElementName
            && xmlReader.isCharacters()
            && !xmlReader.isWhitespace()
        ) {
            xmlBuildNo = xmlReader.text().toString().trimmed();
            if (!xmlBuildNo.isEmpty()
                && xmlBuildNo > Application::BuildNo
            ) {
                break;
            }
        }

        if (!elementName.isEmpty()) {
            lastElementName = xmlReader.name().toString();
        }
    }

    // Update possible
    if (xmlBuildNo > Application::BuildNo) {
        QMessageBox updateInformation;

        updateInformation.setWindowTitle(tr("Update available"));
        updateInformation.setIcon(QMessageBox::Information);
        updateInformation.setText(tr("There is version %1 for DBManager available!").arg(xmlBuildNo));
        updateInformation.setInformativeText(tr("Press \"OK\" to update."));
        updateInformation.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        updateInformation.setDefaultButton(QMessageBox::Ok);

        if (QMessageBox::Ok == updateInformation.exec()) {
            updateInformation.hide();
            QStringList fileList;
            QString currentBuildNumber;
            QString elementName;
            QString elementContent;

            xmlReader.clear();
            xmlReader.addData(data);

            while (!xmlReader.atEnd() && !xmlReader.hasError()) {
                xmlReader.readNext();
                elementName = xmlReader.name().toString();
                elementContent = xmlReader.text().toString().trimmed();

                // only loop to the last lower buildNumber
                if ("BuildNo" == lastElementName
                    && !elementContent.isEmpty()
                    && xmlReader.isCharacters()
                    && !xmlReader.isWhitespace()
                ) {
                    currentBuildNumber = elementContent;
                    if (currentBuildNumber <= Application::BuildNo) {
                        break;
                    }
                }

                if(!elementContent.isEmpty()
                    && "File" == lastElementName
                    && xmlReader.isCharacters()
                    && !xmlReader.isWhitespace()
                    && !fileList.contains(elementContent)
                ) {
                    fileList.append(elementContent);
                    maximumFileSize += fileSize;
//                    qDebug() << "Habe FileSize: " << fileSize;
//                    qDebug() << "Maximum ist jetzt: " << maximumFileSize;
                    fileSize = 0;
                }

                if (!elementName.isEmpty()) {
                    lastElementName = xmlReader.name().toString();
                    if ("File" == lastElementName) {
                        foreach (const QXmlStreamAttribute attribute, xmlReader.attributes()) {
                            if ("size" == attribute.name().toString()) {
                                fileSize = attribute.value().toLongLong();
                                break;
                            }
                        }
                    }
                }
            }

//            qDebug() << fileList;
            QString updateTargetDirectoryPathName = "update";
            QDir updateTargetDirectory(updateTargetDirectoryPathName);
//            QDir updateTargetDirectory(QCoreApplication::applicationDirPath()+"/../");

            // prepare directory
            if (updateTargetDirectory.exists()) {
                if (!updateTargetDirectory.removeRecursively()) {
                    qCritical() << "ERROR CLEAN PATH FOR UPDATE APPLICATION!";
                }
            }

            if (!QDir().mkdir(updateTargetDirectory.absolutePath())) {
                qCritical() << "ERROR MKDIR FOR UPDATE APPLICATION!";
            }

            progressDialogUpdate = new QProgressDialog(tr("Update in progress."), tr("Cancel"), 0, maximumFileSize);
            connect(progressDialogUpdate, SIGNAL(canceled()), this, SLOT(updateCanceled()));

            progressDialogUpdate->setRange(0, maximumFileSize);
//            progressDialogUpdate->setRange(0, fileList.size());
            progressDialogUpdate->show();

//            qDebug() << "MaximumFileSize: " << maximumFileSize;

            for (QString file : fileList) {
                FileDownloader* fileDownloader = new FileDownloader(this);
                fileDownloader->setTargetPathName(updateTargetDirectory.absolutePath()+"/"+file);
//                qDebug() << file;
                fileDownloader->download(file);
                this->fileDownloaderCollection.append(fileDownloader);
//                fileDownloader->deleteLater();
            }

        }
    } else if (true == this->updateManualExecuted) {
        QMessageBox::information(
            this->parent,
            tr("DBManager already on newest version!"),
            tr("Your version of DBManager is already up to date with %1").arg(Application::BuildNo)
        );
    }
}

void UpdateManager::adjustProgressValue(int value) {
//    qDebug() << "UpdateManager::adjustProgressValue";
    progressDialogUpdate->setMaximum(progressDialogUpdate->maximum() + value);
}

void UpdateManager::updateFileFinished(QNetworkReply* reply) {
//    qDebug() << "UpdateManager::updateFileFinished";
/*
    // Path of startup source to execute (/home/user)
    qDebug() << "WorkingDir: " << QDir::currentPath();
    // path of executable
    qDebug() << "ApplicationDirPath: " << QCoreApplication::applicationDirPath();
    QString pwd("");
    char * PWD;
    PWD = getenv ("PWD");
    pwd.append(PWD);

    qDebug() << "PWD: " << pwd;

    QDir dir(".");
    qDebug() << "./: " << dir.absolutePath();
*/
//    qDebug() << "Progressbar Value in Finished: " << progressDialogUpdate->value();

    bool allFileDownloaderSucceed = true;
    int finishedDownloads = 0;

    for (FileDownloader* fileDownloader: this->fileDownloaderCollection) {
        if (!fileDownloader->isFinished()) {
            allFileDownloaderSucceed = false;
        } else {
            finishedDownloads++;
        }
    }

    progressDialogUpdate->setValue(finishedDownloads);

//    qDebug() << "All FileDownloader Succeed?: " << allFileDownloaderSucceed;

    if (allFileDownloaderSucceed) {
        QMessageBox messageBox;
        messageBox.setWindowTitle(tr("DBManager - Update finished!"));
        messageBox.setText(tr("Update finished!"));
        messageBox.setInformativeText(tr("Press ok to restart DBManager and finish the update process."));
        messageBox.setIcon(QMessageBox::Question);
        messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
//        messageBox.resize(600, 300);
//        messageBox.setBaseSize(QSize(600, 300));
//        messageBox.setFixedSize(600, 300);

        progressDialogUpdate->hide();

        if (QMessageBox::Ok == messageBox.exec()) {
            QString shellScriptPath = QCoreApplication::applicationDirPath()+"/../DBManager.sh";
            QString executablePath = QCoreApplication::applicationDirPath()+"/DBManager";

            if (QFileInfo::exists(shellScriptPath)) {
                if (!QProcess::startDetached("/bin/sh", QStringList{shellScriptPath})) {
                    qWarning() << "Failes to restart application!";
                } else {
                    QCoreApplication::quit();
                }
            } else if (QFileInfo::exists(executablePath)) {
                if (!QProcess::startDetached(executablePath)) {
                    qWarning() << "Failes to restart application!";
                } else {
                    QCoreApplication::quit();
                }
            } else {
                qWarning() << "Run Script not found! Automatic restart not possible";
            }
        }
    }
}

void UpdateManager::updateError(QNetworkReply::NetworkError error) {
    qCritical() << "UpdateManager::updateError" << error;
}

void UpdateManager::updateUpdateProgress(qint64 bytesRead, qint64 totalBytes) {
//    qDebug() << "UpdateManager::updateUpdateProgress: " << bytesRead << " von " << totalBytes;
    progressDialogUpdate->setValue(progressDialogUpdate->value() + bytesRead);
//    progressDialogUpdate->setValue(progressDialogUpdate->value()+1);
}

void UpdateManager::updateCanceled() {
//    qDebug() << "UpdateManager::updateCanceled";
    for (FileDownloader* fileDownloader : fileDownloaderCollection) {
        fileDownloader->abort();
    }
}

void UpdateManager::updateTimedOut() {
//    qDebug() << "UpdateManager::updateTimedOut";
}

UpdateManager::~UpdateManager() {
    delete progressDialogUpdate;

    for (FileDownloader* fileDownloader : this->fileDownloaderCollection) {
        delete fileDownloader;
    }
}
