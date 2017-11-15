#ifndef UDPPKT_H
#define UDPPKT_H

#include <QUdpSocket>
#include <QTimer>
#include <QHostAddress>
#include <QObject>
#include <QByteArray>
#include <QDataStream>
class UdpPkt : public QObject
{
    Q_OBJECT
public:
    UdpPkt(QByteArray &block,QUdpSocket * sock,QString &ip,int portNum,quint64 num,
           QObject *parent = nullptr);
    ~UdpPkt();
signals:

private:
    void send();
    void outTime();
    QByteArray data;
    QUdpSocket * socket;
    QTimer * timer;

    int rtt;
    QHostAddress ipAddress;
    int port;
    quint64 num;
};

#endif // UDPPKT_H
