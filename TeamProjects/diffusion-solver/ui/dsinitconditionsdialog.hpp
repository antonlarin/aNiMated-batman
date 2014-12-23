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

private:
    Ui::DSInitConditionsDialog *ui;

    DSModel* model;

    vector<bool> selectedHarmonics;
    vector<bool> activeHarmonicControls;
    vector<QComboBox*> harmonicsSelectors;
    vector<QLineEdit*> activatorCoeffEdits;
    vector<QLineEdit*> inhibitorCoeffEdits;
};

#endif // DSINITCONDITIONSDIALOG_H
