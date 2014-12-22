#include "dsmainwindow.hpp"
#include "ui_dsmainwindow.h"

DSMainWindow::DSMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DSMainWindow)
{
    ui->setupUi(this);
}

DSMainWindow::~DSMainWindow()
{
    delete ui;
}
