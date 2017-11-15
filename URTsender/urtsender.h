#ifndef URTSENDER_H
#define URTSENDER_H

#include <QObject>
#include <QFile>
#include <QUdpSocket>
#include "udppkt.h"
#include <QHash>
#include <QHostInfo>

class UrtSender : public QObject
{
    Q_OBJECT
public:
    explicit UrtSender(QString const &ip,int portNum,QString &filename,QObject *parent = nullptr);
    ~UrtSender();
signals:

public slots:
    void start();
    void getACK(quint64);
    void processPendingDatagram();

private:
    void messageControl(quint64);
    void tryToSend();
    void tryToBegin();
    void tryToEnd();
    QUdpSocket *listenSocket;
    QUdpSocket *socket;
    QFile *file;
    bool isOpened = false;
    QString ipAddress;
    int port;
    qint64	totalBytes;		//数据总大小
    qint64	bytesWritten;		//已经发送数据大小
    qint64	bytesToWrite;			//剩余数据大小
    qint64	loadSize;			//每次发送数据的大小
    QString	fileName;		//保存文件路径
    QString name;
    QHash<quint64,UdpPkt *> * pktPool;
    int now = 0;
    int max = 20;
    quint64 nowNum = 2;
    bool begin = false;
    bool end = false;
    bool sendEnd = false;
    QTimer * timer;
    QTimer * endtimer;
};

#endif // URTSENDER_H
