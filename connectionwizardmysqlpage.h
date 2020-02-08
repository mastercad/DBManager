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

    bool isComplete() const override;

    int nextId() const override;

    void setNameEdit(QLineEdit* const);
    QLineEdit* getNameEdit() const;

    void setHostEdit(QLineEdit* const);
    QLineEdit* getHostEdit() const;

    void setPortEdit(QLineEdit* const);
    QLineEdit* getPortEdit() const;

    void setUserEdit(QLineEdit* const);
    QLineEdit* getUserEdit() const;

    void setPasswordEdit(QLineEdit* const);
    QLineEdit* getPasswordEdit() const;

    void setDatabaseEdit(QLineEdit* const);
    QLineEdit* getDatabaseEdit() const;

public slots:
    bool validatePage() override;

private:
    QLineEdit* nameEdit = nullptr;
    QLineEdit* hostEdit = nullptr;
    QLineEdit* portEdit = nullptr;
    QLineEdit* userEdit = nullptr;
    QLineEdit* passwordEdit = nullptr;
    QLineEdit* databaseEdit = nullptr;

    QLineEdit* mysqlConnectionValidEdit = nullptr;
    QTextEdit* informationTextEdit = nullptr;
    QPushButton* btnTestConnection = new QPushButton(tr("&Test connection"));
    int selectedType = -1;
    QSqlDatabase database;
};

#endif // CONNECTIONWIZARDMYSQLPAGE_H
