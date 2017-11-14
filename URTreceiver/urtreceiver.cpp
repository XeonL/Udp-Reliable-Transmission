#include "urtreceiver.h"

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
        datagram.resize(quint64(1024));
        listenScoket->readDatagram(datagram.data(),datagram.size());
        qDebug() << "*********************************";
        qDebug() << datagram;
        QDataStream in(datagram);
        quint64 sign;
        in >> sign;
        qDebug() << "get " << sign;
        if(sign == 1&&!isBegined) {
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
        } else if(sign == 0&&isEnded) {
            file->close();

        } else if(sign > 1){
//            QByteArray *fileData = new QByteArray();
//            fileData->resize(1016);
//            in >> *fileData;
//            if(sign == now + 1) {
//                qDebug() << "now is " << now;
//                qDebug() << "pkt " << sign << " is by written";
//                waitToWrite-=(*fileData).size();
//                file->write(*fileData);

//                now++;
//                while(buffer->value(now)) {

//                    QByteArray *pFileData = buffer->value(now);
//                    buffer->remove(now);
//                    waitToWrite-=(*pFileData).size();
//                    file->write(*pFileData);

//                    now++;
//                }
//            } else {
//                buffer->insert(sign,fileData);
//            }
//            qDebug() << waitToWrite << " bytes wait written. ";
//            if(waitToWrite==0) {
//                isEnded = true;
//            }
            QByteArray fileData;

            fileData = datagram.remove(0,8);


            qDebug() << fileData;



            fileData.resize(qMin(quint64(1016),waitToWrite));
            if(sign == now + 1) {
                qDebug() << "now is " << now;
                qDebug() << "pkt " << sign << " is by written";
                waitToWrite-=fileData.size();
                file->write(fileData);

                now++;
                while((buffer->value(now))!="") {

                    QByteArray pFileData = buffer->value(now);
                    buffer->remove(now);
                    waitToWrite-=pFileData.size();
                    file->write(pFileData);

                    now++;
                }
            } else {
                buffer->insert(sign,fileData);
            }
            qDebug() << waitToWrite << " bytes wait written. ";
            if(waitToWrite==0) {
                isEnded = true;
            }
        }








        //ACK
        if(sign!=0||(sign==0&&isEnded))
        {
            QByteArray block;
            block.resize(sizeof(quint64));
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_9);
            out << sign;
            socket->write(block);
        }
    }
}
