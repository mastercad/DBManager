#include "connectionwithlogindialog.h"
#include "ui_connectionwithlogindialog.h"

ConnectionWithLoginDialog::ConnectionWithLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionWithLoginDialog)
{
    ui->setupUi(this);
}

QString ConnectionWithLoginDialog::getHostName() const {
    return ui->hostname->text();
}

void ConnectionWithLoginDialog::setHostName(const QString hostName) {
    ui->hostname->setText(hostName);
}

QString ConnectionWithLoginDialog::getPort() const {
    return ui->port->text();
}

void ConnectionWithLoginDialog::setPort(const QString port) {
    ui->port->setText(port);
}

QString ConnectionWithLoginDialog::getUserName() const {
    return ui->user->text();
}

void ConnectionWithLoginDialog::setUserName(const QString userName) {
    ui->user->setText(userName);
}

QString ConnectionWithLoginDialog::getPassword() const {
    return ui->password->text();
}

void ConnectionWithLoginDialog::setPassword(const QString password) {
    ui->password->setText(password);
}

ConnectionWithLoginDialog::~ConnectionWithLoginDialog()
{
    delete ui;
}
