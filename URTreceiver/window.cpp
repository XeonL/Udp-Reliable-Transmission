#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

}

Window::~Window()
{
    delete ui;
}

void Window::on_startButton_clicked()
{
    receiver = new UrtReceiver(QString("222.20.104.94"));
}


