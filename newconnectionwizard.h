#ifndef NEWCONNECTIONWIZARD_H
#define NEWCONNECTIONWIZARD_H

#include <QWizard>
#include <QSqlDatabase>

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
    void setDatabase(QSqlDatabase* database);
    QSqlDatabase* getDatabase();
    NewConnectionWizard(QWidget* parent = nullptr);
//    void done(int result) override;
private:
    QSqlDatabase* database = NULL;
};

#endif // NEWCONNECTIONWIZARD_H
