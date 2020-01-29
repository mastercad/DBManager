#include "connectionwizardfinalpage.h"
#include "newconnectionwizard.h"

#include <QWizardPage>
#include <QVBoxLayout>

ConnectionWizardFinalPage::ConnectionWizardFinalPage(QWidget* parent)
    : QWizardPage(parent)
{
    setTitle("Final");
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/logos/mysql.png"));

    topLabel = new QLabel(tr("This wizard will help you register your copy of"));
    topLabel->setWordWrap(true);

    btnMysqlConnection = new QPushButton();
    btnMysqlConnection->setIcon(QIcon(QPixmap(":/logos/mysql.png")));
    btnMysqlConnection->setIconSize(QSize(64, 64));
    btnSqliteConnection = new QPushButton();
    btnSqliteConnection->setIcon(QIcon(QPixmap(":/logos/sqlite.png")));
    btnSqliteConnection->setIconSize(QSize(64, 64));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(btnMysqlConnection);
    layout->addWidget(btnSqliteConnection);
    setLayout(layout);
}

int ConnectionWizardFinalPage::nextId() const {
    return -1;
}
