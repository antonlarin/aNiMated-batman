#include <QApplication>

#include <memory>
#include "dsmodel.hpp"
#include "dswindowmanager.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::unique_ptr<DSModel> mainModel(new DSModel());
    std::unique_ptr<DSWindowManager> windowManager(
                new DSWindowManager(mainModel.get()));
    windowManager->showMainWindow();

    return a.exec();
}
