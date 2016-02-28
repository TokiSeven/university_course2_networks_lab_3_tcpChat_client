#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chat = new TcpChat_Client;
    this->server_ip = "95.84.146.28";
    connect(chat, SIGNAL(connected()), this, SLOT(sl_connected()));
    connect(chat, SIGNAL(disconnected()), this, SLOT(sl_disconnected()));
    connect(chat, SIGNAL(message_come()), this, SLOT(sl_updateGUI()));
    connect(chat, SIGNAL(clients_come()), this, SLOT(sl_updateGUI()));
    connect(chat, SIGNAL(time_out()), this, SLOT(sl_timeout()));
    connect(chat, SIGNAL(sig_auth()), this, SLOT(sl_auth()));
    chat->connect_to_server(this->server_ip);
    ui->text_message->setEnabled(false);
    ui->button_send->setEnabled(false);
    ui->line_nickname->setEnabled(false);
    ui->button_auth->setEnabled(false);
    ui->button_reconnect->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sl_connected()
{
    if (chat->getServerStatus() == ST_ON)
    {
        this->ui->label_server_status->setText("Online");
        ui->text_message->setEnabled(false);
        ui->button_send->setEnabled(false);
        ui->line_nickname->setEnabled(true);
        ui->button_auth->setEnabled(true);
        ui->button_reconnect->setEnabled(false);
    }
}

void MainWindow::sl_disconnected()
{
    if (chat->getServerStatus() == ST_OFF)
    {
        this->ui->label_server_status->setText("Offline");
        ui->text_message->setEnabled(false);
        ui->button_send->setEnabled(false);
        ui->line_nickname->setEnabled(false);
        ui->button_auth->setEnabled(false);
        ui->button_reconnect->setEnabled(true);
    }
}

void MainWindow::sl_timeout()
{
    if (chat->getServerStatus() == ST_OFF || chat->getServerStatus() == ST_UNDEF)
    {
        this->ui->label_server_status->setText("Undefined");
        ui->text_message->setEnabled(false);
        ui->button_send->setEnabled(false);
        ui->line_nickname->setEnabled(false);
        ui->button_auth->setEnabled(false);
        ui->button_reconnect->setEnabled(true);
    }
}

void MainWindow::sl_auth()
{
    if (chat->isAuth())
    {
        ui->text_message->setEnabled(true);
        ui->button_send->setEnabled(true);
        ui->line_nickname->setEnabled(false);
        ui->button_auth->setEnabled(false);
    }
    else
    {
        ui->text_message->setEnabled(false);
        ui->button_send->setEnabled(false);
        ui->line_nickname->setEnabled(true);
        ui->button_auth->setEnabled(true);
    }
    ui->button_reconnect->setEnabled(false);
}

void MainWindow::sl_updateGUI()
{
    this->ui->list_clients->clear();
    this->ui->list_messages->clear();

    QList<QString> cl = chat->getAllClients();
    QList<QString> ms = chat->getAllMessages();

    for (int i = 0; i < cl.size(); i++)
        this->ui->list_clients->addItem(cl[i]);
    for (int i = 0; i < ms.size(); i++)
        this->ui->list_messages->addItem(ms[i]);
}

void MainWindow::on_button_send_clicked()
{
    if (!this->ui->text_message->text().isEmpty())
    {
        chat->send_message("MESS", this->ui->text_message->text());
        ui->text_message->setText("");
    }
}

void MainWindow::on_button_auth_clicked()
{
    if (!this->ui->label_info_nickname->text().isEmpty())
    {
        chat->send_message("AUTH", this->ui->line_nickname->text());
    }
}

void MainWindow::on_button_reconnect_clicked()
{
    ui->button_reconnect->setEnabled(false);
    chat->connect_to_server(this->server_ip);
}
