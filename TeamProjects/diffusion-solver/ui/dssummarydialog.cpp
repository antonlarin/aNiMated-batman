#include "dssummarydialog.hpp"
#include "ui_dssummarydialog.h"

DSSummaryDialog::DSSummaryDialog(DSWindowManager* manager, QWidget *parent) :
    QDialog(parent),
    IDSWindow(manager),
    ui(new Ui::DSSummaryDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(close()));
}

DSSummaryDialog::~DSSummaryDialog()
{
    delete ui;
}

void DSSummaryDialog::showWindow()
{
    DSModel* model = getManager()->getModel();
    ui->activatorAccuracyValueLabel->setText(
                tr("%1").arg(model->GetAchievedActivatorAccuracy()));
    ui->inhibitorAccuracyValueLabel->setText(
                tr("%1").arg(model->GetAchievedInhibitorAccuracy()));

    show();
}
