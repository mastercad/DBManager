#include "releasenotesparser.h"

ReleaseNotesParser::ReleaseNotesParser()
{
}

void ReleaseNotesParser::parse(QNetworkReply* reply) {

}

ReleaseNote* ReleaseNotesParser::createReleaseNote(const QString& name, const QString& content, const QString& date) {
    return new ReleaseNote(name, content, date);
}
