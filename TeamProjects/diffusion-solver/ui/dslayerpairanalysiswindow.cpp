#include "dslayerpairanalysiswindow.hpp"
#include "ui_dslayerpairanalysiswindow.h"

DSLayerPairAnalysisWindow::DSLayerPairAnalysisWindow(DSWindowManager* manager,
                                                     QWidget* parent) :
    QDialog(parent),
    IDSWindow(manager),
    ui(new Ui::DSLayerPairAnalysisWindow)
{
    ui->setupUi(this);

    connect(ui->layer1Edit, SIGNAL(textEdited(QString)),
            this, SLOT(firstLayerIndexChanged(QString)));
    connect(ui->layer2Edit, SIGNAL(textEdited(QString)),
            this, SLOT(secondLayerIndexChanged(QString)));
    connect(ui->layer1Edit, SIGNAL(editingFinished()),
            this, SLOT(updateComparedLayerEditsText()));
    connect(ui->layer2Edit, SIGNAL(editingFinished()),
            this, SLOT(updateComparedLayerEditsText()));

    connect(getManager()->getModel(), SIGNAL(comparedLayersChanged()),
            this, SLOT(updateComparisonInfo()));

    initPlots();
    updateComparisonInfo();
}

DSLayerPairAnalysisWindow::~DSLayerPairAnalysisWindow()
{
    delete ui;
}

void DSLayerPairAnalysisWindow::showWindow()
{
    show();
}



/*
 * Slots implementations
 */
void DSLayerPairAnalysisWindow::firstLayerIndexChanged(const QString& newIndex)
{
    bool isValidInteger;
    int indexValue = newIndex.toInt(&isValidInteger);
    if (isValidInteger)
        getManager()->getModel()->SetFirstComparedLayerIndex(indexValue);
}

void DSLayerPairAnalysisWindow::secondLayerIndexChanged(const QString& newIndex)
{
    bool isValidInteger;
    int indexValue = newIndex.toInt(&isValidInteger);
    if (isValidInteger)
        getManager()->getModel()->SetSecondComparedLayerIndex(indexValue);
}


void DSLayerPairAnalysisWindow::updateComparisonInfo()
{
    DSModel* model = getManager()->getModel();
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

    updatePlots(activatorLayer1, activatorLayer2,
                inhibitorLayer1, inhibitorLayer2);
}

void DSLayerPairAnalysisWindow::updateComparedLayerEditsText()
{
    DSModel* model = getManager()->getModel();
    ui->layer1Edit->setText(tr("%1").
                            arg(model->GetFirstComparedLayerIndex()));
    ui->layer2Edit->setText(tr("%1").
                            arg(model->GetSecondComparedLayerIndex()));
}



/*
 * Other methods implementations
 */
void DSLayerPairAnalysisWindow::initPlots()
{
    ui->activatorsPlot->addGraph();
    ui->activatorsPlot->addGraph();
    ui->activatorsPlot->graph(0)->setName("Слой 1");
    ui->activatorsPlot->graph(0)->setPen(QPen(Qt::green));
    ui->activatorsPlot->graph(1)->setName("Слой 2");
    ui->activatorsPlot->graph(1)->setPen(QPen(Qt::darkGreen));
    ui->activatorsPlot->xAxis->setRange(0.0, 1.0);
    ui->activatorsPlot->yAxis->setLabel("Концентрации");
    ui->activatorsPlot->legend->setVisible(true);

    ui->inhibitorsPlot->addGraph();
    ui->inhibitorsPlot->addGraph();
    ui->inhibitorsPlot->graph(0)->setName("Слой 1");
    ui->inhibitorsPlot->graph(0)->setPen(QPen(Qt::red));
    ui->inhibitorsPlot->graph(1)->setName("Слой 2");
    ui->inhibitorsPlot->graph(1)->setPen(QPen(Qt::magenta));
    ui->inhibitorsPlot->xAxis->setRange(0.0, 1.0);
    ui->inhibitorsPlot->yAxis->setLabel("Концентрации");
    ui->inhibitorsPlot->legend->setVisible(true);

    ui->activatorsDifferencePlot->addGraph();
    ui->activatorsDifferencePlot->xAxis->setRange(0.0, 1.0);
    ui->activatorsDifferencePlot->yAxis->setLabel("Разность слоёв");

    ui->inhibitorsDifferencePlot->addGraph();
    ui->inhibitorsDifferencePlot->xAxis->setRange(0.0, 1.0);
    ui->inhibitorsDifferencePlot->yAxis->setLabel("Разность слоёв");
}

void DSLayerPairAnalysisWindow::updatePlots(const SchemeLayer& activatorLayer1,
                                            const SchemeLayer& activatorLayer2,
                                            const SchemeLayer& inhibitorLayer1,
                                            const SchemeLayer& inhibitorLayer2)
{
    DSModel* model = getManager()->getModel();
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
                    fabs(activatorLayer1.Get(i) - activatorLayer2.Get(i)));
        inhibitor1Values.push_back(inhibitorLayer1.Get(i));
        inhibitor2Values.push_back(inhibitorLayer2.Get(i));
        inhibitorDifferenceValues.push_back(
                    fabs(inhibitorLayer1.Get(i) - inhibitorLayer2.Get(i)));
    }

    ui->activatorsPlot->graph(0)->setData(xs, activator1Values);
    ui->activatorsPlot->graph(1)->setData(xs, activator2Values);
    ui->activatorsPlot->graph(0)->setName(
                tr("Слой %1").arg(model->GetFirstComparedLayerIndex()));
    ui->activatorsPlot->graph(1)->setName(
                tr("Слой %1").arg(model->GetSecondComparedLayerIndex()));
    ui->activatorsPlot->rescaleAxes();
    ui->activatorsPlot->replot();

    ui->activatorsDifferencePlot->graph(0)->
            setData(xs, activatorDifferenceValues);
    ui->activatorsDifferencePlot->rescaleAxes();
    ui->activatorsDifferencePlot->replot();

    ui->inhibitorsPlot->graph(0)->setData(xs, inhibitor1Values);
    ui->inhibitorsPlot->graph(1)->setData(xs, inhibitor2Values);
    ui->inhibitorsPlot->graph(0)->setName(
                tr("Слой %1").arg(model->GetFirstComparedLayerIndex()));
    ui->inhibitorsPlot->graph(1)->setName(
                tr("Слой %1").arg(model->GetSecondComparedLayerIndex()));
    ui->inhibitorsPlot->rescaleAxes();
    ui->inhibitorsPlot->replot();

    ui->inhibitorsDifferencePlot->graph(0)->
            setData(xs, inhibitorDifferenceValues);
    ui->inhibitorsDifferencePlot->rescaleAxes();
    ui->inhibitorsDifferencePlot->replot();
}
