#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "releasenote.h"

#include <QAction>
#include <QWidget>

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    void addReleaseNote(ReleaseNote *releaseNote);
    ReleaseNote *addReleaseNote(const QString &title, const QString &content, const QDate &releaseDate);
    QSize minimumSizeHint() const;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent * event);

    ReleaseNote *releaseNoteAt(const QPoint &at);
private:
    QList<ReleaseNote *> releaseNotes;

    ReleaseNote *checkedReleaseNote;
    ReleaseNote *overReleaseNote;
};

#endif // SIDEBAR_H
