#ifndef NEWCONNECTIONWINDOW_H
#define NEWCONNECTIONWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
    class NewConnectionWindow;
}

class NewConnectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnectionWindow(QWidget *parent = 0);
    ~NewConnectionWindow();

public slots:
    void showNewMySqlConnectionWindow();
    void showNewSqliteConnectionWindow();

private:
    Ui::NewConnectionWindow *ui;
    QDialog* dialog = NULL;
};

#endif // NEWCONNECTIONWINDOW_H
