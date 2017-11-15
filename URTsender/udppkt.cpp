#include "udppkt.h"

UdpPkt::UdpPkt(QByteArray &block,QUdpSocket * sock,QString &ip,int portNum,quint64 Num,QObject *parent)
{
    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&UdpPkt::outTime);
    socket = sock;
    ipAddress.setAddress(ip);
    port = portNum;
    num = Num;
    rtt = 100;
//    data.resize(0);
    QDataStream out(&data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << num;

    data += block;
//    qDebug() << "*********************";
//    qDebug() << data;
    send();
}
UdpPkt::~UdpPkt() {
    if(timer->isActive()) {
        timer->stop();
    }
    delete timer;
}
void UdpPkt::send() {
    //qDebug() << "send pkt " << num;
    socket->writeDatagram(data,ipAddress,port);
    timer->setInterval(rtt);
    timer->start();
}
void UdpPkt::outTime() {
    if(rtt < 1000) {
        rtt *= 2;
    } else {
        rtt = 100;
    }
    timer->stop();
    send();
}
