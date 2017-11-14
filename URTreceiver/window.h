#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QUdpSocket>
#include "urtreceiver.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

private slots:
    void on_startButton_clicked();

private:
    Ui::Window *ui;
    QUdpSocket *socket;
    UrtReceiver *receiver;
};

#endif // WINDOW_H
