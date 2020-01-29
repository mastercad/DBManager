#include "connectionwithfiledialog.h"
#include "ui_connectionwithfiledialog.h"

ConnectionWithFileDialog::ConnectionWithFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionWithFileDialog)
{
    ui->setupUi(this);
}

ConnectionWithFileDialog::~ConnectionWithFileDialog()
{
    delete ui;
}
