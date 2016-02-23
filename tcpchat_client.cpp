#include "tcpchat_client.h"
#include <QTimer>

TcpChat_Client::TcpChat_Client() : QObject(this)
{
    soc = new QTcpSocket(this);
    connect(soc, SIGNAL(readyRead()), this, SLOT(read_message()));
    connect(sok, SIGNAL(connected()), this, SLOT(sl_connected()));
    connect(sok, SIGNAL(disconnected()), this, SLOT(sl_disconnected()));
    this->server_status = ST_UNDEF;
    _blockSize = 0;
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

void TcpChat_Client::send_message(QString mess)
{
    if (this->server_status == ST_ON)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        //����������� 2 ����� ��� ������� �����.
        out << (quint16)0 << mess;

        //������������� � ������
        out.device()->seek(0);

        //��������� ������ ����� �� ����������������� �����
        out << (quint16)(block.size() - sizeof(quint16));
        soc->write(block);
    }
}

void TcpChat_Client::read_message()
{
    //��� �������������� ������ �� �������
    QDataStream in(soc);

    //���� ��������� ����� ���� ������ 2 ����� ��� ��� ������
    if (_blockSize == 0)
    {
        //���� ������ ������ 2 ���� ���� ���� ����� 2 �����
        if (soc->bytesAvailable() < (int)sizeof(quint16))
            return;

        //��������� ������ (2 �����)
        in >> _blockSize;
    }

    //���� ���� ���� ������� ���������
    if (_sok->bytesAvailable() < _blockSize)
        return;
    else
        _blockSize = 0;//����� ��������� ����� ����

    QString message;
    in >> message;

    this->messages.append(message);
}

void TcpChat_Client::sl_connected()
{
    this->server_status = ST_ON;
    emit connected();
}

void TcpChat_Client::sl_disconnected()
{
    this->server_status = ST_OFF;
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
