#ifndef CONNECTIONWIZARDFINALPAGE_H
#define CONNECTIONWIZARDFINALPAGE_H

#include <QWizardPage>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>

class ConnectionWizardFinalPage : public QWizardPage
{
    Q_OBJECT

public:
    enum {
        Type_Mysql,
        Type_Sqlite
    };

    ConnectionWizardFinalPage(QWidget* parent = nullptr);

    int nextId() const override;

private:
    QLabel* topLabel;
    QPushButton* btnMysqlConnection;
    QPushButton* btnSqliteConnection;
    int selectedType = -1;
};

#endif // CONNECTIONWIZARDFINALPAGE_H
