#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpchat_client.h"
#include <QDebug>

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
    void sl_connected();
    void sl_disconnected();
    void sl_auth();
    void sl_updateGUI();
    void sl_timeout();

    void on_button_send_clicked();

    void on_button_auth_clicked();

    void on_button_reconnect_clicked();

private:
    Ui::MainWindow *ui;
    TcpChat_Client *chat;
    QString server_ip;
};

#endif // MAINWINDOW_H
