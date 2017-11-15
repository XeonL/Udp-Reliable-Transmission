#include "urtreceiver.h"
#define SIZE 1024*32
UrtReceiver::UrtReceiver(QString const &ip,QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket();
    listenScoket = new QUdpSocket();
    buffer = new QHash<quint64,QByteArray>;
    socket->connectToHost(ip,45454);
    listenScoket->bind(24356,QUdpSocket::ShareAddress);
    connect(listenScoket,&QUdpSocket::readyRead,this,&UrtReceiver::processPendingDatagram);

}
void UrtReceiver::processPendingDatagram() {
    while(listenScoket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(quint64(SIZE+8));
        listenScoket->readDatagram(datagram.data(),datagram.size());
        //qDebug() << datagram;
        QDataStream in(datagram);
        quint64 sign;
        in >> sign;

        if(sign == 1) {
            if(!isBegined)
            {
                isBegined = true;
                in >> size;
                waitToWrite = size;
                qDebug() << "total size:" << size;
                quint64 nameSize;
                in >> nameSize;
                QString name;

                in >> name;
                name.resize(nameSize);
                fileName = name;
                qDebug() << "fileName :" << fileName;
                qDebug() << "name size:" << nameSize;
                file = new QFile(fileName);
                file->open(QIODevice::WriteOnly);
                now = 1;
            }
            getACK(sign);
        } else if(sign == 0&&isEnded) {
            file->close();
            getACK(sign);

        } else if(sign > 1){
            QByteArray fileData;
            fileData = datagram.remove(0,8);
            qDebug() << fileData;
            //fileData.resize(qMin(quint64(SIZE),waitToWrite));
            if(sign == now + 1) {
                waitToWrite-=fileData.size();
                file->write(fileData);
                now++;             
                qDebug() << "pkt " << sign << " is by written";
                qDebug() << "now is " << now;
                qDebug() << waitToWrite << " bytes wait written. ";
                while((buffer->value(now+1))!="") {
                    QByteArray pFileData = buffer->value(now+1);
                    buffer->remove(now+1);
                    pFileData.resize(qMin(quint64(SIZE),waitToWrite));
                    waitToWrite-=pFileData.size();                  
                    file->write(pFileData);
                    now++;
                    bufferUse--;
                    qDebug() << "pkt " << now << " is by written";
                    qDebug() << "now is " << now;
                    qDebug() << waitToWrite << " bytes wait written. ";
                }
                getACK(sign);
            } else if(sign > now + 1) {
                if(bufferUse<bufferSize) {
                    buffer->insert(sign,fileData);
                    bufferUse++;
                    getACK(sign);
                }
            } else if(sign <= now) {
                qDebug() << sign << " is a again pkt";
                getACK(sign);
            }
            if(waitToWrite==0) {
                isEnded = true;
            }
        }
    }
}
void UrtReceiver::getACK(quint64 sign) {
    qDebug() << "ACK " << sign;
    QByteArray block;
    block.resize(sizeof(quint64));
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << sign;
    socket->write(block);
}
