#include <QThread>
#include "dssolvingprogressdialog.hpp"
#include "ui_dssolvingprogressdialog.h"

DSSolvingProgressDialog::DSSolvingProgressDialog(DSModel* model,
                                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSSolvingProgressDialog),
    model(model)
{
    ui->setupUi(this);

    connect(model, SIGNAL(iterationDone(DSSolverIterationInfo&)),
            this, SLOT(updateIterationInfo(DSSolverIterationInfo&)));

    connect(model, SIGNAL(resultAcquired()),
            this, SLOT(close()));

    connect(ui->buttonStopSolver, SIGNAL(clicked()),
            model, SLOT(stopSolver()));
}

DSSolvingProgressDialog::~DSSolvingProgressDialog()
{
    delete ui;
}


void DSSolvingProgressDialog::updateIterationInfo(DSSolverIterationInfo& info)
{
    int iters = info.GetCurrentIterationNumber();
    int total = info.GetPlannedIterationsCount();

    QString labelText;
    labelText.sprintf("Пройдено %d итераций из %d", iters, total);
    ui->labelIterationNumber->setText(labelText);

    ui->progressIterationsCounter->setMaximum(total);
    ui->progressIterationsCounter->setValue(iters);

    double activatorAccuracy = info.GetActivatorCurrentAccuracy();
    double inhibitorAccuracy = info.GetInhibitorCurrentAccuracy();

    labelText.sprintf("%f", activatorAccuracy);
    ui->labelActivatorAccuracyValue->setText(labelText);

    labelText.sprintf("%f", inhibitorAccuracy);
    ui->labelInhibitorAccuracyValue->setText(labelText);
}
