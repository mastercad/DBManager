#ifndef RELEASENOTESPARSER_H
#define RELEASENOTESPARSER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "releasenote.h"

class ReleaseNotesParser: public QObject
{
    Q_OBJECT
public:
    ReleaseNotesParser();
    void parse(QNetworkReply*);
private:
    QNetworkAccessManager networkManager;
    ReleaseNote* createReleaseNote(const QString& name, const QString& version, const QString &content, const QString& date);
};

#endif // RELEASENOTESPARSER_H
