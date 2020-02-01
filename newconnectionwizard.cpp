#include "newconnectionwizard.h"
#include "connectionwizardintropage.h"
#include "connectionwizardmysqlpage.h"
#include "connectionwizardsqlitepage.h"
#include "connectionwizardfinalpage.h"

#include <QGridLayout>

#include <QDebug>

NewConnectionWizard::NewConnectionWizard(QWidget* parent)
    : QWizard(parent)
{
    setPage(Page_Intro, new ConnectionWizardIntroPage);
    setPage(Page_Mysql, new ConnectionWizardMysqlPage);
    setPage(Page_Sqlite, new ConnectionWizardSqlitePage);
    setPage(Page_Finish, new ConnectionWizardFinalPage);

    setStartId(Page_Intro);

#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif

    setWindowTitle(tr("New Connection-Wizard"));
/*
    QWidget * sideWidget = new QWidget();
    QGridLayout * gridLayout = new QGridLayout();
    QLabel * label = new QLabel();
    label->setPixmap(QPixmap(":/logos/mysql.png"));
    sideWidget->setLayout(gridLayout);
    gridLayout->addWidget(label);
    setSideWidget(sideWidget);
*/
}

//void NewConnectionWizard::done(int state) {
//    qDebug() << "Done State: " << state;
//}

void NewConnectionWizard::setDatabase(QSqlDatabase* database) {
    this->database = database;
}

QSqlDatabase* NewConnectionWizard::getDatabase() {
    return this->database;
}
