#include "dsmainwindow.hpp"
#include "ui_dsmainwindow.h"

#include <vector>

DSMainWindow::DSMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DSMainWindow),
    model(nullptr)
{
    ui->setupUi(this);

    connect(ui->lambda1Edit, SIGNAL(textEdited(QString)),
            this, SLOT(lambda1CoeffChanged(QString)));
    connect(ui->lambda2Edit, SIGNAL(textEdited(QString)),
            this, SLOT(lambda2CoeffChanged(QString)));
    connect(ui->kEdit, SIGNAL(textEdited(QString)),
            this, SLOT(kCoeffChanged(QString)));
    connect(ui->cEdit, SIGNAL(textEdited(QString)),
            this, SLOT(cCoeffChanged(QString)));
    connect(ui->rhoEdit, SIGNAL(textEdited(QString)),
            this, SLOT(rhoCoeffChanged(QString)));
    connect(ui->nuEdit, SIGNAL(textEdited(QString)),
            this, SLOT(nuCoeffChanged(QString)));
    connect(ui->gammaEdit, SIGNAL(textEdited(QString)),
            this, SLOT(gammaCoeffChanged(QString)));
    connect(ui->gridDimEdit, SIGNAL(textEdited(QString)),
            this, SLOT(gridDimensionChanged(QString)));
    connect(ui->timeStepEdit, SIGNAL(textEdited(QString)),
            this, SLOT(timeStepChanged(QString)));
    connect(ui->accuracyEdit, SIGNAL(textEdited(QString)),
            this, SLOT(accuracyChanged(QString)));
    connect(ui->iterationsEdit, SIGNAL(textEdited(QString)),
            this, SLOT(iterationsLimitChanged(QString)));

    connect(ui->finiteRunButton, SIGNAL(clicked()),
            this, SLOT(finiteRunStart()));
}

DSMainWindow::~DSMainWindow()
{
    delete ui;
}

void DSMainWindow::setModel(DSModel *newModel)
{
    model = newModel;

    std::vector<double> u1 = { 1.0 };
    std::vector<double> u2 = { 0.5 };
    model->SetInitialConditions(u1, u2);
}

/*
 * Slots implementations
 */
void DSMainWindow::lambda1CoeffChanged(const QString& newLambda1)
{
    bool ok;
    double value = newLambda1.toDouble(&ok);
    if (ok)
        model->SetLambda1(value);
}

void DSMainWindow::lambda2CoeffChanged(const QString& newLambda2)
{
    bool ok;
    double value = newLambda2.toDouble(&ok);
    if (ok)
        model->SetLambda2(value);
}

void DSMainWindow::kCoeffChanged(const QString& newK)
{
    bool ok;
    double value = newK.toDouble(&ok);
    if (ok)
        model->SetK(value);
}

void DSMainWindow::cCoeffChanged(const QString& newC)
{
    bool ok;
    double value = newC.toDouble(&ok);
    if (ok)
        model->SetC(value);
}

void DSMainWindow::rhoCoeffChanged(const QString& newRho)
{
    bool ok;
    double value = newRho.toDouble(&ok);
    if (ok)
        model->SetRho(value);
}

void DSMainWindow::nuCoeffChanged(const QString& newNu)
{
    bool ok;
    double value = newNu.toDouble(&ok);
    if (ok)
        model->SetNu(value);
}

void DSMainWindow::gammaCoeffChanged(const QString& newGamma)
{
    bool ok;
    double value = newGamma.toDouble(&ok);
    if (ok)
        model->SetGamma(value);
}

void DSMainWindow::gridDimensionChanged(const QString& newGridDimension)
{
    bool ok;
    int value = newGridDimension.toInt(&ok);
    if (ok)
        model->SetGridDimension(value);
}

void DSMainWindow::timeStepChanged(const QString& newTimeStep)
{
    bool ok;
    double value = newTimeStep.toDouble(&ok);
    if (ok)
        model->SetTimeStep(value);
}

void DSMainWindow::accuracyChanged(const QString& newAccuracy)
{
    bool ok;
    double value = newAccuracy.toDouble(&ok);
    if (ok)
        model->SetAccuracy(value);
}

void DSMainWindow::iterationsLimitChanged(const QString& newIterationsLimit)
{
    bool ok;
    int value = newIterationsLimit.toInt(&ok);
    if (ok)
        model->SetIterationsLimit(value);
}

void DSMainWindow::finiteRunStart()
{
    model->StartFiniteRun();
}
