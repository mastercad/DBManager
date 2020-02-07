#ifndef MYSQLCONNECTIONEDITWIDGET_H
#define MYSQLCONNECTIONEDITWIDGET_H

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
//    explicit MysqlConnectionEditWidget(QWidget *parent = 0, QMap<QString, QMap<QString, ConnectionInfo*> >* connections = nullptr);
    explicit MysqlConnectionEditWidget(QWidget *parent = 0, Connections* connections = nullptr);
    ~MysqlConnectionEditWidget();

    void parseConnectionInfo(ConnectionInfo* connectionInfo);

public slots:
    void save();

private:
    QWidget* parent = NULL;
//    QMap<QString, QMap<QString, ConnectionInfo*> >* connections = NULL;
    Connections* connections = NULL;
    Ui::MysqlConnectionEditWidget *ui;
    ConnectionInfo* currentConnectionInfo = NULL;
};

#endif // MYSQLCONNECTIONEDITWIDGET_H
