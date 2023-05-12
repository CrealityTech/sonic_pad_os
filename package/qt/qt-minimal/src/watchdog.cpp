#include <QTimerEvent>
#include "watchdog.h"
#include <QAbstractSocket>

#define HEARTBEAT_INTERVAL   3000

watchdog::watchdog(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket();
    tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    timer.start(HEARTBEAT_INTERVAL, this);
    connect(tcpSocket,&QTcpSocket::connected,[=](){
        qDebug() << "connected : ";
        tcpSocket->write("4");
        tcpSocket->close();
    });

    connect(tcpSocket,&QTcpSocket::disconnected,[=](){
        qDebug() << "disconnected : ";
        timer.start(HEARTBEAT_INTERVAL, this);
    });
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [=](QAbstractSocket::SocketError state){
       qDebug() << "error : ";
       timer.start(HEARTBEAT_INTERVAL, this);
    });

}

void watchdog::timerEvent(QTimerEvent *event)
{   
    qDebug() << "timerEvent : ";
    timer.stop();
    tcpSocket->connectToHost("127.0.0.1",9090);
    //tcpSocket->write("4");
}

