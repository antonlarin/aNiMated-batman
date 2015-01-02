#include "dsmainwindow.hpp"
#include "ui_dsmainwindow.h"

#include <vector>

DSMainWindow::DSMainWindow(DSModel* newModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DSMainWindow),
    initConditionsDialog(nullptr),
    solvingProgressDialog(nullptr),
    layerPairAnalysisWindow(nullptr),
    model(newModel)
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
    connect(ui->activatorAccuracyEdit, SIGNAL(textEdited(QString)),
            this, SLOT(activatorAccuracyChanged(QString)));
    connect(ui->inhibitorAccuracyEdit, SIGNAL(textEdited(QString)),
            this, SLOT(inhibitorAccuracyChanged(QString)));
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
            this, SLOT(startFiniteRun()));
    connect(ui->stabilityRunButton, SIGNAL(clicked()),
            this, SLOT(startStabilityRun()));

    connect(ui->quitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui->initConditionsAction, SIGNAL(triggered()),
            this, SLOT(showInitConditionsDialog()));
    connect(ui->layerPairAnalysisAction, SIGNAL(triggered()),
            this, SLOT(showLayerPairAnalysisWindow()));

    connect(model, SIGNAL(layerIndexChanged()),
            this, SLOT(updateDisplayedLayer()));
    connect(model, SIGNAL(resultAcquired()),
            this, SLOT(displayRunResults()));

    connect(ui->explicitSolverRadioButton, SIGNAL(clicked()),
            model, SLOT(selectExplicitSolver()));
    connect(ui->implicitSolverRadioButton, SIGNAL(clicked()),
            model, SLOT(selectImplicitSolver()));

    initPlots();
}

DSMainWindow::~DSMainWindow()
{
    delete ui;
}

/*
 * Slots implementations
 */
void DSMainWindow::lambda1CoeffChanged(const QString& newLambda1)
{
    bool isValidDouble;
    double value = newLambda1.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetLambda1(value);
}

void DSMainWindow::lambda2CoeffChanged(const QString& newLambda2)
{
    bool isValidDouble;
    double value = newLambda2.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetLambda2(value);
}

void DSMainWindow::kCoeffChanged(const QString& newK)
{
    bool isValidDouble;
    double value = newK.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetK(value);
}

void DSMainWindow::cCoeffChanged(const QString& newC)
{
    bool isValidDouble;
    double value = newC.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetC(value);
}

void DSMainWindow::rhoCoeffChanged(const QString& newRho)
{
    bool isValidDouble;
    double value = newRho.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetRho(value);
}

void DSMainWindow::nuCoeffChanged(const QString& newNu)
{
    bool isValidDouble;
    double value = newNu.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetNu(value);
}

void DSMainWindow::gammaCoeffChanged(const QString& newGamma)
{
    bool isValidDouble;
    double value = newGamma.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetGamma(value);
}

void DSMainWindow::gridDimensionChanged(const QString& newGridDimension)
{
    bool isValidInteger;
    int value = newGridDimension.toInt(&isValidInteger);
    if (isValidInteger)
        model->AccessParameters()->SetGridDimension(value);
}

void DSMainWindow::timeStepChanged(const QString& newTimeStep)
{
    bool isValidDouble;
    double value = newTimeStep.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetTimeStep(value);
}

void DSMainWindow::activatorAccuracyChanged(const QString& newAccuracy)
{
    bool isValidDouble;
    double value = newAccuracy.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetActivatorAccuracy(value);
}

void DSMainWindow::inhibitorAccuracyChanged(const QString& newAccuracy)
{
    bool isValidDouble;
    double value = newAccuracy.toDouble(&isValidDouble);
    if (isValidDouble)
        model->AccessParameters()->SetActivatorAccuracy(value);
}

void DSMainWindow::iterationsLimitChanged(const QString& newIterationsLimit)
{
    bool isValidInteger;
    int value = newIterationsLimit.toInt(&isValidInteger);
    if (isValidInteger)
        model->AccessParameters()->SetIterationsLimit(value);
}

void DSMainWindow::startFiniteRun()
{
    try
    {
        model->StartRun(SchemeSolverMode::AllLayers);
        showSolvingProgressDialog();
    }
    catch (std::runtime_error e)
    {
        QMessageBox::critical(this, QString("Неверные параметры"),
                              QString(e.what()));
    }
}

void DSMainWindow::startStabilityRun()
{
    try
    {
        model->StartRun(SchemeSolverMode::StableLayer);
        showSolvingProgressDialog();
    }
    catch (std::runtime_error e)
    {
        QMessageBox::critical(this, QString("Неверные параметры"),
                              QString(e.what()));
    }
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
    bool isValidInteger;
    int value = newLayer.toInt(&isValidInteger);
    if (isValidInteger)
        model->SetCurrentLayerIndex(value);
}

void DSMainWindow::changeLayerStep(const QString& newLayerStep)
{
    bool isValidInteger;
    int value = newLayerStep.toInt(&isValidInteger);
    if (isValidInteger)
        model->SetLayerStep(value);
}

void DSMainWindow::showInitConditionsDialog()
{
    initConditionsDialog = new DSInitConditionsDialog(model, this);
    initConditionsDialog->setAttribute(Qt::WA_DeleteOnClose);
    initConditionsDialog->show();
}

void DSMainWindow::showLayerPairAnalysisWindow()
{
    layerPairAnalysisWindow = new DSLayerPairAnalysisWindow(model);
    layerPairAnalysisWindow->setAttribute(Qt::WA_DeleteOnClose);
    layerPairAnalysisWindow->show();
}

void DSMainWindow::updateDisplayedLayer()
{
    ui->totalLayerNumLabel->setText(tr("из %1, шаг").
                                    arg(model->GetLayerCount()));
    ui->currentLayerEdit->setText(tr("%1").
                                  arg(model->GetCurrentLayerIndex()));
    ui->layerStepEdit->setText(tr("%1").
                               arg(model->GetLayerStep()));
    resetPlotsScale(model->GetActivatorMinimum(), model->GetActivatorMaximum(),
               model->GetInhibitorMinimum(), model->GetInhibitorMaximum());
    displayActivatorLayer(model->GetCurrentActivatorLayer());
    displayInhibitorLayer(model->GetCurrentInhibitorLayer());
}

void DSMainWindow::displayRunResults()
{
    updateDisplayedLayer();
    ui->layerPairAnalysisAction->setEnabled(true);
    // TODO: Show run info
}



/*
 * Private worker methods implementation
 */
void DSMainWindow::initPlots()
{
    ui->activatorPlot->addGraph();
    ui->activatorPlot->xAxis->setLabel("x");
    ui->activatorPlot->yAxis->setLabel("Концентрация активатора");
    ui->activatorPlot->xAxis->setRange(0, 1);

    ui->inhibitorPlot->addGraph();
    ui->inhibitorPlot->xAxis->setLabel("x");
    ui->inhibitorPlot->yAxis->setLabel("Концентрация ингибитора");
    ui->inhibitorPlot->xAxis->setRange(0, 1);

    // Scale y axes on plots with some junk values
    resetPlotsScale(0, 1, 0, 1);
}

void DSMainWindow::resetPlotsScale(double activatorMin, double activatorMax,
                              double inhibitorMin, double inhibitorMax)
{
    double yrange = activatorMax - activatorMin;
    ui->activatorPlot->yAxis->setRange(activatorMin - 0.05 * yrange,
                                       activatorMax + 0.05 * yrange);


    yrange = inhibitorMax - inhibitorMin;
    ui->inhibitorPlot->yAxis->setRange(inhibitorMin - 0.05 * yrange,
                                       inhibitorMax + 0.05 * yrange);
}

void DSMainWindow::displayActivatorLayer(const SchemeLayer& layer)
{
    QVector<double> xs, ys;
    int layerLength = layer.GetLength();
    int arrayStep = layerLength / maxPlotPointsNumber() + 1;
    double xStep = 1.0 / model->AccessParameters()->GetGridDimension();
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
    double xStep = 1.0 / model->AccessParameters()->GetGridDimension();
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

void DSMainWindow::showSolvingProgressDialog()
{
    solvingProgressDialog = new DSSolvingProgressDialog(model, this);
    solvingProgressDialog->setAttribute(Qt::WA_DeleteOnClose);
    solvingProgressDialog->show();
}
