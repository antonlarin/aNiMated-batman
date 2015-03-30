#include "dsequationhelpdialog.hpp"
#include "ui_dsequationhelpdialog.h"

DSEquationHelpDialog::DSEquationHelpDialog(DSWindowManager* manager,
                                           QWidget* parent) :
    IDSWindow(manager),
    QDialog(parent),
    ui(new Ui::DSEquationHelpDialog)
{
    ui->setupUi(this);
}

DSEquationHelpDialog::~DSEquationHelpDialog()
{
    delete ui;
}


void DSEquationHelpDialog::showWindow()
{
    show();
}
