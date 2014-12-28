#include "dsmainwindow.hpp"
#include <QApplication>

#include "dsmodel.hpp"

#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::unique_ptr<DSModel> mainModel(new DSModel());
    DSMainWindow w(mainModel.get());
//    w.setModel(mainModel.get());
    w.show();

    return a.exec();
}
