#ifndef CONNECTIONWITHLOGINDIALOG_H
#define CONNECTIONWITHLOGINDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class ConnectionWithLoginDialog;
}

class ConnectionWithLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionWithLoginDialog(QWidget *parent = 0);
    ~ConnectionWithLoginDialog();

    QString getHostName() const;
    QString getPort() const;
    QString getUserName() const;
    QString getPassword() const;

    void setHostName(QString const);
    void setPort(QString const);
    void setUserName(QString const);
    void setPassword(QString const);

private:
    Ui::ConnectionWithLoginDialog *ui;
};

#endif // CONNECTIONWITHLOGINDIALOG_H
