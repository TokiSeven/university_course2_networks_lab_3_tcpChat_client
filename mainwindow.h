#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpchat_client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sl_updateGUI();

    void on_button_connect_clicked();

    void on_button_send_clicked();

private:
    Ui::MainWindow *ui;
    TcpChat_Client *chat;
};

#endif // MAINWINDOW_H
