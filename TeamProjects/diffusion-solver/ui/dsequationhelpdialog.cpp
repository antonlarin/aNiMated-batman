#include "dsequationhelpdialog.hpp"
#include "ui_dsequationhelpdialog.h"

DSEquationHelpDialog::DSEquationHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSEquationHelpDialog)
{
    ui->setupUi(this);
}

DSEquationHelpDialog::~DSEquationHelpDialog()
{
    delete ui;
}
