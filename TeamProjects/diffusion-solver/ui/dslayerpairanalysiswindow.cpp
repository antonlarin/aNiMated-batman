#include "dslayerpairanalysiswindow.hpp"
#include "ui_dslayerpairanalysiswindow.h"

DSLayerPairAnalysisWindow::DSLayerPairAnalysisWindow(DSModel* model, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DSLayerPairAnalysisWindow),
    model(model)
{
    ui->setupUi(this);

    connect(ui->layer1Edit, SIGNAL(textEdited(QString)),
            this, SLOT(firstLayerIndexChanged(QString)));
    connect(ui->layer2Edit, SIGNAL(textEdited(QString)),
            this, SLOT(secondLayerIndexChanged(QString)));

    connect(model, SIGNAL(comparedLayersChanged()),
            this, SLOT(updateComparisonInfo()));

    initPlots();
    updateComparisonInfo();
}

DSLayerPairAnalysisWindow::~DSLayerPairAnalysisWindow()
{
    delete ui;
}



/*
 * Slots implementations
 */
void DSLayerPairAnalysisWindow::firstLayerIndexChanged(const QString& newIndex)
{
    bool isValidInteger;
    int indexValue = newIndex.toInt(&isValidInteger);
    if (isValidInteger)
        model->SetFirstComparedLayerIndex(indexValue);
}

void DSLayerPairAnalysisWindow::secondLayerIndexChanged(const QString& newIndex)
{
    bool isValidInteger;
    int indexValue = newIndex.toInt(&isValidInteger);
    if (isValidInteger)
        model->SetSecondComparedLayerIndex(indexValue);
}



/*
 * Private worker methods implementations
 */
void DSLayerPairAnalysisWindow::initPlots()
{
    ui->activatorsPlot->addGraph();
    ui->activatorsPlot->addGraph();
    ui->activatorsPlot->graph(0)->setName("Слой 1");
    ui->activatorsPlot->graph(0)->setPen(QPen(Qt::magenta));
    ui->activatorsPlot->graph(1)->setName("Слой 2");
    ui->activatorsPlot->graph(1)->setPen(QPen(Qt::cyan));
    ui->activatorsPlot->xAxis->setRange(0.0, 1.0);
    ui->activatorsPlot->legend->setVisible(true);

    ui->inhibitorsPlot->addGraph();
    ui->inhibitorsPlot->addGraph();
    ui->inhibitorsPlot->graph(0)->setName("Слой 1");
    ui->inhibitorsPlot->graph(0)->setPen(QPen(Qt::darkMagenta));
    ui->inhibitorsPlot->graph(1)->setName("Слой 2");
    ui->inhibitorsPlot->graph(1)->setPen(QPen(Qt::darkCyan));
    ui->inhibitorsPlot->xAxis->setRange(0.0, 1.0);
    ui->inhibitorsPlot->legend->setVisible(true);

    ui->activatorsDifferencePlot->addGraph();
    ui->activatorsDifferencePlot->xAxis->setRange(0.0, 1.0);

    ui->inhibitorsDifferencePlot->addGraph();
    ui->inhibitorsDifferencePlot->xAxis->setRange(0.0, 1.0);
}

void DSLayerPairAnalysisWindow::updateComparisonInfo()
{
    ui->totalLayerCountLabel->setText(
                tr("Слоёв доступно для сравнения: %1").
                arg(model->GetLayerCount()));

    SchemeLayer activatorLayer1 =
            model->GetActivatorLayer(model->GetFirstComparedLayerIndex());
    SchemeLayer inhibitorLayer1 =
            model->GetInhibitorLayer(model->GetFirstComparedLayerIndex());
    SchemeLayer activatorLayer2 =
            model->GetActivatorLayer(model->GetSecondComparedLayerIndex());
    SchemeLayer inhibitorLayer2 =
            model->GetInhibitorLayer(model->GetSecondComparedLayerIndex());

    ui->activatorsDifferenceNormLabel->setText(
                tr("Норма разности слоёв: %1").
                arg(SchemeLayer::MaxDifference(activatorLayer1,
                                               activatorLayer2)));
    ui->inhibitorsDifferenceNormLabel->setText(
                tr("Норма разности слоёв: %1").
                arg(SchemeLayer::MaxDifference(inhibitorLayer1,
                                               inhibitorLayer2)));

    QVector<double> xs;
    QVector<double> activator1Values;
    QVector<double> activator2Values;
    QVector<double> activatorDifferenceValues;
    QVector<double> inhibitor1Values;
    QVector<double> inhibitor2Values;
    QVector<double> inhibitorDifferenceValues;

    int layerLength = activatorLayer1.GetLength();
    int loopStep = layerLength / GetMaxPlotPointsCount() + 1;
    double gridStep = 1.0 / ((double) layerLength);
    for (int i = 0; i < layerLength - 1; i += loopStep)
    {
        xs.push_back(i * gridStep);
        activator1Values.push_back(activatorLayer1.Get(i));
        activator2Values.push_back(activatorLayer2.Get(i));
        activatorDifferenceValues.push_back(
                    abs(activatorLayer1.Get(i) - activatorLayer2.Get(i)));
        inhibitor1Values.push_back(inhibitorLayer1.Get(i));
        inhibitor2Values.push_back(inhibitorLayer2.Get(i));
        inhibitorDifferenceValues.push_back(
                    abs(inhibitorLayer1.Get(i) - inhibitorLayer2.Get(i)));
    }

    ui->activatorsPlot->graph(0)->setData(xs, activator1Values);
    ui->activatorsPlot->graph(1)->setData(xs, activator2Values);
    ui->activatorsPlot->rescaleAxes();
    ui->activatorsPlot->replot();

    ui->activatorsDifferencePlot->graph(0)->setData(xs, activatorDifferenceValues);
    ui->activatorsDifferencePlot->rescaleAxes();
    ui->activatorsDifferencePlot->replot();

    ui->inhibitorsPlot->graph(0)->setData(xs, inhibitor1Values);
    ui->inhibitorsPlot->graph(1)->setData(xs, inhibitor2Values);
    ui->inhibitorsPlot->rescaleAxes();
    ui->inhibitorsPlot->replot();

    ui->inhibitorsDifferencePlot->graph(0)->setData(xs, inhibitorDifferenceValues);
    ui->inhibitorsDifferencePlot->rescaleAxes();
    ui->inhibitorsDifferencePlot->replot();
}
