#ifndef NEWCONNECTIONWIZARD_H
#define NEWCONNECTIONWIZARD_H

#include <QWizard>
#include <QSqlDatabase>

class NewConnectionWizard : public QWizard {
    Q_OBJECT

public:
    NewConnectionWizard(QWidget* parent = nullptr);

    enum {
        Page_Intro,
        Page_Mysql,
        Page_Sqlite,
        Page_Finish
    };

    void setDatabase(QSqlDatabase* database);
    QSqlDatabase* getDatabase();
//    void done(int result) override;

protected:
    void initializePage(int id);

private:
    QSqlDatabase* database = nullptr;
};

#endif // NEWCONNECTIONWIZARD_H
