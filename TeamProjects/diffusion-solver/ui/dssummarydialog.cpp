#include "dssummarydialog.hpp"
#include "ui_dssummarydialog.h"

DSSummaryDialog::DSSummaryDialog(DSWindowManager* manager, QWidget *parent) :
    QDialog(parent),
    IDSWindow(manager),
    ui(new Ui::DSSummaryDialog)
{
    ui->setupUi(this);
}

DSSummaryDialog::~DSSummaryDialog()
{
    delete ui;
}

void DSSummaryDialog::showWindow()
{
    show();
}
