#ifndef URTRECEIVER_H
#define URTRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QDataStream>
#include <QHash>

class UrtReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UrtReceiver(QString const &ip,QObject *parent = nullptr);

signals:

public slots:
private:
    void getACK(quint64);
    void processPendingDatagram();
    QUdpSocket *socket;
    QUdpSocket *listenScoket;
    QFile *file;
    QHash<quint64,QByteArray> *buffer;
    quint64 size;
    quint64 waitToWrite;
    int bufferSize = 50000;
    int bufferUse = 0;
    bool isBegined = false;
    bool isEnded = false;
    QString fileName;
    quint64 now = 1;
};

#endif // URTRECEIVER_H
