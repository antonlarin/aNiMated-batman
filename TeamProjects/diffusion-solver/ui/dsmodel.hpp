#ifndef DSVIEWMODEL_HPP
#define DSVIEWMODEL_HPP

#include <vector>
#include <memory>

#include <QObject>

#include <diffusioncore>
#include "iobserver.hpp"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using namespace diffusioncore;

enum class SolverType { EXPLICIT_SOLVER, IMPLICIT_SOLVER };

class DSModel : public QObject
{
    Q_OBJECT

public:
    explicit DSModel();

    void RegisterView(IObserver* view);

    PROPERTY(double, Lambda1)
    PROPERTY(double, Lambda2)
    PROPERTY(double, K)
    PROPERTY(double, C)
    PROPERTY(double, Rho)
    PROPERTY(double, Gamma)
    PROPERTY(double, Nu)
    PROPERTY(double, TimeStep)
    PROPERTY(double, ActivatorAccuracy)
    PROPERTY(double, InhibitorAccuracy)
    PROPERTY(int, GridDimension)
    PROPERTY(int, IterationsLimit)
    PROPERTY(int, CurrentLayerIndex)
    PROPERTY(int, LayerStep)
    PROPERTY(vector<double>, ActivatorInitialConditions)
    PROPERTY(vector<double>, InhibitorInitialConditions)
    PROPERTY(SolverType, SolverType)

    void AcquireResult(SchemeSolverResult& newResult);

    void StartRun(SchemeSolvingMode mode);

    const SchemeLayer GetCurrentActivatorLayer();
    const SchemeLayer GetCurrentInhibitorLayer();
    double GetActivatorMaximum() const;
    double GetActivatorMinimum() const;
    double GetInhibitorMaximum() const;
    double GetInhibitorMinimum() const;

    int GetLayerCount() const;
    int GetPerformedIterationsCount() const;

signals:
    void modelChanged();

private:
    double lambda1;
    double lambda2;
    double k;
    double c;
    double rho;
    double gamma;
    double nu;
    double timeStep;
    double activatorAccuracy;
    double inhibitorAccuracy;
    int gridDimension;
    int iterationsLimit;
    vector<double> activatorInitConditionsCoeffs;
    vector<double> inhibitorInitConditionsCoeffs;
    SolverType solverType;

    shared_ptr<SchemeTask> task;
    unique_ptr<SchemeSolver> solver;
    unique_ptr<SchemeSolverResult> result;

    vector<IObserver*> views;

    int currentLayerIndex;
    int layerStep;
};

#endif // DSVIEWMODEL_HPP

