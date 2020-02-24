#include "connectionwizardintropage.h"
#include "newconnectionwizard.h"

#include <QWizardPage>
#include <QVBoxLayout>
#include <QCoreApplication>

#include <QDebug>

ConnectionWizardIntroPage::ConnectionWizardIntroPage(QWidget* parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/logos/mysql.png"));

//    setPixmap(QWizard::LogoPixmap, QPixmap(":/logos/mysql.png"));

    topLabel = new QLabel(tr("This wizard help you to create new connection to database."));
    topLabel->setWordWrap(true);

    btnMysqlConnection = new QPushButton();
    btnMysqlConnection->setIcon(QIcon(QPixmap(":/logos/mysql.png")));
    btnMysqlConnection->setIconSize(QSize(64, 64));
    btnSqliteConnection = new QPushButton();
    btnSqliteConnection->setIcon(QIcon(QPixmap(":/logos/sqlite.png")));
    btnSqliteConnection->setIconSize(QSize(64, 64));
//    btnSqliteConnection->setStyleSheet("border: 1px solid blue");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(btnMysqlConnection);
    layout->addWidget(btnSqliteConnection);
    setLayout(layout);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    if (isVisible()) {
        qDebug() << "VISIBLE!";
        wizard()->button(QWizard::NextButton)->setText(QCoreApplication::translate("ConnectionWizardIntroPage", "&Next"));
        wizard()->button(QWizard::CancelButton)->setText(tr("Cancel"));
        wizard()->button(QWizard::BackButton)->setText(tr("&Back"));
        wizard()->button(QWizard::FinishButton)->setText(tr("&Finish"));
    }

    connect(btnMysqlConnection, SIGNAL(clicked(bool)), this, SLOT(selectMysqlConnection()));
    connect(btnSqliteConnection, SIGNAL(clicked(bool)), this, SLOT(selectSqliteConnection()));
}

void ConnectionWizardIntroPage::selectMysqlConnection() {
//    if (Type_Mysql == selectedType) {
//        selectedType = -1;
//    } else {
        selectedType = Type_Mysql;
        wizard()->next();
//    }
}

void ConnectionWizardIntroPage::selectSqliteConnection() {
//    if (Type_Sqlite == selectedType) {
//        selectedType = -1;
//    } else {
        selectedType = Type_Sqlite;
        wizard()->next();
//    }
}

int ConnectionWizardIntroPage::nextId() const {
    if (Type_Sqlite == selectedType) {
        return NewConnectionWizard::Page_Sqlite;
    }
//    } else if (Type_Sqlite == selectedType) {
    // default next page
        return NewConnectionWizard::Page_Mysql;
//    }
}
