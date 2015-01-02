#ifndef IDSWINDOW_H
#define IDSWINDOW_H

class DSWindowManager;

class IDSWindow
{
public:
    IDSWindow(DSWindowManager* manager);
    virtual ~IDSWindow();

    virtual void showWindow() = 0;
    DSWindowManager* getManager();

private:
    DSWindowManager* manager;
};

#endif // IDSWINDOW_H
