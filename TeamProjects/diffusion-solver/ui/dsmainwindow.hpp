#ifndef DSMAINWINDOW_HPP
#define DSMAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class DSMainWindow;
}

class DSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DSMainWindow(QWidget *parent = 0);
    ~DSMainWindow();

private:
    Ui::DSMainWindow *ui;
};

#endif // DSMAINWINDOW_HPP
