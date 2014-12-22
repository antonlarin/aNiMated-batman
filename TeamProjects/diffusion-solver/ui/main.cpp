#include "dsmainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DSMainWindow w;
    w.show();

    return a.exec();
}
