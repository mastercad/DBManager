#include "newconnectionwizard.h"
#include "connectionwizardintropage.h"
#include "connectionwizardmysqlpage.h"
#include "connectionwizardsqlitepage.h"

#include <QGridLayout>

#include <QDebug>

NewConnectionWizard::NewConnectionWizard(QWidget* parent)
    : QWizard(parent)
{
    setPage(Page_Intro, new ConnectionWizardIntroPage);
    setPage(Page_Mysql, new ConnectionWizardMysqlPage);
    setPage(Page_Sqlite, new ConnectionWizardSqlitePage);

    setStartId(Page_Intro);

#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif

    setWindowTitle(tr("New Connection-Wizard"));
    int width = 600, height = 400;

    resize(width, height);
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

void NewConnectionWizard::initializePage(int id) {
    this->page(id)->initializePage();

    this->button(QWizard::NextButton)->setText(tr("&Next"));
    this->button(QWizard::CancelButton)->setText(tr("Cancel"));
    this->button(QWizard::BackButton)->setText(tr("&Back"));
    this->button(QWizard::FinishButton)->setText(tr("&Finish"));
}

void NewConnectionWizard::setDatabase(QSqlDatabase* database) {
    this->database = database;
}

QSqlDatabase* NewConnectionWizard::getDatabase() {
    return this->database;
}
