#ifndef DSVIEWMODEL_HPP
#define DSVIEWMODEL_HPP

#include <vector>
#include <memory>

#include "CoreGlobal.hpp"
#include "ExplicitSchemeSolver.hpp"
#include "ImplicitSchemeSolver.hpp"
#include "DefaultSchemeInitialConditions.hpp"
#include "iobserver.hpp"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using namespace diffusioncore;

enum class SolverType { EXPLICIT_SOLVER, IMPLICIT_SOLVER };

class DSModel
{
public:
    explicit DSModel();
    void RegisterView(IObserver* view);
    void NotifyViews();

    PROPERTY(double, Lambda1)
    PROPERTY(double, Lambda2)
    PROPERTY(double, K)
    PROPERTY(double, C)
    PROPERTY(double, Rho)
    PROPERTY(double, Gamma)
    PROPERTY(double, Nu)
    PROPERTY(double, TimeStep)
    PROPERTY(double, Accuracy)
    PROPERTY(int, GridDimension)
    PROPERTY(int, IterationsLimit)
    PROPERTY(int, CurrentLayerIndex)
    PROPERTY(int, LayerStep)
    PROPERTY(SolverType, SolverType)

    void SetInitialConditions(vector<double>& U1InitConditions,
                              vector<double>& U2InitConditions);

    void AcquireResult(SchemeResult& newResult);

    void StartFiniteRun();

    const SchemeLayer GetCurrentActivatorLayer();
    const SchemeLayer GetCurrentInhibitorLayer();
    double GetActivatorMaximum() const;
    double GetActivatorMinimum() const;
    double GetInhibitorMaximum() const;
    double GetInhibitorMinimum() const;

    int GetLayerCount() const;

private:
    double lambda1;
    double lambda2;
    double k;
    double c;
    double rho;
    double gamma;
    double nu;
    double timeStep;
    double accuracy;
    int gridDimension;
    int iterationsLimit;
    SolverType solverType;

    unique_ptr<SchemeSolver> solver;
    shared_ptr<DefaultSchemeInitialConditions> iconditions;
    unique_ptr<SchemeResult> result;
    vector<IObserver*> views;

    int currentLayerIndex;
    int layerStep;
};

#endif // DSVIEWMODEL_HPP

