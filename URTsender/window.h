#ifndef WINDOW_H
#define WINDOW_H

#include <QUdpSocket>
#include <QWidget>
#include "urtsender.h"

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
    void on_sendButton_clicked();

    void on_openButton_clicked();

private:
    void sendFile();
    Ui::Window *ui;
    QUdpSocket *socket;
    QString fileName;
    UrtSender *sender;
};

#endif // WINDOW_H
