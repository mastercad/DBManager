#ifndef CONNECTIONWIZARDMYSQLPAGE_H
#define CONNECTIONWIZARDMYSQLPAGE_H

#include <QWizardPage>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QSqlDatabase>

class ConnectionWizardMysqlPage : public QWizardPage
{
    Q_OBJECT

public:
    enum {
        Type_Mysql,
        Type_Sqlite
    };

    ConnectionWizardMysqlPage(QWidget* parent = nullptr);
    bool isComplete() const;

    int nextId() const override;

public slots:
    bool validatePage();

private:
    QLineEdit* nameEdit = NULL;
    QLineEdit* hostEdit = NULL;
    QLineEdit* portEdit = NULL;
    QLineEdit* userEdit = NULL;
    QLineEdit* passwordEdit = NULL;
    QLineEdit* databaseEdit = NULL;
    QLineEdit* mysqlConnectionValidEdit = NULL;
    QTextEdit* informationTextEdit = NULL;
    QPushButton* btnTestConnection = new QPushButton(tr("&Test connection"));
    int selectedType = -1;
    QSqlDatabase database;
};

#endif // CONNECTIONWIZARDMYSQLPAGE_H
