#ifndef CONNECTIONWIZARDINTROPAGE_H
#define CONNECTIONWIZARDINTROPAGE_H

#include <QWizardPage>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>

class ConnectionWizardIntroPage : public QWizardPage
{
    Q_OBJECT

public:
    enum {
        Type_Mysql,
        Type_Sqlite
    };

    ConnectionWizardIntroPage(QWidget* parent = nullptr);

    int nextId() const override;

private slots:
    void selectMysqlConnection();
    void selectSqliteConnection();

private:
    QLabel* topLabel;
    QPushButton* btnMysqlConnection;
    QPushButton* btnSqliteConnection;
    int selectedType = -1;
};

#endif // CONNECTIONWIZARDINTROPAGE_H
