#ifndef RELEASENOTESMANAGER_H
#define RELEASENOTESMANAGER_H

#include "releasenote.h"
#include "releasenotesparser.h"
#include "releasenotesdownloader.h"

#include <QList>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QNetworkReply>

class ReleaseNotesManager: public QObject
{
    Q_OBJECT

public:
    ReleaseNotesManager(QWidget* sideBarContainer, QVBoxLayout* sideBarLayout, QWidget* parent = nullptr);

public slots:
    void createReleaseNotes(QNetworkReply* reply);
    QNetworkReply* refresh();
    void toggle();

private:
    ReleaseNotesDownloader releaseNotesDownloader;
    QWidget* sideBarContainer = nullptr;
//    QVBoxLayout* sideBarLayout = nullptr;
    QVBoxLayout* sideBarLayout = nullptr;
    QList<ReleaseNote*> releaseNotes;
    QNetworkReply* currentReply = nullptr;
    bool notesRead = false;
};

#endif // RELEASENOTESMANAGER_H
