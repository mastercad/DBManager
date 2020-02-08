#include "connectionwizardsqlitepage.h"
#include "newconnectionwizard.h"

#include <QWizardPage>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>

#include <QDebug>

ConnectionWizardSqlitePage::ConnectionWizardSqlitePage(QWidget* parent)
    : QWizardPage(parent)
{
    setTitle("Sqlite Connection");
//    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/logos/sqlite.png"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/logos/sqlite_small.png"));

    database = QSqlDatabase::addDatabase("QSQLITE", "sqlite");

    QLabel* nameLabel = new QLabel(tr("Name:"));
    nameEdit = new QLineEdit;

    filePathNameEdit = new QLineEdit;
    QPushButton* openFileDialog = new QPushButton(tr("&Open Database"));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(filePathNameEdit);
    layout->addWidget(openFileDialog);
    setLayout(layout);

    connect(openFileDialog, SIGNAL(clicked(bool)), this, SLOT(showFileDialog()));
    connect(filePathNameEdit, SIGNAL(editingFinished()), this, SLOT(validatePage()));

    if (isVisible()) {
        wizard()->button(QWizard::NextButton)->setVisible(false);
        wizard()->button(QWizard::NextButton)->setEnabled(false);
        wizard()->button(QWizard::FinishButton)->setEnabled(false);
    }

    sqliteConnectionValid = new QLineEdit;
    sqliteConnectionValid->setText("NO");

    registerField("sqlite.connection.name", nameEdit);
    registerField("sqlite.connection.valid", sqliteConnectionValid);
    registerField("sqlite.path", filePathNameEdit);
}

void ConnectionWizardSqlitePage::showFileDialog() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    if (dialog.exec()) {
        filePathNameEdit->setText(dialog.selectedFiles()[0]);
        validatePage();
    }
}

bool ConnectionWizardSqlitePage::validatePage() {
    if (filePathNameEdit->text().isEmpty()) {
        return false;
    }

    database.setDatabaseName(filePathNameEdit->text());
    database.open();

    if (database.isOpen()) {
        wizard()->button(QWizard::NextButton)->setEnabled(true);
        wizard()->button(QWizard::FinishButton)->setEnabled(true);
        setField("sqlite.connection.valid", "YES");
        return true;
    }
    wizard()->button(QWizard::NextButton)->setEnabled(false);
    wizard()->button(QWizard::FinishButton)->setEnabled(false);
    setField("sqlite.connection.valid", "NO");
    return false;
}

bool ConnectionWizardSqlitePage::isComplete() const {
    return database.isOpen();
}

int ConnectionWizardSqlitePage::nextId() const {
//    return NewConnectionWizard::Page_Finish;
    return -1;
}
