#include "dsinitconditionsdialog.hpp"
#include "ui_dsinitconditionsdialog.h"

DSInitConditionsDialog::DSInitConditionsDialog(DSModel* model,
                                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSInitConditionsDialog),
    model(model),
    selectedHarmonics(20, false),
    activeHarmonicControls(6, false),
    harmonicsSelectors(),
    activatorCoeffEdits(),
    inhibitorCoeffEdits()
{
    ui->setupUi(this);
    selectedHarmonics.front() = true;
    activeHarmonicControls.front() = true;

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
    std::vector<double> activatorCoeffs(20, 0.0);
    std::vector<double> inhibitorCoeffs(20, 0.0);

    for (auto i = selectedHarmonics.begin(); i != selectedHarmonics.end(); ++i)
    {
        int index = (int)(i - selectedHarmonics.begin());
        if (*i)
        {
            activatorCoeffs[index] =
                    activatorCoeffEdits[index]->text().toDouble();
            inhibitorCoeffs[index] =
                    inhibitorCoeffEdits[index]->text().toDouble();
        }
    }
    model->SetInitialConditions(activatorCoeffs, inhibitorCoeffs);
}
