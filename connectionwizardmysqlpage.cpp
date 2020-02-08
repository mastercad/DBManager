#include "connectionwizardmysqlpage.h"
#include "newconnectionwizard.h"
#include "connectionfactory.h"
#include "connection.h"
#include "connectioninfofactory.h"
#include "defaults.h"
#include "mysqlconnectionvalidator.h"

#include <QWizardPage>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlError>

#include <QDebug>

ConnectionWizardMysqlPage::ConnectionWizardMysqlPage(QWidget* parent)
    : QWizardPage(parent)
{
    setTitle("Mysql Connection");
//    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/logos/mysql.png"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/logos/mysql_small.png"));

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
    nameEdit->setPlaceholderText(tr("Define name for new connection"));
    hostEdit = new QLineEdit;
    hostEdit->setPlaceholderText(Defaults::MYSQL::HOST);
    portEdit = new QLineEdit;
    portEdit->setPlaceholderText(QString::number(Defaults::MYSQL::PORT));
    userEdit = new QLineEdit;
    userEdit->setPlaceholderText(Defaults::MYSQL::USER);
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText(Defaults::MYSQL::PASSWORD);

    mysqlConnectionValidEdit = new QLineEdit;
    mysqlConnectionValidEdit->setText("NO");

    databaseEdit = new QLineEdit;
    btnTestConnection = new QPushButton(tr("&Test connection"));

    informationTextEdit = new QTextEdit;
    informationTextEdit->setDisabled(true);
    informationTextEdit->hide();

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0, 1, 1);
    layout->addWidget(nameEdit, 0, 1, 1, 3);
    layout->addWidget(hostLabel, 1, 0, 1, 1);
    layout->addWidget(hostEdit, 1, 1, 1, 1);
    layout->addWidget(portLabel, 1, 2, 1, 1);
    layout->addWidget(portEdit, 1, 3, 1, 1);
    layout->addWidget(userLabel, 2, 0, 1, 1);
    layout->addWidget(userEdit, 2, 1, 1, 3);
    layout->addWidget(passwordLabel, 3, 0, 1, 1);
    layout->addWidget(passwordEdit, 3, 1, 1, 3);
    layout->addWidget(databaseLabel, 4, 0, 1, 1);
    layout->addWidget(databaseEdit, 4, 1, 1, 3);
    layout->addWidget(btnTestConnection, 5, 0, 1, 4);
    layout->addWidget(informationTextEdit, 6, 0, 1, 4);

    setLayout(layout);

    /*
    registerField("mysql.connection.name", nameEdit, "New Connection", "textChanged");
    registerField("mysql.connection.valid", mysqlConnectionValidEdit, "NO", "textChanged");
    registerField("mysql.host", hostEdit, "localhost", "textChanged");
    registerField("mysql.port", portEdit, "3306", "textChanged");
    registerField("mysql.user", userEdit, "root", "textChanged");
    registerField("mysql.password", passwordEdit, "root", "textChanged");
    registerField("mysql.database", databaseEdit, "", "textChanged");
    */
    registerField("mysql.connection.name", nameEdit);
    registerField("mysql.connection.valid", mysqlConnectionValidEdit);
    registerField("mysql.host", hostEdit);
    registerField("mysql.port", portEdit);
    registerField("mysql.user", userEdit);
    registerField("mysql.password", passwordEdit);
    registerField("mysql.database", databaseEdit);

    connect(btnTestConnection, SIGNAL(clicked(bool)), this, SLOT(validatePage()));

    if (isVisible()) {
        wizard()->button(QWizard::NextButton)->setVisible(false);
        wizard()->button(QWizard::NextButton)->setEnabled(false);
        wizard()->button(QWizard::FinishButton)->setEnabled(false);
    }
}

bool ConnectionWizardMysqlPage::validatePage() {
    ConnectionInfoFactory connectionInfoFactory;
    ConnectionInfo* connectionInfo = connectionInfoFactory.create(this);
    ConnectionFactory connectionFactory;
    Connection* connection = connectionFactory.create(connectionInfo);
    MysqlConnectionValidator connectionValidator;
    bool result = false;

    if (connectionValidator.validate(connection)) {
        wizard()->button(QWizard::NextButton)->setEnabled(true);
        wizard()->button(QWizard::FinishButton)->setEnabled(true);
        mysqlConnectionValidEdit->setText("YES");
        informationTextEdit->setText("OK.");
        informationTextEdit->hide();

//        setField("mysql.connection.valid", mysqlConnectionValidEdit);
        setField("mysql.connection.valid", "YES");
        result = true;
    } else {
        wizard()->button(QWizard::NextButton)->setEnabled(false);
        wizard()->button(QWizard::FinishButton)->setEnabled(false);
        mysqlConnectionValidEdit->setText("NO");
        informationTextEdit->setText(connection->lastError().databaseText());
        informationTextEdit->show();
    //    setField("mysql.connection.valid", mysqlConnectionValidEdit);
        setField("mysql.connection.valid", "NO");
    }

    delete connectionInfo;
    delete connection;

    return result;
}

bool ConnectionWizardMysqlPage::isComplete() const {
    return database.isOpen();
}

int ConnectionWizardMysqlPage::nextId() const {
//    return NewConnectionWizard::Page_Finish;
    return -1;
}

void ConnectionWizardMysqlPage::setNameEdit(QLineEdit* const nameEdit){
    this->nameEdit = nameEdit;
}

QLineEdit* ConnectionWizardMysqlPage::getNameEdit() const {
    return this->nameEdit;
}

void ConnectionWizardMysqlPage::setHostEdit(QLineEdit* const hostEdit) {
    this->hostEdit = hostEdit;
}

QLineEdit* ConnectionWizardMysqlPage::getHostEdit() const {
    return this->hostEdit;
}

void ConnectionWizardMysqlPage::setPortEdit(QLineEdit* const portEdit) {
    this->portEdit = portEdit;
}

QLineEdit* ConnectionWizardMysqlPage::getPortEdit() const {
    return this->portEdit;
}

void ConnectionWizardMysqlPage::setUserEdit(QLineEdit* const userEdit) {
    this->userEdit = userEdit;
}

QLineEdit* ConnectionWizardMysqlPage::getUserEdit() const {
    return this->userEdit;
}

void ConnectionWizardMysqlPage::setPasswordEdit(QLineEdit* const passwordEdit) {
    this->passwordEdit = passwordEdit;
}

QLineEdit* ConnectionWizardMysqlPage::getPasswordEdit() const {
    return this->passwordEdit;
}

void ConnectionWizardMysqlPage::setDatabaseEdit(QLineEdit* const databaseEdit) {
    this->databaseEdit = databaseEdit;
}

QLineEdit* ConnectionWizardMysqlPage::getDatabaseEdit() const {
    return this->databaseEdit;
}
