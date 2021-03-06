#include "dsinitconditionsdialog.hpp"
#include "ui_dsinitconditionsdialog.h"


DSInitConditionsDialog::DSInitConditionsDialog(DSWindowManager* manager,
                                               QWidget *parent) :
    QDialog(parent),
    IDSWindow(manager),
    ui(new Ui::DSInitConditionsDialog),
    selectedHarmonics(maxHarmonicOrder() + 1, false),
    activeHarmonicControls(maxHarmonicsCount(), false),
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

    QSignalMapper* harmonicsAdditionMapper = new QSignalMapper(this);
    for (int i = 0; i < maxHarmonicsCount(); ++i)
    {
        harmonicsAdditionMapper->setMapping(harmonicsSelectors[i], i);
    }
    connect(ui->harmonicCombo1, SIGNAL(currentIndexChanged(int)),
            harmonicsAdditionMapper, SLOT(map()));
    connect(ui->harmonicCombo2, SIGNAL(currentIndexChanged(int)),
            harmonicsAdditionMapper, SLOT(map()));
    connect(ui->harmonicCombo3, SIGNAL(currentIndexChanged(int)),
            harmonicsAdditionMapper, SLOT(map()));
    connect(ui->harmonicCombo4, SIGNAL(currentIndexChanged(int)),
            harmonicsAdditionMapper, SLOT(map()));
    connect(ui->harmonicCombo5, SIGNAL(currentIndexChanged(int)),
            harmonicsAdditionMapper, SLOT(map()));

    connect(harmonicsAdditionMapper, SIGNAL(mapped(int)),
            this, SLOT(addHarmonic(int)));


    QSignalMapper* harmonicsRemovalMapper = new QSignalMapper(this);
    harmonicsRemovalMapper->setMapping(ui->removeHarmonicButton1, 1);
    harmonicsRemovalMapper->setMapping(ui->removeHarmonicButton2, 2);
    harmonicsRemovalMapper->setMapping(ui->removeHarmonicButton3, 3);
    harmonicsRemovalMapper->setMapping(ui->removeHarmonicButton4, 4);
    harmonicsRemovalMapper->setMapping(ui->removeHarmonicButton5, 5);
    connect(ui->removeHarmonicButton1, SIGNAL(clicked()),
            harmonicsRemovalMapper, SLOT(map()));
    connect(ui->removeHarmonicButton2, SIGNAL(clicked()),
            harmonicsRemovalMapper, SLOT(map()));
    connect(ui->removeHarmonicButton3, SIGNAL(clicked()),
            harmonicsRemovalMapper, SLOT(map()));
    connect(ui->removeHarmonicButton4, SIGNAL(clicked()),
            harmonicsRemovalMapper, SLOT(map()));
    connect(ui->removeHarmonicButton5, SIGNAL(clicked()),
            harmonicsRemovalMapper, SLOT(map()));

    connect(harmonicsRemovalMapper, SIGNAL(mapped(int)),
            this, SLOT(removeHarmonic(int)));

    for (int i = 0; i < maxHarmonicsCount(); ++i)
    {
        connect(activatorCoeffEdits[i], SIGNAL(textEdited(QString)),
                this, SLOT(activatorHarmonicCoeffChanged(QString)));
        connect(inhibitorCoeffEdits[i], SIGNAL(textEdited(QString)),
                this, SLOT(inhibitorHarmonicCoeffChanged(QString)));
    }

    initPlots();
    populateSelectorItemLists();
    displayCurrentInitialConditions();
    displayPlotsOfCurrentInitialConditions();
}

DSInitConditionsDialog::~DSInitConditionsDialog()
{
    delete ui;
}

void DSInitConditionsDialog::showWindow()
{
    show();
}



/*
 * Slots implementation
 */


void DSInitConditionsDialog::acceptInitialConditions()
{
    std::vector<double> activatorCoeffs = getCurrentActivatorCoeffs();
    std::vector<double> inhibitorCoeffs = getCurrentInhibitorCoeffs();

    DSModel* model = getManager()->getModel();
    QString errMessage;
    bool hasErr = false;
    if (!model->AccessParameters()->SetActivatorInitialConditions(activatorCoeffs))
    {
        hasErr = true;
        errMessage += "Начальный слой активатора должен быть положительным "
                      "на отрезке [0, 1].\n";
    }

    if (!model->AccessParameters()->SetInhibitorInitialConditions(inhibitorCoeffs))
    {
        hasErr = true;
        errMessage += "Начальный слой ингибитора должен быть положительным "
                      "на отрезке [0, 1].\n";
    }

    if (hasErr)
    {
        errMessage += "Необходимо изменить коэффициенты.";
        QMessageBox::information(this, "Некорректные параметры", errMessage);
    }
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
    displayPlotsOfCurrentInitialConditions();
}

void DSInitConditionsDialog::removeHarmonic(int index)
{
    harmonicsSelectors[index]->setCurrentIndex(0);
    activeHarmonicControls[index] = false;
    updateSelectedHarmonics();
    displayPlotsOfCurrentInitialConditions();
}

void DSInitConditionsDialog::activatorHarmonicCoeffChanged(const QString& value)
{
    bool isValidDouble;
    value.toDouble(&isValidDouble);
    if (isValidDouble)
    {
        updateSelectedHarmonics();
        displayPlotsOfCurrentInitialConditions();
    }
}

void DSInitConditionsDialog::inhibitorHarmonicCoeffChanged(const QString& value)
{
    bool isValidDouble;
    value.toDouble(&isValidDouble);
    if (isValidDouble)
    {
        updateSelectedHarmonics();
        displayPlotsOfCurrentInitialConditions();
    }
}



/*
 * Other methods implementation
 */
vector<double> DSInitConditionsDialog::getCurrentActivatorCoeffs()
{
    vector<double> activatorCoeffs(maxHarmonicOrder() + 1, 0.0);

    for (auto i = selectedHarmonics.begin(); i != selectedHarmonics.end(); ++i)
    {
        int index = (int)(i - selectedHarmonics.begin());
        if (*i)
        {
            for (int j = 0; j < maxHarmonicsCount(); ++j)
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
    vector<double> inhibitorCoeffs(maxHarmonicOrder() + 1, 0.0);

    for (auto i = selectedHarmonics.begin(); i != selectedHarmonics.end(); ++i)
    {
        int index = (int)(i - selectedHarmonics.begin());
        if (*i)
        {
            for (int j = 0; j < maxHarmonicsCount(); ++j)
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
void DSInitConditionsDialog::initPlots()
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

void DSInitConditionsDialog::populateSelectorItemLists()
{
    for (int i = 1; i < maxHarmonicsCount(); ++i)
    {
        harmonicsSelectors[i]->insertItem(0, "N/A");
        for (int j = 1; j < maxHarmonicOrder() + 1; ++j)
        {
            harmonicsSelectors[i]->insertItem(j, tr("%1").arg(j));
        }
    }
}

void DSInitConditionsDialog::displayCurrentInitialConditions()
{
    DSModel* model = getManager()->getModel();
    vector<double> activatorCoeffs =
            model->AccessParameters()->GetActivatorInitialConditions();
    vector<double> inhibitorCoeffs =
            model->AccessParameters()->GetInhibitorInitialConditions();

    for (int i = 0; i < (int)activatorCoeffs.size(); ++i)
    {
        if (activatorCoeffs[i] != 0.0 || inhibitorCoeffs[i] != 0.0)
        {
            selectedHarmonics[i] = true;
            int j = 0;
            while (j < maxHarmonicsCount() && activeHarmonicControls[j]) j++;
            if (j == maxHarmonicsCount())
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

void DSInitConditionsDialog::updateSelectedHarmonics()
{
    for (auto it = selectedHarmonics.begin();
         it != selectedHarmonics.end(); ++it)
    {
        *it = false;
    }

    for (int i = 0; i < maxHarmonicsCount(); ++i)
    {
        if (harmonicsSelectors[i]->currentText() != "N/A")
            selectedHarmonics[harmonicsSelectors[i]->currentText().toInt()] =
                    true;
    }
}
