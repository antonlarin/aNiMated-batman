#include "dssolvingprogressdialog.hpp"
#include "ui_dssolvingprogressdialog.h"

#include <cmath>

DSSolvingProgressDialog::DSSolvingProgressDialog(DSModel* model,
                                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSSolvingProgressDialog),
    model(model)
{
    ui->setupUi(this);

    ui->performedIterationsProgressBar->setMinimum(0);
    ui->performedIterationsProgressBar->setMaximum(model->GetIterationsLimit());

    connect(model, SIGNAL(iterationInfoChanged()),
            this, SLOT(displayIterationInfo()));
    connect(model, SIGNAL(resultAcquired()),
            this, SLOT(close()));
}

DSSolvingProgressDialog::~DSSolvingProgressDialog()
{
    delete ui;
}



/*
 * Slots implementations
 */
void DSSolvingProgressDialog::displayIterationInfo()
{
    int totalIterations = model->GetIterationsLimit();
    int performedIterations = model->GetPerformedIterationsCount();

    ui->totalIterationsValueLabel->setText(tr("%1").arg(totalIterations));
    ui->performedIterationsValueLabel->
            setText(tr("%1").arg(performedIterations));
    ui->activatorAccuracyValueLabel->
            setText(tr("%1").arg(model->GetAchievedActivatorAccuracy()));
    ui->inhibitorAccuracyValueLabel->
            setText(tr("%1").arg(model->GetAchievedInhibitorAccuracy()));
    ui->performedIterationsProgressBar->setValue(performedIterations);
}
