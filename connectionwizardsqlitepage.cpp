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
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/logos/sqlite.png"));

    database = QSqlDatabase::addDatabase("QSQLITE");

    filePathNameEdit = new QLineEdit;
    QPushButton* openFileDialog = new QPushButton(tr("&Open Database"));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(filePathNameEdit);
    layout->addWidget(openFileDialog);
    setLayout(layout);

    connect(openFileDialog, SIGNAL(clicked(bool)), this, SLOT(showFileDialog()));
    connect(filePathNameEdit, SIGNAL(editingFinished()), this, SLOT(testConnection()));

    if (isVisible()) {
        wizard()->button(QWizard::NextButton)->setVisible(false);
    }
}

void ConnectionWizardSqlitePage::showFileDialog() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    if (dialog.exec()) {
        filePathNameEdit->setText(dialog.selectedFiles()[0]);
        testConnection();
    }
}

bool ConnectionWizardSqlitePage::testConnection() {
    if (filePathNameEdit->text().isEmpty()) {
        return false;
    }

    database.setDatabaseName(filePathNameEdit->text());
    database.open();

    if (database.isOpen()) {
        wizard()->button(QWizard::NextButton)->setEnabled(true);
        wizard()->button(QWizard::FinishButton)->setEnabled(true);
        return true;
    }
    qDebug() << "CLOSED!";
    wizard()->button(QWizard::NextButton)->setEnabled(false);
    wizard()->button(QWizard::FinishButton)->setEnabled(false);
    return false;
}

bool ConnectionWizardSqlitePage::isComplete() const {
    return database.isOpen();
}

int ConnectionWizardSqlitePage::nextId() const {
//    return NewConnectionWizard::Page_Finish;
    return -1;
}
