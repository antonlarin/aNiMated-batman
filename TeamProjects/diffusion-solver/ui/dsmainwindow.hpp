#ifndef DSMAINWINDOW_HPP
#define DSMAINWINDOW_HPP

#include <QMainWindow>

#include "dsmodel.hpp"

namespace Ui {
class DSMainWindow;
}

class DSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DSMainWindow(QWidget *parent = 0);
    ~DSMainWindow();

    void setModel(DSModel *newModel);

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

    void finiteRunStart();

private:
    Ui::DSMainWindow *ui;
    DSModel* model;
};

#endif // DSMAINWINDOW_HPP
