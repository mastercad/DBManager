#ifndef MYSQLCONNECTIONEDITWIDGET_H
#define MYSQLCONNECTIONEDITWIDGET_H

#include "connectionmanager.h"
#include "connectioninfo.h"
#include "connections.h"

#include <QWidget>
#include <QMap>
#include <QString>

namespace Ui {
class MysqlConnectionEditWidget;
}

class MysqlConnectionEditWidget : public QWidget
{
    Q_OBJECT

public:
//    explicit MysqlConnectionEditWidget(QWidget *parent = nullptr, QMap<QString, QMap<QString, ConnectionInfo*> >* connections = nullptr);
    explicit MysqlConnectionEditWidget(ConnectionManager *parent = nullptr, Connections* connections = nullptr);
    ~MysqlConnectionEditWidget();

    void parseConnectionInfo(ConnectionInfo* connectionInfo);

public slots:
    void save();
    bool testConnection();

private:
    ConnectionInfo* generateConnectionInfo();

    ConnectionManager* parent = nullptr;
//    QMap<QString, QMap<QString, ConnectionInfo*> >* connections = NULL;
    Connections* connections = nullptr;
    Ui::MysqlConnectionEditWidget *ui;
    ConnectionInfo* currentConnectionInfo = nullptr;
};

#endif // MYSQLCONNECTIONEDITWIDGET_H
