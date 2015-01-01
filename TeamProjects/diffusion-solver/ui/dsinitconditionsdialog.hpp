#ifndef DSINITCONDITIONSDIALOG_H
#define DSINITCONDITIONSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>

#include <vector>

#include "dsmodel.hpp"

using std::vector;

namespace Ui {
class DSInitConditionsDialog;
}

class DSInitConditionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DSInitConditionsDialog(DSModel* model, QWidget *parent = 0);
    ~DSInitConditionsDialog();
public slots:
    void acceptInitialConditions();
    void addHarmonic1();
    void addHarmonic2();
    void addHarmonic3();
    void addHarmonic4();
    void addHarmonic5();
    void removeHarmonic1();
    void removeHarmonic2();
    void removeHarmonic3();
    void removeHarmonic4();
    void removeHarmonic5();
    void activatorHarmonicCoeffChanged(const QString& value);
    void inhibitorHarmonicCoeffChanged(const QString& value);

private:
    vector<double> getCurrentActivatorCoeffs();
    vector<double> getCurrentInhibitorCoeffs();
    void initInitalConditionsPlots();
    void setSelectorItemLists();
    void displayCurrentInitialConditions();
    void displayPlotsOfCurrentInitialConditions();
    void addHarmonic(int index);
    void removeHarmonic(int index);
    void updateSelectedHarmonics();

    Ui::DSInitConditionsDialog *ui;

    DSModel* model;

    vector<bool> selectedHarmonics;
    vector<bool> activeHarmonicControls;
    vector<QComboBox*> harmonicsSelectors;
    vector<QLineEdit*> activatorCoeffEdits;
    vector<QLineEdit*> inhibitorCoeffEdits;
};

#endif // DSINITCONDITIONSDIALOG_H
