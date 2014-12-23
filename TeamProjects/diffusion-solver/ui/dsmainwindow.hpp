#ifndef DSMAINWINDOW_HPP
#define DSMAINWINDOW_HPP

#include <QMainWindow>

#include "dsmodel.hpp"
#include "iobserver.hpp"

namespace Ui {
class DSMainWindow;
}

class DSMainWindow : public QMainWindow, public IObserver
{
    Q_OBJECT

public:
    explicit DSMainWindow(QWidget *parent = 0);
    virtual ~DSMainWindow();

    void setModel(DSModel *newModel);
    virtual void update();

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
    void accuracyChanged(const QString& newAccuracy);
    void iterationsLimitChanged(const QString& newIterationsLimit);

    void goToPrevLayer();
    void goToNextLayer();
    void goToFirstLayer();
    void goToLastLayer();
    void goToLayer(const QString& newLayer);
    void changeLayerStep(const QString& newLayerStep);

    void finiteRunStart();

private:
    static int maxPlotPointsNumber() { return 400; }

    void resetPlots();
    void displayActivatorLayer(const SchemeLayer& layer);
    void displayInhibitorLayer(const SchemeLayer& layer);

    Ui::DSMainWindow *ui;
    DSModel* model;
};

#endif // DSMAINWINDOW_HPP
