#include "udppkt.h"

UdpPkt::UdpPkt(QByteArray &block,QUdpSocket * sock,QString &ip,int portNum,quint64 num,QObject *parent)
{
    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&UdpPkt::outTime);
    socket = sock;
    ipAddress.setAddress(ip);
    port = portNum;
    rtt = 100;
//    data.resize(0);
    QDataStream out(&data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << num;

    data += block;
    qDebug() << "*********************";
    qDebug() << data;
    send();
}
UdpPkt::~UdpPkt() {
    if(timer->isActive()) {
        timer->stop();
    }
    delete timer;
}
void UdpPkt::send() {
    socket->writeDatagram(data,ipAddress,port);
    timer->setInterval(rtt);
    timer->start();
}
void UdpPkt::outTime() {
    rtt *= 2;
    timer->stop();
    send();
}
