#ifndef DSINITCONDITIONSDIALOG_H
#define DSINITCONDITIONSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>

#include <vector>

#include "dswindowmanager.hpp"

using std::vector;

namespace Ui {
class DSInitConditionsDialog;
}

class DSInitConditionsDialog : public QDialog, public IDSWindow
{
    Q_OBJECT

public:
    explicit DSInitConditionsDialog(DSWindowManager* manager,
                                    QWidget *parent = 0);
    virtual ~DSInitConditionsDialog();

    virtual void showWindow();

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
    static int maxHarmonicOrder() { return 20; }
    static int maxHarmonicsCount() { return 6; }

    vector<double> getCurrentActivatorCoeffs();
    vector<double> getCurrentInhibitorCoeffs();

    void initPlots();
    void populateSelectorItemLists();

    void addHarmonic(int index);
    void removeHarmonic(int index);

    void updateSelectedHarmonics();
    void displayCurrentInitialConditions();
    void displayPlotsOfCurrentInitialConditions();

    Ui::DSInitConditionsDialog *ui;

    vector<bool> selectedHarmonics;
    vector<bool> activeHarmonicControls;
    vector<QComboBox*> harmonicsSelectors;
    vector<QLineEdit*> activatorCoeffEdits;
    vector<QLineEdit*> inhibitorCoeffEdits;
};

#endif // DSINITCONDITIONSDIALOG_H
