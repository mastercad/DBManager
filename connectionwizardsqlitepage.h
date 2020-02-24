#ifndef CONNECTIONWIZARDSQLITEPAGE_H
#define CONNECTIONWIZARDSQLITEPAGE_H

#include <QWizardPage>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSqlDatabase>
#include <QLineEdit>

class ConnectionWizardSqlitePage : public QWizardPage
{
    Q_OBJECT

public:
    enum {
        Type_Mysql,
        Type_Sqlite
    };

    ConnectionWizardSqlitePage(QWidget* parent = nullptr);

    bool isComplete() const;

    int nextId() const override;

public slots:
    void showFileDialog();
    bool validatePage();

private:
    QLineEdit* filePathNameEdit = nullptr;
    QLineEdit* nameEdit = nullptr;
    QLineEdit* sqliteConnectionValid = nullptr;
    QSqlDatabase database;
};

#endif // CONNECTIONWIZARDSQLITEPAGE_H
