#ifndef DSMAINWINDOW_HPP
#define DSMAINWINDOW_HPP

#include <QMainWindow>

#include "dsmodel.hpp"
#include "dsinitconditionsdialog.hpp"
#include "dssolvingprogressdialog.hpp"

namespace Ui {
class DSMainWindow;
}

class DSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DSMainWindow(DSModel* newModel, QWidget *parent = 0);
    virtual ~DSMainWindow();

public slots:
    void lambda1CoeffChanged(const QString& newLambda1);
    void lambda2CoeffChanged(const QString& newLambda2);
    void kCoeffChanged(const QString& newK);
    void cCoeffChanged(const QString& newC);
    void rhoCoeffChanged(const QString& newRho);
    void nuCoeffChanged(const QString& newNu);
    void gammaCoeffChanged(const QString& newGamma);
    void gridDimensionChanged(const QString& newGridDimension);
    void timeStepChanged(const QString& newTimeStep);
    void activatorAccuracyChanged(const QString& newAccuracy);
    void inhibitorAccuracyChanged(const QString& newAccuracy);
    void iterationsLimitChanged(const QString& newIterationsLimit);
    void changeSolverType();

    void goToPrevLayer();
    void goToNextLayer();
    void goToFirstLayer();
    void goToLastLayer();
    void goToLayer(const QString& newLayer);
    void changeLayerStep(const QString& newLayerStep);

    void startFiniteRun();
    void startStabilityRun();

    void openInitConditionsDialog();

    void updateDisplayedLayer();
    void displayRunResults();

private:
    static int maxPlotPointsNumber() { return 400; }

    void initPlots();
    void resetPlotsScale(double activatorMin, double activatorMax,
                    double inhibitorMin, double inhibitorMax);
    void displayActivatorLayer(const SchemeLayer& layer);
    void displayInhibitorLayer(const SchemeLayer& layer);

    void showSolvingProgressDialog();

    Ui::DSMainWindow *ui;
    DSInitConditionsDialog* initConditionsDialog;
    DSSolvingProgressDialog* solvingProgressDialog;
    DSModel* model;
};

#endif // DSMAINWINDOW_HPP
