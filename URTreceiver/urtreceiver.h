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
    void processPendingDatagram();
    QUdpSocket *socket;
    QUdpSocket *listenScoket;
    QFile *file;
    QHash<quint64,QByteArray> *buffer;
    quint64 size;
    quint64 waitToWrite;
    bool isBegined = false;
    bool isEnded = false;
    QString fileName;
    quint64 now = 1;
};

#endif // URTRECEIVER_H
