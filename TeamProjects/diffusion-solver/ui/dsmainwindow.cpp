#include "dsmainwindow.hpp"
#include "ui_dsmainwindow.h"

#include <vector>
#include "dsmodel.hpp"
#include "dssettingsmanager.hpp"

DSMainWindow::DSMainWindow(DSWindowManager* manager, QWidget *parent) :
    QMainWindow(parent),
    IDSWindow(manager),
    ui(new Ui::DSMainWindow)
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

    connect(ui->savingLayersRadioButton, SIGNAL(clicked()),
            this, SLOT(solvingModeChanged()));
    connect(ui->nonSavingLayersRadioButton, SIGNAL(clicked()),
            this, SLOT(solvingModeChanged()));
    connect(ui->layerSavingStepEdit, SIGNAL(textEdited(QString)),
            this, SLOT(layerSavingStepChanged(QString)));
    connect(ui->newRunButton, SIGNAL(clicked()),
            this, SLOT(startNewRun()));
    connect(ui->continueRunButton, SIGNAL(clicked()),
            this, SLOT(continueRun()));
    connect(ui->lastResultsButton, SIGNAL(clicked()),
            getManager(), SLOT(showSummaryDialog()));

    connect(ui->quitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui->initConditionsAction, SIGNAL(triggered()),
            getManager(), SLOT(showInitialConditionsDialog()));
    connect(ui->layerPairAnalysisAction, SIGNAL(triggered()),
            getManager(), SLOT(showLayerPairAnalysisWindow()));
    connect(ui->saveInhibitorPlotAction, SIGNAL(triggered()),
            this, SLOT(saveInhibitorPlot()));
    connect(ui->saveActivatorPlotAction, SIGNAL(triggered()),
            this, SLOT(saveActivatorPlot()));
    connect(ui->showEquationsHelpAction, SIGNAL(triggered()),
            getManager(), SLOT(showEquationHelpDialog()));
    connect(ui->saveParametersAction, SIGNAL(triggered()),
        this, SLOT(saveEquationsParameters()));
    connect(ui->loadParametersAction, SIGNAL(triggered()),
            this, SLOT(loadEquationsParameters()));
    connect(ui->showEquilibriumHelpAction, SIGNAL(triggered()),
            this, SLOT(showSystemEquilibriumInformation()));

    DSModel* model = getManager()->getModel();
    connect(model, SIGNAL(LayerIndexChanged()),
            this, SLOT(showSelectedLayer()));
    connect(model, SIGNAL(ResultAcquired()),
            this, SLOT(displayRunResults()));
    connect(model, SIGNAL(ResultChanged(const SchemeSolverIterationInfo&)),
            this, SLOT(updateModelResult(const SchemeSolverIterationInfo&)));
    connect(model, SIGNAL(SolverError(const DSSolverException&)),
            this, SLOT(modelSolverError(const DSSolverException&)));

    connect(ui->explicitSolverRadioButton, SIGNAL(clicked()),
            model, SLOT(SelectExplicitSolver()));
    connect(ui->implicitSolverRadioButton, SIGNAL(clicked()),
            model, SLOT(SelectImplicitSolver()));

    ui->plotControlPanel->setVisible(false);
    initPlots();
}

DSMainWindow::~DSMainWindow()
{
    delete ui;
}

void DSMainWindow::showWindow()
{
    show();
}

/*
 * Slots implementations
 */
void DSMainWindow::lambda1CoeffChanged(const QString& newLambda1)
{
    bool isValidDouble;
    double value = newLambda1.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetLambda1(value);
}

void DSMainWindow::lambda2CoeffChanged(const QString& newLambda2)
{
    bool isValidDouble;
    double value = newLambda2.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetLambda2(value);
}

void DSMainWindow::kCoeffChanged(const QString& newK)
{
    bool isValidDouble;
    double value = newK.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetK(value);
}

void DSMainWindow::cCoeffChanged(const QString& newC)
{
    bool isValidDouble;
    double value = newC.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetC(value);
}

void DSMainWindow::rhoCoeffChanged(const QString& newRho)
{
    bool isValidDouble;
    double value = newRho.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetRho(value);
}

void DSMainWindow::nuCoeffChanged(const QString& newNu)
{
    bool isValidDouble;
    double value = newNu.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetNu(value);
}

void DSMainWindow::gammaCoeffChanged(const QString& newGamma)
{
    bool isValidDouble;
    double value = newGamma.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetGamma(value);
}

void DSMainWindow::gridDimensionChanged(const QString& newGridDimension)
{
    bool isValidInteger;
    int value = newGridDimension.toInt(&isValidInteger);
    if (isValidInteger)
        getManager()->getModel()->AccessParameters()->SetGridDimension(value);
}

void DSMainWindow::timeStepChanged(const QString& newTimeStep)
{
    bool isValidDouble;
    double value = newTimeStep.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->SetTimeStep(value);
}

void DSMainWindow::activatorAccuracyChanged(const QString& newAccuracy)
{
    bool isValidDouble;
    double value = newAccuracy.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->
                SetActivatorAccuracy(value);
}

void DSMainWindow::inhibitorAccuracyChanged(const QString& newAccuracy)
{
    bool isValidDouble;
    double value = newAccuracy.toDouble(&isValidDouble);
    if (isValidDouble)
        getManager()->getModel()->AccessParameters()->
                SetInhibitorAccuracy(value);
}

void DSMainWindow::iterationsLimitChanged(const QString& newIterationsLimit)
{
    bool isValidInteger;
    int value = newIterationsLimit.toInt(&isValidInteger);
    if (isValidInteger)
        getManager()->getModel()->AccessParameters()->SetIterationsLimit(value);
}

void DSMainWindow::solvingModeChanged()
{
    SchemeSolverMode value;
    if (ui->savingLayersRadioButton->isChecked())
    {
        value = SchemeSolverMode::AllLayers;
    }
    else // if (ui->nonSavingLayerRadioButton->isChecked())
    {
        value = SchemeSolverMode::StableLayer;
    }

    getManager()->getModel()->AccessParameters()->SetSolvingMode(value);
}

void DSMainWindow::layerSavingStepChanged(const QString& newLayerSavingStep)
{
    bool isValidInteger;
    int value = newLayerSavingStep.toInt(&isValidInteger);
    if (isValidInteger)
        getManager()->getModel()->AccessParameters()->SetLayerSavingStep(value);
}

void DSMainWindow::startNewRun()
{
    try
    {
        this->showWarningMessages();
        getManager()->getModel()->StartRun();
        getManager()->showSolvingProgressDialog();
    }
    catch (std::runtime_error e)
    {
        QMessageBox::critical(this, QString("Неверные параметры"),
                              QString(e.what()));
    }
}

void DSMainWindow::continueRun()
{
    getManager()->getModel()->ContinueRun();
    getManager()->showSolvingProgressDialog();
}

void DSMainWindow::showLastRunResults() {

}

void DSMainWindow::goToPrevLayer()
{
    DSModel* model = getManager()->getModel();
    model->SetCurrentLayerIndex(model->GetCurrentLayerIndex() -
                                  model->GetLayerStep());
}

void DSMainWindow::goToNextLayer()
{
    DSModel* model = getManager()->getModel();
    model->SetCurrentLayerIndex(model->GetCurrentLayerIndex() +
                                model->GetLayerStep());
}

void DSMainWindow::goToFirstLayer()
{
    getManager()->getModel()->SetCurrentLayerIndex(0);
}

void DSMainWindow::goToLastLayer()
{
    DSModel* model = getManager()->getModel();
    model->SetCurrentLayerIndex(model->GetLayerCount() - 1);
}

void DSMainWindow::goToLayer(const QString& newLayer)
{
    bool isValidInteger;
    int value = newLayer.toInt(&isValidInteger);
    if (isValidInteger)
        getManager()->getModel()->SetCurrentLayerIndex(value);
}

void DSMainWindow::changeLayerStep(const QString& newLayerStep)
{
    bool isValidInteger;
    int value = newLayerStep.toInt(&isValidInteger);
    if (isValidInteger)
        getManager()->getModel()->SetLayerStep(value);
}

void DSMainWindow::showSelectedLayer()
{
    DSModel* model = getManager()->getModel();
    ui->totalLayerNumLabel->setText(
        QString("из %1, шаг").arg(model->GetAvailableLayerCount())
    );
    ui->currentLayerEdit->setText(
        QString("%1").arg(model->GetCurrentLayerIndex())
    );
    ui->layerStepEdit->setText(
        QString("%1").arg(model->GetLayerStep())
    );
    ui->currentLayerTimeLabel->setText(
        QString(", t = %1").arg(model->GetCurrentLayerTime())
    );

    displayActivatorLayer(model->GetCurrentActivatorLayer().Weak());
    displayInhibitorLayer(model->GetCurrentInhibitorLayer().Weak());
}

void DSMainWindow::displayRunResults()
{
    DSModel* model = getManager()->getModel();
    model->SetCurrentLayerIndex(model->GetLayerCount() - 1);
    resetPlotsScale(model->GetActivatorMinimum(), model->GetActivatorMaximum(),
                    model->GetInhibitorMinimum(), model->GetInhibitorMaximum());
    showSelectedLayer();
    ui->plotControlPanel->setVisible(true);
    ui->layerPairAnalysisAction->setEnabled(true);
    ui->lastResultsButton->setEnabled(true);
    ui->continueRunButton->setEnabled(model->IsContinuationAvailable());
}

void DSMainWindow::updateModelResult(const SchemeSolverIterationInfo& result)
{
    resetPlotsScale(result.GetMinValueU1(),
                    result.GetMaxValueU1(),
                    result.GetMinValueU2(),
                    result.GetMaxValueU2());

    SchemeWeakLayer activator = result.GetCurrentLayerU1();
    SchemeWeakLayer inhibitor = result.GetCurrentLayerU2();
    displayActivatorLayer(activator);
    displayInhibitorLayer(inhibitor);
}

void DSMainWindow::modelSolverError(const DSSolverException& ex)
{
    // TODO: Replace error message to error code
    QMessageBox::critical(this, "Ошибка", ex.GetSource().what());
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
    ui->activatorPlot->graph(0)->setPen(QPen(QColor(0, 255, 0)));

    ui->inhibitorPlot->addGraph();
    ui->inhibitorPlot->xAxis->setLabel("x");
    ui->inhibitorPlot->yAxis->setLabel("Концентрация ингибитора");
    ui->inhibitorPlot->xAxis->setRange(0, 1);
    ui->inhibitorPlot->graph(0)->setPen(QPen(QColor(255, 0, 0)));

    // Scale y axes on plots with some junk values
    resetPlotsScale(0, 1, 0, 1);
}

void DSMainWindow::resetPlotsScale(double activatorMin, double activatorMax,
                                   double inhibitorMin, double inhibitorMax)
{
    double activatorYRange = activatorMax - activatorMin;
    double activatorPlotMargin = plotRelativeYMargin() *
            std::max(activatorYRange, minPlotYRange());
    ui->activatorPlot->yAxis->setRange(activatorMin - activatorPlotMargin,
                                       activatorMax + activatorPlotMargin);


    double inhibitorYRange = inhibitorMax - inhibitorMin;
    double inhibitorPlotMargin = plotRelativeYMargin() *
            std::max(inhibitorYRange, minPlotYRange());
    ui->inhibitorPlot->yAxis->setRange(inhibitorMin - inhibitorPlotMargin,
                                       inhibitorMax + inhibitorPlotMargin);
}

void DSMainWindow::displayActivatorLayer(const SchemeWeakLayer& layer)
{
    QVector<double> xs, ys;
    int layerLength = layer.GetLength();
    int arrayStep = layerLength / maxPlotPointsNumber() + 1;
    double xStep = 1.0 / getManager()->getModel()->AccessParameters()->
            GetGridDimension();
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

void DSMainWindow::displayInhibitorLayer(const SchemeWeakLayer& layer)
{
    QVector<double> xs, ys;
    int layerLength = layer.GetLength();
    int arrayStep = layerLength / maxPlotPointsNumber() + 1;
    double xStep = 1.0 / getManager()->getModel()->AccessParameters()->
            GetGridDimension();
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
void DSMainWindow::saveActivatorPlot()
{
    QString filePath = QFileDialog::getSaveFileName(0,
                      "Сохранить график концентрации активатора", "", "*.png");

    if(filePath.indexOf(".png") < 0 && !filePath.isEmpty())
        filePath += ".png";

    if(!filePath.isEmpty())
    {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
        {
            QMessageBox::warning(0,"Ошибка сохранения файла",
                       QObject::tr( "\n Невозможно создать файл"));
        }
        else    {
            ui->activatorPlot->savePng(filePath, 640, 480, 1.0, -1);
            file.close();
        }
    }
}
void DSMainWindow::saveInhibitorPlot()
{
    QString filePath = QFileDialog::getSaveFileName(0,
                      "Сохранить график концентрации ингибитора", "", "*.png");

    if(filePath.indexOf(".png") < 0 && !filePath.isEmpty())
        filePath += ".png";

    if(!filePath.isEmpty())
    {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
        {
            QMessageBox::warning(0,"Ошибка сохранения файла",
                       QObject::tr( "\n Невозможно создать файл"));
        }
        else    {
            ui->inhibitorPlot->savePng(filePath, 640, 480, 1.0, -1);
            file.close();
        }
    }
}
void DSMainWindow::showWarningMessages()
{
    if( ui->explicitSolverRadioButton->isChecked())
    {
        double lambda1 = getManager()->getModel()->AccessParameters()->GetLambda1();
        double lambda2 = getManager()->getModel()->AccessParameters()->GetLambda2();
        double h = 1.0/getManager()->getModel()->AccessParameters()->GetGridDimension();
        double k = getManager()->getModel()->AccessParameters()->GetTimeStep();

        if(k>h*h/std::max(lambda1, lambda2)/2)
            QMessageBox::critical(this, QString("Неверные параметры"),
                                  QString("При выбранных параметрах явная схема неустойчива"));
    }
}
void DSMainWindow::saveEquationsParameters()
{
    QString filePath = QFileDialog::getSaveFileName(0,
                      "Сохранить параметры системы уравнений", "", "*.xml");

    if(!filePath.isEmpty())    {
        if(filePath.indexOf(".xml") < 0)
            filePath += ".xml";
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))  {
            QMessageBox::warning(0,"Ошибка сохранения файла",
                       QObject::tr( "\n Невозможно создать файл"));
        }
        else    {
            getManager()->getModel()->AccessSettingsManager()->saveSettings(file);
            file.close();
        }
    }
}
void DSMainWindow::loadEquationsParameters()
{
    QString filePath = QFileDialog::getOpenFileName(0,
                      "Загрузить параметры системы уравнений", "", "*.xml");
    if(!filePath.isEmpty())
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly|QFile::ReadOnly))    {
            QMessageBox::warning(0,"Ошибка открытия файла",
                       QObject::tr( "\n Невозможно открыть файл"));
        }
        else    {
            try     {

            DSParameterSet* currentParameters =
                        getManager()->getModel()->AccessParameters();
            getManager()->getModel()->AccessSettingsManager()->loadSettings(file);

            ui->lambda1Edit->setText(QString::number(currentParameters->GetLambda1(), 'g', 16));
            ui->lambda2Edit->setText(QString::number(currentParameters->GetLambda2(), 'g', 16));
            ui->cEdit->setText(QString::number(currentParameters->GetC(), 'g', 16));
            ui->kEdit->setText(QString::number(currentParameters->GetK(), 'g', 16));
            ui->rhoEdit->setText(QString::number(currentParameters->GetRho(), 'g', 16));
            ui->nuEdit->setText(QString::number(currentParameters->GetNu(), 'g', 16));
            ui->gammaEdit->setText(QString::number(currentParameters->GetGamma(), 'g', 16));
            ui->timeStepEdit->setText(QString::number(currentParameters->GetTimeStep(), 'g', 16));
            ui->activatorAccuracyEdit->setText(QString::number(currentParameters->GetActivatorAccuracy(), 'g', 16));
            ui->inhibitorAccuracyEdit->setText(QString::number(currentParameters->GetInhibitorAccuracy(), 'g', 16));
            ui->gridDimEdit->setText(QString::number(currentParameters->GetGridDimension()));
            ui->iterationsEdit->setText(QString::number(currentParameters->GetIterationsLimit()));

            }
            catch(std::exception exp)   {
                QMessageBox::critical(this, QString("Ошибка"),
                  QString(exp.what()));
            }
            file.close();
        }
    }
}
void DSMainWindow::showSystemEquilibriumInformation()
{
    DSParameterSet* currentParameters =
            getManager()->getModel()->AccessParameters();
    double u1 = (currentParameters->GetRho() +
                 currentParameters->GetK()*currentParameters->GetNu()/currentParameters->GetC())/
            currentParameters->GetGamma();
    double u2 = u1*u1*currentParameters->GetC()/currentParameters->GetNu();
    QString infoMessage("");
    infoMessage+="Стационарное однородное по пространству решение: ( ";
    infoMessage+=QString::number(u1) + QString(" ; ") + QString::number(u2) + QString(" )\n");
    QMessageBox::information(0,"Информация о стационарном решении", infoMessage);
}
