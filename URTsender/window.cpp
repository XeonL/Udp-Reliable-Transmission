#include "window.h"
#include "ui_window.h"
#include <QFileDialog>
#include <QTextStream>
Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    socket = new QUdpSocket();

}

Window::~Window()
{
    delete ui;
}

void Window::on_sendButton_clicked()
{
    if(fileName.isEmpty()) {
        return;
    }
    if(ui->ipLine->text() == "") return;
    sendFile();
}

void Window::on_openButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,tr("open"));
    if(fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
//    file.open(QFile::ReadOnly);
    QStringList list = fileName.split("/");

    QString name = list[list.count()-1];
    ui->openStatus->setText(QString("%1 size:%2bytes").arg(name).arg(file.size()));
}
void Window::sendFile() {
    sender = new UrtSender(ui->ipLine->text(),24356,fileName);
    sender->start();

}
