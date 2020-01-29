#include "newconnectionwindow.h"
#include "ui_newconnectionwindow.h"

#include "connectionwithlogindialog.h"
#include "connectionwithfiledialog.h"

NewConnectionWindow::NewConnectionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConnectionWindow)
{
    ui->setupUi(this);

    connect(ui->btnMysqlConnection, SIGNAL(clicked(bool)), this, SLOT(showNewMySqlConnectionWindow()));
    connect(ui->btnSqliteConnection, SIGNAL(clicked(bool)), this, SLOT(showNewSqliteConnectionWindow()));
}

void NewConnectionWindow::showNewMySqlConnectionWindow() {
    delete dialog;
    dialog = new ConnectionWithLoginDialog();
    ui->horizontalLayout->addWidget(dialog);
}

void NewConnectionWindow::showNewSqliteConnectionWindow() {
    delete dialog;
    dialog = new ConnectionWithFileDialog();
    ui->horizontalLayout->addWidget(dialog);
}

NewConnectionWindow::~NewConnectionWindow()
{
    delete dialog;
    delete ui;
}
