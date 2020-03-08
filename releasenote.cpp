#include "releasenote.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QScrollArea>
#include <QStyle>
#include <QLabel>

#include "defaults.h"

#include <QDebug>

int ReleaseNote::notesCount = 0;

ReleaseNote::ReleaseNote(const QString &name, const QString& version, const QString &content, const QDate &releaseDate, QWidget *parent) :
    QWidget(parent),
    name(name),
    content(content),
    releaseDate(releaseDate)
{
    ++ReleaseNote::notesCount;
    init(name, version, content, releaseDate);
}

ReleaseNote::ReleaseNote(const QString &name, const QString& version, const QString &content, const QString &releaseDateStr, QWidget *parent) :
    QWidget(parent),
    name(name),
    content(content),
    releaseDateStr(releaseDateStr)
{
    ++ReleaseNote::notesCount;
    releaseDate = convertReleaseDateString(releaseDateStr);
    init(name, version, content, releaseDate);
}

void ReleaseNote::init(const QString& name, const QString& version, const QString& content, const QDate& releaseDate) {
    // give name to have a possibility to call with specific stylesheet, but seems this does not work, nor ReleaseNote either #releaseNote_1
    this->setObjectName(QString("releaseNote_") + QString::number(ReleaseNote::notesCount));

    QVBoxLayout* layoutContainer = new QVBoxLayout;

    layoutContainer->addWidget(createTitleElement(releaseDate));

    layoutContainer->addLayout(createNameElement(name, version));

    layoutContainer->addWidget(createContentElement(content));

    layoutContainer->setSpacing(0);

    this->setLayout(layoutContainer);
}

QLabel* ReleaseNote::createTitleElement(const QDate& releaseDate) {
    QLabel* titleLabel = new QLabel(this);
    titleLabel->setText("DBManager Update "+releaseDate.toString(tr("dd.MM.yyyy")));
    titleLabel->setStyleSheet("font-size: 16px; padding-top: 20px; padding-right: 20px; padding-left: 20px; padding-bottom: 10px; font-weight: bold; background-color: white; margin-bottom: 0; border-top: 1px solid lightgrey; border-left: 1px solid lightgrey; border-right: 1px solid lightgrey;");

    return titleLabel;
}

QHBoxLayout* ReleaseNote::createNameElement(const QString& name, const QString& version) {
    QHBoxLayout* nameLayout = new QHBoxLayout;
    QIcon* nameIcon = new QIcon(":/icons/programm_icon.png");

    QLabel* nameIconLabel = new QLabel;
    nameIconLabel->setPixmap(nameIcon->pixmap(QSize(32, 32)));
    nameIconLabel->setStyleSheet("width: 40px;");
    nameIconLabel->setStyleSheet("padding-top: 0; padding-right: 0; padding-left: 20px; padding-bottom: 0; font-weight: bold; background-color: white; margin-bottom: 0; border-left: 1px solid lightgrey; border-right: 0;");

    QLabel* nameLabel = new QLabel;
    nameLabel->setText(name);
    nameLabel->setWordWrap(true);
    nameLabel->setStyleSheet("padding-top: 0; padding-right: 20px; padding-left: 0; padding-bottom: 0; font-weight: bold; background-color: white; margin-bottom: 0; border-left: 0; border-right: 1px solid lightgrey;");
    nameLabel->setAlignment(Qt::AlignLeft);

    nameLayout->addWidget(nameIconLabel);

    QVBoxLayout* versionInformationLayout = new QVBoxLayout;
    QLabel* versionNumberLabel = new QLabel;
    versionNumberLabel->setText("Version "+version);
    versionNumberLabel->setStyleSheet("font-size: 9px; font-style: italic; color: grey; italic; padding-top: 0; padding-right: 20px; padding-left: 0; padding-bottom: 0; font-weight: bold; background-color: white; margin-bottom: 0; border-left: 0; border-right: 1px solid lightgrey;");

    versionInformationLayout->addWidget(nameLabel);
    versionInformationLayout->addWidget(versionNumberLabel);

    nameLayout->addLayout(versionInformationLayout);

    nameLayout->setStretch(0, 1);
    nameLayout->setStretch(1, 4);

    return nameLayout;
}

QLabel* ReleaseNote::createContentElement(const QString& content) {
    QLabel* contentLabel = new QLabel;
    contentLabel->setStyleSheet("padding-top: 10; padding-right: 20px; padding-left: 20px; padding-bottom: 20px; background-color: white; margin-top: 0; border-bottom: 1px solid lightgrey; border-left: 1px solid lightgrey; border-right: 1px solid lightgrey;");
    contentLabel->setText(content);
    contentLabel->setTextFormat(Qt::RichText);
    contentLabel->setWordWrap(true);

    return contentLabel;
}

QDate ReleaseNote::convertReleaseDateString(const QString& releaseDateString) {
    return QDate::fromString(releaseDateString, "dd.MM.yyyy");
}
