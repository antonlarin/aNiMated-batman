#include "dsmainwindow.hpp"
#include <QApplication>

#include "dsmodel.hpp"

#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DSMainWindow w;
    std::unique_ptr<DSModel> mainModel(new DSModel());
    w.setModel(mainModel.get());
    w.show();

    return a.exec();
}
