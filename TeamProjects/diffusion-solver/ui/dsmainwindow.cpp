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

    connect(ui->currentLayerEdit, SIGNAL(textEdited(QString)),
            this, SLOT(goToLayer(QString)));
    connect(ui->prevLayerButton, SIGNAL(clicked()),
            this, SLOT(goToPrevLayer()));
    connect(ui->nextLayerButton, SIGNAL(clicked()),
            this, SLOT(goToNextLayer()));
    connect(ui->firstLayerButton, SIGNAL(clicked()),
            this, SLOT(goToFirstLayer()));
    connect(ui->lastLayerButton, SIGNAL(clicked()),
            this, SLOT(goToLastLayer()));
    connect(ui->layerStepEdit, SIGNAL(textEdited(QString)),
            this, SLOT(changeLayerStep(QString)));

    connect(ui->finiteRunButton, SIGNAL(clicked()),
            this, SLOT(finiteRunStart()));

    resetPlots();
}

DSMainWindow::~DSMainWindow()
{
    delete ui;
}

void DSMainWindow::setModel(DSModel *newModel)
{
    model = newModel;
    model->RegisterView(this);

    std::vector<double> u1 = { 1.0 };
    std::vector<double> u2 = { 0.5 };
    model->SetInitialConditions(u1, u2);
}

void DSMainWindow::update()
{
    ui->totalLayerNumLabel->setText(tr("из %1, шаг").
                                    arg(model->GetLayerCount()));
    ui->currentLayerEdit->setText(tr("%1").
                                  arg(model->GetCurrentLayerIndex()));
    ui->layerStepEdit->setText(tr("%1").
                               arg(model->GetLayerStep()));
    displayActivatorLayer(model->GetCurrentActivatorLayer());
    displayInhibitorLayer(model->GetCurrentInhibitorLayer());
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

void DSMainWindow::goToPrevLayer()
{
    model->SetCurrentLayerIndex(model->GetCurrentLayerIndex() -
                                model->GetLayerStep());
}

void DSMainWindow::goToNextLayer()
{
    model->SetCurrentLayerIndex(model->GetCurrentLayerIndex() +
                                model->GetLayerStep());
}

void DSMainWindow::goToFirstLayer()
{
    model->SetCurrentLayerIndex(0);
}

void DSMainWindow::goToLastLayer()
{
    model->SetCurrentLayerIndex(model->GetLayerCount() - 1);
}

void DSMainWindow::goToLayer(const QString& newLayer)
{
    bool ok;
    int value = newLayer.toInt(&ok);
    if (ok)
        model->SetCurrentLayerIndex(value);
}

void DSMainWindow::changeLayerStep(const QString& newLayerStep)
{
    bool ok;
    int value = newLayerStep.toInt(&ok);
    if (ok && value > 0)
        model->SetLayerStep(value);
}




/*
 * Private worker methods implementation
 */

void DSMainWindow::resetPlots()
{
    ui->activatorPlot->addGraph();
    ui->activatorPlot->xAxis->setLabel("x");
    ui->activatorPlot->yAxis->setLabel("Концентрация активатора");
    ui->activatorPlot->xAxis->setRange(0, 1);

    ui->inhibitorPlot->addGraph();
    ui->inhibitorPlot->xAxis->setLabel("x");
    ui->inhibitorPlot->yAxis->setLabel("Концентрация ингибитора");
    ui->inhibitorPlot->xAxis->setRange(0, 1);
}

void DSMainWindow::displayActivatorLayer(const SchemeLayer& layer)
{
    QVector<double> xs, ys;
    int layerLength = layer.GetLength();
    int arrayStep = layerLength / maxPlotPointsNumber() + 1;
    double xStep = 1.0 / model->GetGridDimension();
    for (int i = 0; i < layerLength - 1; i += arrayStep)
    {
        xs.push_back(xStep * i);
        ys.push_back(layer.Get(i));
    }
    xs.push_back(1.0);
    ys.push_back(layer.Get(layerLength - 1));

    ui->activatorPlot->graph(0)->setData(xs, ys);
    ui->activatorPlot->replot();
}

void DSMainWindow::displayInhibitorLayer(const SchemeLayer& layer)
{
    QVector<double> xs, ys;
    int layerLength = layer.GetLength();
    int arrayStep = layerLength / maxPlotPointsNumber() + 1;
    double xStep = 1.0 / model->GetGridDimension();
    for (int i = 0; i < layerLength - 1; i += arrayStep)
    {
        xs.push_back(xStep * i);
        ys.push_back(layer.Get(i));
    }
    xs.push_back(1.0);
    ys.push_back(layer.Get(layerLength - 1));

    ui->inhibitorPlot->graph(0)->setData(xs, ys);
    ui->inhibitorPlot->replot();
}