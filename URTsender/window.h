#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

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
    void on_snedButton_clicked();

    void on_openButton_clicked();

private:
    Ui::Window *ui;
};

#endif // WINDOW_H
