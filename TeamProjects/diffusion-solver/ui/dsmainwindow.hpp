#ifndef DSMAINWINDOW_HPP
#define DSMAINWINDOW_HPP

#include <QMainWindow>

#include "dswindowmanager.hpp"

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

    void showSelectedLayer();
    void displayRunResults();

    void updateModelResult(const SchemeSolverResult&);

private:
    static int maxPlotPointsNumber() { return 400; }
    static double minPlotYRange() { return 1e-4; }
    static double plotRelativeYMargin() { return 0.05; }

    void initPlots();
    void resetPlotsScale(double activatorMin, double activatorMax,
                         double inhibitorMin, double inhibitorMax);
    void expandPlotsScale(double activatorMin, double activatorMax,
                          double inhibitorMin, double inhibitorMax);
    void displayActivatorLayer(const SchemeLayer& layer);
    void displayInhibitorLayer(const SchemeLayer& layer);

    Ui::DSMainWindow *ui;
    double activatorPlotMargin;
    double inhibitorPlotMargin;
    bool plotsNeedScaleReset;
};

#endif // DSMAINWINDOW_HPP
