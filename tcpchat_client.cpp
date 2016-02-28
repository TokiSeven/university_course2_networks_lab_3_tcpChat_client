#include "tcpchat_client.h"
#include <QTimer>

TcpChat_Client::TcpChat_Client()
{
    soc = new QTcpSocket(this);
    connect(soc, SIGNAL(readyRead()), this, SLOT(read_message()));
    connect(soc, SIGNAL(connected()), this, SLOT(sl_connected()));
    connect(soc, SIGNAL(disconnected()), this, SLOT(sl_disconnected()));
    this->server_status = ST_UNDEF;
    _blockSize = 0;
    this->auth = false;
}

TcpChat_Client::~TcpChat_Client()
{
    delete soc;
}

void TcpChat_Client::connect_to_server(QString ip)
{
    if (this->server_status == ST_OFF || this->server_status == ST_UNDEF)
    {
        this->server_ip.setAddress(ip);
        soc->connectToHost(this->server_ip, 4343);
        QTimer::singleShot(3000, this, SLOT(sl_timeout()));
    }
}

void TcpChat_Client::disconnect_from_server()
{
    if (this->server_status == ST_ON)
    {
        soc->disconnectFromHost();
        this->server_status = ST_OFF;
        emit disconnected();
    }
}

void TcpChat_Client::send_message(QString cmd, QString mess)
{
    if (this->server_status == ST_ON)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        //резервируем 2 байта для размера блока.
        out << (quint16)0;
        out << cmd;
        out << mess;

        //возваращаемся в начало
        out.device()->seek(0);

        //вписываем размер блока на зарезервированное место
        out << (quint16)(block.size() - sizeof(quint16));
        soc->write(block);
    }
}

void TcpChat_Client::read_message()
{
    //тут обрабатываются данные от сервера
    QDataStream in(soc);

    //если считываем новый блок первые 2 байта это его размер
    if (_blockSize == 0)
    {
        //если пришло меньше 2 байт ждем пока будет 2 байта
        if (soc->bytesAvailable() < (int)sizeof(quint16))
            return;

        //считываем размер (2 байта)
        in >> _blockSize;
    }

    //ждем пока блок прийдет полностью
    if (soc->bytesAvailable() < _blockSize)
        return;
    else
        _blockSize = 0;//можно принимать новый блок

    QString cmd;
    in >> cmd;

    if (cmd == QString::fromStdString("CLIENTS"))
    {
        QList<QString> addr;
        in >> addr;
        this->clients = addr;
        emit clients_come();
    }
    else if (cmd == QString::fromStdString("MESS"))
    {
        QString mess;
        in >> mess;
        this->messages.append(mess);
        emit message_come();
    }
    else if (cmd == QString::fromStdString("AUTH"))
    {
        QString mess;
        in >> mess;
        qDebug() << cmd + "::" + mess;
        if (mess == QString::fromStdString("YES"))
            this->auth = true;
        else
            this->auth = false;

        emit sig_auth();
    }
}

void TcpChat_Client::sl_connected()
{
    this->server_status = ST_ON;
    emit connected();
}

void TcpChat_Client::sl_disconnected()
{
    this->server_status = ST_OFF;
    this->clients.clear();
    this->messages.clear();
    emit disconnected();
}

void TcpChat_Client::sl_timeout()
{
    if (this->server_status == ST_UNDEF)
    {
        soc->disconnectFromHost();
        emit time_out();
    }
}
