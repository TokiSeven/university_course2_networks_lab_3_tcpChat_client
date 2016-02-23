#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chat = new TcpChat_Client;
    connect(chat, SIGNAL(connected()), this, SLOT(sl_updateGUI()));
    connect(chat, SIGNAL(disconnected()), this, SLOT(sl_updateGUI()));
    connect(chat, SIGNAL(message_come()), this, SLOT(sl_updateGUI()));
    connect(chat, SIGNAL(time_out()), this, SLOT(sl_updateGUI()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sl_updateGUI()
{
    if (chat->getServerStatus() == ST_ON)
    {
        this->ui->label_server_status->setText("Online");
    }
    else if (chat->getServerStatus() == ST_OFF)
    {
        this->ui->label_server_status->setText("Offline");
    }
    else if (chat->getServerStatus() == ST_UNDEF)
    {
        this->ui->label_server_status->setText("Undefined");
    }

    this->ui->list_clients->clear();
    this->ui->list_messages->clear();

    for (int i = 0; i < chat->getAllClients().size(); i++)
        this->ui->list_clients->addItem(chat->getAllClients()[i].toString());
    for (int i = 0; i < chat->getAllMessages().size(); i++)
        this->ui->list_messages->addItem(chat->getAllMessages()[i]);
}

void MainWindow::on_button_connect_clicked()
{
    chat->connect_to_server(this->ui->line_server_ip->text());
}

void MainWindow::on_button_send_clicked()
{
    if (!this->ui->text_message->text().isEmpty())
        chat->send_message(this->ui->text_message->text());
}
