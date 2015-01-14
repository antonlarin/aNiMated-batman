#include <QThread>
#include "dssolvingprogressdialog.hpp"
#include "ui_dssolvingprogressdialog.h"

DSSolvingProgressDialog::DSSolvingProgressDialog(DSWindowManager* manager,
                                                 QWidget *parent) :
    QDialog(parent),
    IDSWindow(manager),
    ui(new Ui::DSSolvingProgressDialog)
{
    ui->setupUi(this);

    DSModel* model = getManager()->getModel();
    connect(model, SIGNAL(resultChanged(const SchemeSolverResult&)),
            this, SLOT(updateResult(const SchemeSolverResult&)));

    connect(model, SIGNAL(resultAcquired()),
            this, SLOT(close()));

    connect(ui->buttonStopSolver, SIGNAL(clicked()),
            model, SLOT(stopSolver()));
}

DSSolvingProgressDialog::~DSSolvingProgressDialog()
{
    delete ui;
}

void DSSolvingProgressDialog::showWindow()
{
    show();
}


void DSSolvingProgressDialog::updateResult(const SchemeSolverResult& res)
{
    auto stat = res.GetStatistic();
    auto task = res.GetTask();

    int iters = stat.GetIterationsCount();
    int total = task.GetMaximumLayers() - 1;

    QString labelText;
    labelText.sprintf("Пройдено %d итераций из %d", iters, total);
    ui->labelIterationNumber->setText(labelText);

    ui->progressIterationsCounter->setMaximum(total);
    ui->progressIterationsCounter->setValue(iters);

    double activatorAccuracy = stat.GetStopAccuracyU1();
    double inhibitorAccuracy = stat.GetStopAccuracyU2();

    labelText.sprintf("%f", activatorAccuracy);
    ui->labelActivatorAccuracyValue->setText(labelText);

    labelText.sprintf("%f", inhibitorAccuracy);
    ui->labelInhibitorAccuracyValue->setText(labelText);
}
