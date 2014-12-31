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
    labelText.sprintf("Сделано %d итераций из %d", iters, total);
    ui->labelIterationNumber->setText(labelText);
}
