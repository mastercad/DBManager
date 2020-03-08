#include "releasenotesmanager.h"

#include "releasenote.h"

#include <QPropertyAnimation>
#include <QXmlStreamReader>
#include <QPushButton>

#include <QDebug>

ReleaseNotesManager::ReleaseNotesManager(QWidget* sideBarContainer, QVBoxLayout* sideBarLayout, QWidget* parent) :
    QObject(parent),
    sideBarContainer(sideBarContainer),
    sideBarLayout(sideBarLayout)
{
    connect(&releaseNotesDownloader, SIGNAL(onFinished(QNetworkReply*)), this, SLOT(createReleaseNotes(QNetworkReply*)));
}

/**
 * create Release Notes Collection from given XML reply
 *
 * @todo this function should moved in the planned ReleaseNotesParser.
 *
 * @brief ReleaseNotesManager::createReleaseNotes
 * @param reply
 */
void ReleaseNotesManager::createReleaseNotes(QNetworkReply* reply) {
    QByteArray data = reply->readAll();

    QXmlStreamReader xmlReader(data);
    QString lastElementName;
    QString name;
    QString version;
    QString content;
    QString releaseDate;

    while (!xmlReader.atEnd()
        && !xmlReader.hasError()
    ) {
        xmlReader.readNext();
        QString elementName = xmlReader.name().toString();
        QString currentElementContent = xmlReader.text().toString().trimmed();

        if ("Name" == lastElementName
            && !currentElementContent.isEmpty()
        ) {
            name = currentElementContent;
        } else if ("Content" == lastElementName
            && !currentElementContent.isEmpty()
        ) {
            content = currentElementContent;
        } else if ("Date" == lastElementName
            && !currentElementContent.isEmpty()
        ) {
            releaseDate = currentElementContent;
        } else if ("Version" == lastElementName
           && !currentElementContent.isEmpty()
        ) {
            version = currentElementContent;
        }

        if ("ReleaseNote" == elementName
            && xmlReader.isEndElement()
        ) {
            ReleaseNote* releaseNote = new ReleaseNote(name, version, content, releaseDate);
            releaseNotes.append(releaseNote);
        }

        if (!elementName.isEmpty()) {
            lastElementName = xmlReader.name().toString();
        }
    }

    this->notesRead = true;
    toggle();
}

QNetworkReply* ReleaseNotesManager::refresh() {
    for(ReleaseNote* releaseNote: releaseNotes) {
        sideBarLayout->removeWidget(releaseNote);
    }
    releaseNotes.clear();
    currentReply = this->releaseNotesDownloader.download();

    return currentReply;
}

void ReleaseNotesManager::toggle() {
    if (false == notesRead) {

        if (nullptr != currentReply) {
            currentReply->abort();
        }
        refresh();
        return;
    }

    // initialize sideBar Container with releaseNotes
    if (sideBarLayout->isEmpty()
        && true == notesRead
    ) {
        QHBoxLayout* buttonsLayout = new QHBoxLayout;

        QPushButton* closeButton = new QPushButton("close");
        connect(closeButton, SIGNAL(clicked()), this, SLOT(toggle()));

        QPushButton* refreshButton = new QPushButton("refresh");
        connect(refreshButton, SIGNAL(clicked()), this, SLOT(toggle()));
        connect(refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

        buttonsLayout->addWidget(closeButton);
        buttonsLayout->addWidget(refreshButton);

        sideBarLayout->addLayout(buttonsLayout);

        int releaseNodeCount = 1;
        for(ReleaseNote* releaseNote: releaseNotes) {
            sideBarLayout->addWidget(releaseNote);
           ++releaseNodeCount;
        }
        sideBarLayout->setAlignment(Qt::AlignTop);

        sideBarContainer->showMaximized();
        sideBarContainer->setStyleSheet("background-color: white;");
    }

    if (350 == sideBarContainer->maximumWidth()) {
        QPropertyAnimation *animationMin = new QPropertyAnimation(sideBarContainer, "minimumWidth");
        animationMin->setDuration(250);
        animationMin->setStartValue(350);
        animationMin->setEndValue(0);
        animationMin->start();

        QPropertyAnimation *animationMax = new QPropertyAnimation(sideBarContainer, "maximumWidth");
        animationMax->setDuration(250);
        animationMax->setStartValue(350);
        animationMax->setEndValue(0);
        animationMax->start();
    } else {
        QPropertyAnimation *animationMin = new QPropertyAnimation(sideBarContainer, "minimumWidth");
        animationMin->setDuration(250);
        animationMin->setStartValue(0);
        animationMin->setEndValue(350);
        animationMin->start();

        QPropertyAnimation *animationMax = new QPropertyAnimation(sideBarContainer, "maximumWidth");
        animationMax->setDuration(250);
        animationMax->setStartValue(0);
        animationMax->setEndValue(350);
        animationMax->start();
    }
}
