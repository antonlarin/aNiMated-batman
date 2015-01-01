#include "dsinitconditionsdialog.hpp"
#include "ui_dsinitconditionsdialog.h"


DSInitConditionsDialog::DSInitConditionsDialog(DSModel* model,
                                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSInitConditionsDialog),
    model(model),
    selectedHarmonics(21, false),
    activeHarmonicControls(6, false),
    harmonicsSelectors(),
    activatorCoeffEdits(),
    inhibitorCoeffEdits()
{
    ui->setupUi(this);

    harmonicsSelectors.push_back(ui->harmonicCombo0);
    harmonicsSelectors.push_back(ui->harmonicCombo1);
    harmonicsSelectors.push_back(ui->harmonicCombo2);
    harmonicsSelectors.push_back(ui->harmonicCombo3);
    harmonicsSelectors.push_back(ui->harmonicCombo4);
    harmonicsSelectors.push_back(ui->harmonicCombo5);

    activatorCoeffEdits.push_back(ui->activatorEdit0);
    activatorCoeffEdits.push_back(ui->activatorEdit1);
    activatorCoeffEdits.push_back(ui->activatorEdit2);
    activatorCoeffEdits.push_back(ui->activatorEdit3);
    activatorCoeffEdits.push_back(ui->activatorEdit4);
    activatorCoeffEdits.push_back(ui->activatorEdit5);

    inhibitorCoeffEdits.push_back(ui->inhibitorEdit0);
    inhibitorCoeffEdits.push_back(ui->inhibitorEdit1);
    inhibitorCoeffEdits.push_back(ui->inhibitorEdit2);
    inhibitorCoeffEdits.push_back(ui->inhibitorEdit3);
    inhibitorCoeffEdits.push_back(ui->inhibitorEdit4);
    inhibitorCoeffEdits.push_back(ui->inhibitorEdit5);

    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(acceptInitialConditions()));

    connect(ui->harmonicCombo1, SIGNAL(currentIndexChanged(int)),
            this, SLOT(addHarmonic1()));
    connect(ui->harmonicCombo2, SIGNAL(currentIndexChanged(int)),
            this, SLOT(addHarmonic2()));
    connect(ui->harmonicCombo3, SIGNAL(currentIndexChanged(int)),
            this, SLOT(addHarmonic3()));
    connect(ui->harmonicCombo4, SIGNAL(currentIndexChanged(int)),
            this, SLOT(addHarmonic4()));
    connect(ui->harmonicCombo5, SIGNAL(currentIndexChanged(int)),
            this, SLOT(addHarmonic5()));

    connect(ui->removeHarmonicButton1, SIGNAL(clicked()),
            this, SLOT(removeHarmonic1()));
    connect(ui->removeHarmonicButton2, SIGNAL(clicked()),
            this, SLOT(removeHarmonic2()));
    connect(ui->removeHarmonicButton3, SIGNAL(clicked()),
            this, SLOT(removeHarmonic3()));
    connect(ui->removeHarmonicButton4, SIGNAL(clicked()),
            this, SLOT(removeHarmonic4()));
    connect(ui->removeHarmonicButton5, SIGNAL(clicked()),
            this, SLOT(removeHarmonic5()));

    for (int i = 0; i < 6; ++i)
    {
        connect(activatorCoeffEdits[i], SIGNAL(textEdited(QString)),
                this, SLOT(activatorHarmonicCoeffChanged(QString)));
        connect(inhibitorCoeffEdits[i], SIGNAL(textEdited(QString)),
                this, SLOT(inhibitorHarmonicCoeffChanged(QString)));

    }

    initInitalConditionsPlots();

    setSelectorItemLists();
    displayCurrentInitialConditions();
}

DSInitConditionsDialog::~DSInitConditionsDialog()
{
    delete ui;
}



/*
 * Slots implementation
 */


void DSInitConditionsDialog::acceptInitialConditions()
{
    std::vector<double> activatorCoeffs = getCurrentActivatorCoeffs();
    std::vector<double> inhibitorCoeffs = getCurrentInhibitorCoeffs();

    model->SetActivatorInitialConditions(activatorCoeffs);
    model->SetInhibitorInitialConditions(inhibitorCoeffs);
}

void DSInitConditionsDialog::addHarmonic1()
{
    addHarmonic(1);
}

void DSInitConditionsDialog::addHarmonic2()
{
    addHarmonic(2);
}

void DSInitConditionsDialog::addHarmonic3()
{
    addHarmonic(3);
}

void DSInitConditionsDialog::addHarmonic4()
{
    addHarmonic(4);
}

void DSInitConditionsDialog::addHarmonic5()
{
    addHarmonic(5);
}

void DSInitConditionsDialog::removeHarmonic1()
{
    removeHarmonic(1);
}

void DSInitConditionsDialog::removeHarmonic2()
{
    removeHarmonic(2);
}

void DSInitConditionsDialog::removeHarmonic3()
{
    removeHarmonic(3);
}

void DSInitConditionsDialog::removeHarmonic4()
{
    removeHarmonic(4);
}

void DSInitConditionsDialog::removeHarmonic5()
{
    removeHarmonic(5);
}

void DSInitConditionsDialog::activatorHarmonicCoeffChanged(const QString& value)
{
    bool valueIsValid;
    value.toDouble(&valueIsValid);
    if (valueIsValid)
    {
        updateSelectedHarmonics();
    }
}

void DSInitConditionsDialog::inhibitorHarmonicCoeffChanged(const QString& value)
{
    bool valueIsValid;
    value.toDouble(&valueIsValid);
    if (valueIsValid)
    {
        updateSelectedHarmonics();
    }
}



/*
 * Private worker methods implementation
 */
vector<double> DSInitConditionsDialog::getCurrentActivatorCoeffs()
{
    vector<double> activatorCoeffs(21, 0.0);

    for (auto i = selectedHarmonics.begin(); i != selectedHarmonics.end(); ++i)
    {
        int index = (int)(i - selectedHarmonics.begin());
        if (*i)
        {
            for (int j = 0; j < 6; ++j)
            {
                if (harmonicsSelectors[j]->currentText() == tr("%1").arg(index))
                {
                    activatorCoeffs[index] =
                        activatorCoeffEdits[j]->text().toDouble();
                    break;
                }
            }
        }
    }

    return activatorCoeffs;
}

vector<double> DSInitConditionsDialog::getCurrentInhibitorCoeffs()
{
    vector<double> inhibitorCoeffs(21, 0.0);

    for (auto i = selectedHarmonics.begin(); i != selectedHarmonics.end(); ++i)
    {
        int index = (int)(i - selectedHarmonics.begin());
        if (*i)
        {
            for (int j = 0; j < 6; ++j)
            {
                if (harmonicsSelectors[j]->currentText() == tr("%1").arg(index))
                {
                    inhibitorCoeffs[index] =
                        inhibitorCoeffEdits[j]->text().toDouble();
                    break;
                }
            }
        }
    }

    return inhibitorCoeffs;
}
void DSInitConditionsDialog::initInitalConditionsPlots()
{
    ui->activatorPreviewPlot->addGraph();
    ui->activatorPreviewPlot->graph(0)->setPen(QPen(Qt::darkGreen));
    ui->activatorPreviewPlot->xAxis->setRange(0.0, 1.0);
    ui->activatorPreviewPlot->yAxis->setAutoTickCount(4);

    ui->inhibitorPreviewPlot->addGraph();
    ui->inhibitorPreviewPlot->graph(0)->setPen(QPen(Qt::red));
    ui->inhibitorPreviewPlot->xAxis->setRange(0.0, 1.0);
    ui->inhibitorPreviewPlot->yAxis->setAutoTickCount(4);
}

void DSInitConditionsDialog::setSelectorItemLists()
{
    for (int i = 1; i < 6; ++i)
    {
        harmonicsSelectors[i]->insertItem(0, "N/A");
        for (int j = 1; j < 21; ++j)
        {
            harmonicsSelectors[i]->insertItem(j, tr("%1").arg(j));
        }
    }
}

void DSInitConditionsDialog::displayCurrentInitialConditions()
{
    vector<double> activatorCoeffs = model->GetActivatorInitialConditions();
    vector<double> inhibitorCoeffs = model->GetInhibitorInitialConditions();

    for (int i = 0; i < (int)activatorCoeffs.size(); ++i)
    {
        if (activatorCoeffs[i] != 0.0 || inhibitorCoeffs[i] != 0.0)
        {
            selectedHarmonics[i] = true;
            int j = 0;
            while (j < 6 && activeHarmonicControls[j]) j++;
            if (j == 6)
            {
                break;
            }
            else
            {
                activeHarmonicControls[j] = true;
                harmonicsSelectors[j]->setCurrentText(tr("%1").arg(i));
                activatorCoeffEdits[j]->setText(tr("%1").
                                                arg(activatorCoeffs[i]));
                inhibitorCoeffEdits[j]->setText(tr("%1").
                                                arg(inhibitorCoeffs[i]));
            }
        }
    }
    displayPlotsOfCurrentInitialConditions();
}

void DSInitConditionsDialog::displayPlotsOfCurrentInitialConditions()
{
    const int PLOT_SEGMENTS_NUM = 100;
    double plotGridStep = 1.0 / ((double) PLOT_SEGMENTS_NUM);

    QVector<double> xs;
    QVector<double> activatorValues;
    QVector<double> inhibitorValues;

    SchemeLayerGeneratorInitial generator;
    generator.SetIntervalsCount(PLOT_SEGMENTS_NUM);

    generator.SetCoefficients(getCurrentActivatorCoeffs());
    SchemeLayer activatorLayer = generator.Generate();

    generator.SetCoefficients(getCurrentInhibitorCoeffs());
    SchemeLayer inhibitorLayer = generator.Generate();

    for (int i = 0; i <= PLOT_SEGMENTS_NUM; ++i)
    {
        xs.push_back(i * plotGridStep);
        activatorValues.push_back(activatorLayer.Get(i));
        inhibitorValues.push_back(inhibitorLayer.Get(i));
    }

    ui->activatorPreviewPlot->graph(0)->setData(xs, activatorValues);
    ui->activatorPreviewPlot->rescaleAxes();
    ui->activatorPreviewPlot->replot();

    ui->inhibitorPreviewPlot->graph(0)->setData(xs, inhibitorValues);
    ui->inhibitorPreviewPlot->rescaleAxes();
    ui->inhibitorPreviewPlot->replot();
}

void DSInitConditionsDialog::addHarmonic(int index)
{
    if (harmonicsSelectors[index]->currentText() == "N/A")
    {
        activeHarmonicControls[index] = false;
        activatorCoeffEdits[index]->setText("0.0");
        inhibitorCoeffEdits[index]->setText("0.0");
    }
    else
    {
        activeHarmonicControls[index] = true;
    }
    updateSelectedHarmonics();
}

void DSInitConditionsDialog::removeHarmonic(int index)
{
    harmonicsSelectors[index]->setCurrentIndex(0);
    activeHarmonicControls[index] = false;
    updateSelectedHarmonics();
}

void DSInitConditionsDialog::updateSelectedHarmonics()
{
    for (auto it = selectedHarmonics.begin();
         it != selectedHarmonics.end(); ++it)
    {
        *it = false;
    }

    for (int i = 0; i < 6; ++i)
    {
        if (harmonicsSelectors[i]->currentText() != "N/A")
            selectedHarmonics[harmonicsSelectors[i]->currentText().toInt()] =
                    true;
    }
    displayPlotsOfCurrentInitialConditions();
}
