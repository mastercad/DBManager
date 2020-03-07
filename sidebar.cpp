#include "sidebar.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QEvent>

#include <QDebug>

#define action_height 90

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    checkedReleaseNote(nullptr),
    overReleaseNote(nullptr)
{
    setMouseTracking(true);
}


void SideBar::paintEvent(QPaintEvent *event) {
    qDebug() << "PaintEvent!";
    QPainter p(this);

    QFont fontText(p.font());
    fontText.setFamily("Helvetica Neue");
    p.setFont(fontText);

    int action_y = 0;
    p.fillRect(rect(), QColor(255, 255, 255));

    for(auto releaseNote: releaseNotes) {
        QRect actionRect(0, action_y, event->rect().width(), action_height);

//        if(releaseNote->isChecked()) {
//            p.fillRect(actionRect, QColor(35, 35, 35));
//        }

        if(releaseNote == overReleaseNote) {
            p.fillRect(actionRect, QColor(150, 150, 150));
        }

        p.setPen(QColor(123, 123, 123));
//        QSize size = p.fontMetrics().size(Qt::TextSingleLine, releaseNote->toPlainText());
        QSize size = releaseNote->size();
        QRect actionTextRect(QPoint(actionRect.width()/2 - size.width()/2, actionRect.bottom()-size.height()-5), size);
//        p.drawText(actionTextRect, Qt::AlignCenter, releaseNote->toPlainText());

//        QRect actionIconRect(0, action_y + 10, actionRect.width(), actionRect.height()-2*actionTextRect.height()-10);
//        QIcon  actionIcon(releaseNote->icon());
//        actionIcon.paint(&p, actionIconRect);

        action_y += actionRect.height();
    }
}

QSize SideBar::minimumSizeHint() const
{
    return action_height*QSize(1, releaseNotes.size());
}

void SideBar::addReleaseNote(ReleaseNote* releaseNote)
{
    releaseNotes.push_back(releaseNote);
//    releaseNote->setCheckable(true);
    qDebug() << "SideBar::addReleaseNote update()";
    update();
}

ReleaseNote *SideBar::addReleaseNote(const QString &title, const QString &content, const QDate &releaseDate) {
    ReleaseNote *releaseNote = new ReleaseNote(title, content, releaseDate, this);
//    action->setCheckable(true);
    releaseNotes.push_back(releaseNote);
    update();
    return releaseNote;
}

void SideBar::mousePressEvent(QMouseEvent *event) {
    ReleaseNote* tempReleaseNote = releaseNoteAt(event->pos());
    if(tempReleaseNote == nullptr) // || tempReleaseNote->isChecked())
        return;

    qDebug()<<"clicked";
//    if(checkedReleaseNote)
//        checkedReleaseNote->setChecked(false);
    if(overReleaseNote == tempReleaseNote)
        overReleaseNote = nullptr;
    checkedReleaseNote = tempReleaseNote;
//    tempReleaseNote->setChecked(true);
    update();
    QWidget::mousePressEvent(event);
}


void SideBar::mouseMoveEvent(QMouseEvent *event)
{
    ReleaseNote* tempReleaseNote = releaseNoteAt(event->pos());
    if(tempReleaseNote == nullptr){
        overReleaseNote = nullptr;
        update();
        return;
    }
//    if(tempReleaseNote->isChecked() || overReleaseNote == tempReleaseNote)
    if(overReleaseNote == tempReleaseNote)
        return;

    overReleaseNote = tempReleaseNote;
    update();
    QWidget::mouseMoveEvent(event);
}

void SideBar::leaveEvent(QEvent * event)
{
    overReleaseNote = nullptr;
    update();
    QWidget::leaveEvent(event);
}

ReleaseNote* SideBar::releaseNoteAt(const QPoint &at)
{
    int action_y = 0;
    for(auto releaseNote: releaseNotes)
    {
        QRect actionRect(0, action_y, rect().width(), action_height);
        if(actionRect.contains(at))
            return releaseNote;
        action_y += actionRect.height();
    }
    return nullptr;
}

#undef action_height
