#include "idswindow.hpp"

IDSWindow::IDSWindow(DSWindowManager* manager) :
    manager(manager)
{}

IDSWindow::~IDSWindow() {}

DSWindowManager* IDSWindow::getManager()
{
    return manager;
}

