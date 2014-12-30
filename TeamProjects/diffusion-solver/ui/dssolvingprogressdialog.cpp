#include "dssolvingprogressdialog.hpp"
#include "ui_dssolvingprogressdialog.h"

DSSolvingProgressDialog::DSSolvingProgressDialog(DSModel* model,
                                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSSolvingProgressDialog),
    model(model)
{
    ui->setupUi(this);
}

DSSolvingProgressDialog::~DSSolvingProgressDialog()
{
    delete ui;
}
