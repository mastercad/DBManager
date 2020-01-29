#ifndef NEWCONNECTIONWIZARD_H
#define NEWCONNECTIONWIZARD_H

#include <QWizard>

class NewConnectionWizard : public QWizard
{
    Q_OBJECT

public:
    enum {
        Page_Intro,
        Page_Mysql,
        Page_Sqlite,
        Page_Finish
    };
    NewConnectionWizard(QWidget* parent = nullptr);
};

#endif // NEWCONNECTIONWIZARD_H
