#ifndef DSMAINWINDOW_HPP
#define DSMAINWINDOW_HPP

#include <memory>
#include <QMainWindow>

#include "dswindowmanager.hpp"
#include "dsequationhelpdialog.hpp"

namespace Ui {
class DSMainWindow;
}

class DSMainWindow : public QMainWindow, public IDSWindow
{
    Q_OBJECT

public:
    explicit DSMainWindow(DSWindowManager* manager, QWidget *parent = 0);
    virtual ~DSMainWindow();

    virtual void showWindow();

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

    void goToPrevLayer();
    void goToNextLayer();
    void goToFirstLayer();
    void goToLastLayer();
    void goToLayer(const QString& newLayer);
    void changeLayerStep(const QString& newLayerStep);

    void startFiniteRun();
    void startStabilityRun();
    void continueRun();
    void showLastRunResults();

    void showSelectedLayer();
    void displayRunResults();

    void saveActivatorPlot();
    void saveInhibitorPlot();

    void loadEquationsParameters();
    void saveEquationsParameters();

    void updateModelResult(const SchemeSolverResult&);

    void modelSolverError(const DSSolverException&);

private:
    static int maxPlotPointsNumber() { return 400; }
    static double minPlotYRange() { return 1e-4; }
    static double plotRelativeYMargin() { return 0.05; }

    void initPlots();
    void resetPlotsScale(double activatorMin, double activatorMax,
                         double inhibitorMin, double inhibitorMax);
    void displayActivatorLayer(const SchemeLayer& layer);
    void displayInhibitorLayer(const SchemeLayer& layer);

    void showWarningMessages();

    Ui::DSMainWindow *ui;
    std::unique_ptr<DSEquationHelpDialog> equationHelpDialog;
};

#endif // DSMAINWINDOW_HPP
