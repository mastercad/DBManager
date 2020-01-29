#ifndef CONNECTIONWITHFILEDIALOG_H
#define CONNECTIONWITHFILEDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectionWithFileDialog;
}

class ConnectionWithFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionWithFileDialog(QWidget *parent = 0);
    ~ConnectionWithFileDialog();

private:
    Ui::ConnectionWithFileDialog *ui;
};

#endif // CONNECTIONWITHFILEDIALOG_H
