#include "dssummarydialog.hpp"
#include "ui_dssummarydialog.h"

DSSummaryDialog::DSSummaryDialog(DSWindowManager* manager, QWidget *parent) :
    QDialog(parent),
    IDSWindow(manager),
    ui(new Ui::DSSummaryDialog)
{
    ui->setupUi(this);

    connect(ui->closeButton, SIGNAL(clicked()),
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
        QString("%1").arg(model->GetAchievedActivatorAccuracy())
    );
    ui->finalTimeValueLabel->setText(
        QString("%1").arg(model->GetLastLayerTime())
    );
    ui->inhibitorAccuracyValueLabel->setText(
        QString("%1").arg(model->GetAchievedInhibitorAccuracy())
    );
    ui->iterationsPerformedValueLabel->setText(
        QString("%1").arg(model->GetPerformedIterationsCount())
    );

    show();
}
