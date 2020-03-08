#ifndef RELEASENOTE_H
#define RELEASENOTE_H

#include <QHBoxLayout>
#include <QTextEdit>
#include <QString>
#include <QDate>
#include <QLabel>

class ReleaseNote : public QWidget
{
    Q_OBJECT
public:
    explicit ReleaseNote(const QString &name, const QString& version, const QString &content, const QDate &releaseDate, QWidget *parent = nullptr);
    explicit ReleaseNote(const QString &name, const QString& version, const QString &content, const QString &releaseDate, QWidget *parent = nullptr);
    static int notesCount;

signals:

public slots:

private:
    QDate convertReleaseDateString(const QString& releaseDateString);
    void init(const QString&, const QString& version, const QString&, const QDate&);
    QLabel* createTitleElement(const QDate& releaseDate);
    QHBoxLayout* createNameElement(const QString& name, const QString& version);
    QLabel* createContentElement(const QString& content);
    QString name;
    QString version;
    QString title;
    QString content;
    QString releaseDateStr;
    QDate releaseDate;
};

#endif // RELEASENOTE_H
