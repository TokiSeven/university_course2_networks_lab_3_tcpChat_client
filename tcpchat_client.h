#ifndef TCPCHAT_CLIENT_H
#define TCPCHAT_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>

enum STATUS {ST_ON, ST_OFF, ST_UNDEF};

class TcpChat_Client : public QObject
{
    Q_OBJECT
public:
    TcpChat_Client();
    ~TcpChat_Client();

    void connect_to_server(QString);//try to connect to server
    void disconnect_from_server();//if you want to disconnect from server

    void send_message(QString);

    STATUS getServerStatus()const{return this->server_status;}
    QList<QString> getAllMessages()const{return this->messages;}
    QList<QHostAddress> getAllClients()const{return this->clients;}

signals:
    void connected();
    void disconnected();
    void time_out();
    void message_come();
    void clients_come();

private slots:
    void read_message();//when message come
    void sl_connected();
    void sl_disconnected();
    void sl_timeout();

private:
    QTcpSocket *soc;//client socket
    QHostAddress server_ip;//server ip
    STATUS server_status;//server status
    QList<QString> messages;//all messages
    QList<QHostAddress> clients;//all clients
    quint16 _blockSize;//current size of data block
};

#endif // TCPCHAT_CLIENT_H
