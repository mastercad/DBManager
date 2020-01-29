#include "connectionwizardmysqlpage.h"
#include "newconnectionwizard.h"

#include <QWizardPage>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSqlDatabase>

#include <QDebug>

ConnectionWizardMysqlPage::ConnectionWizardMysqlPage(QWidget* parent)
    : QWizardPage(parent)
{
    setTitle("Mysql Connection");
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/logos/mysql.png"));

    database = QSqlDatabase::addDatabase("QMYSQL", "mysql");

//    topLabel = new QLabel(tr("On this page you can provide your MySQL Connection details."));
//    topLabel->setWordWrap(true);

    QLabel* nameLabel = new QLabel(tr("Name:"));
    QLabel* hostLabel = new QLabel(tr("Host:"));
    QLabel* portLabel = new QLabel(tr("Port:"));
    QLabel* userLabel = new QLabel(tr("User:"));
    QLabel* passwordLabel = new QLabel(tr("Password:"));
    QLabel* databaseLabel = new QLabel(tr("Database:"));

    nameEdit = new QLineEdit;
    hostEdit = new QLineEdit;
    portEdit = new QLineEdit;
    userEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    databaseEdit = new QLineEdit;
    btnTestConnection = new QPushButton(tr("&Test connection"));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameEdit, 0, 1);
    layout->addWidget(hostLabel, 1, 0);
    layout->addWidget(hostEdit, 1, 1);
    layout->addWidget(portLabel, 1, 2);
    layout->addWidget(portEdit, 1, 3);
    layout->addWidget(userLabel, 2, 0);
    layout->addWidget(userEdit, 2, 1);
    layout->addWidget(passwordLabel, 3, 0);
    layout->addWidget(passwordEdit, 3, 1);
    layout->addWidget(databaseLabel, 4, 0);
    layout->addWidget(databaseEdit, 4, 1);
    layout->addWidget(btnTestConnection, 5, 0);

    setLayout(layout);

    registerField("connection.name", nameEdit);
    registerField("mysql.host", hostEdit);
    registerField("mysql.port", portEdit);
    registerField("mysql.user", userEdit);
    registerField("mysql.password", passwordEdit);
    registerField("mysql.database", databaseEdit);

    connect(btnTestConnection, SIGNAL(clicked(bool)), this, SLOT(testConnection()));

    if (isVisible()) {
        wizard()->button(QWizard::NextButton)->setVisible(false);
    }
}

bool ConnectionWizardMysqlPage::testConnection() {
    database.close();
    database.setHostName(hostEdit->text());

    if (!databaseEdit->text().isEmpty()) {
        database.setDatabaseName(databaseEdit->text());
    }
    database.setUserName(userEdit->text());
    database.setPassword(passwordEdit->text());
    database.open();

    if (database.isOpen()) {
        wizard()->button(QWizard::NextButton)->setEnabled(true);
        wizard()->button(QWizard::FinishButton)->setEnabled(true);
        return true;
    }
    wizard()->button(QWizard::NextButton)->setEnabled(false);
    wizard()->button(QWizard::FinishButton)->setEnabled(false);
    return false;
}

bool ConnectionWizardMysqlPage::isComplete() const {
    return database.isOpen();
}

int ConnectionWizardMysqlPage::nextId() const {
//    return NewConnectionWizard::Page_Finish;
    return -1;
}
