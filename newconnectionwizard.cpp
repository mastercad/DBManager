#include "newconnectionwizard.h"
#include "connectionwizardintropage.h"
#include "connectionwizardmysqlpage.h"
#include "connectionwizardsqlitepage.h"
#include "connectionwizardfinalpage.h"

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
}
