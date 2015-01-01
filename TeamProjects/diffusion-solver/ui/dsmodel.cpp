#include "dsmodel.hpp"

#include <functional>
#include <iostream>

using namespace std::placeholders;

DSModel::DSModel() :
    QObject(),
    lambda1(1.0),
    lambda2(1.0),
    k(1.0),
    c(1.0),
    rho(1.0),
    gamma(1.0),
    nu(1.0),
    timeStep(0.00001),
    activatorAccuracy(0.001),
    inhibitorAccuracy(0.001),
    gridDimension(100),
    iterationsLimit(1000),
    activatorInitConditionsCoeffs(),
    inhibitorInitConditionsCoeffs(),
    solverType(SolverType::EXPLICIT_SOLVER),
    task(new SchemeTask),
    currentLayerIndex(0),
    layerStep(1)
{
    selectExplicitSolver();
}



/*
 * Properties implementation
 */

double DSModel::GetLambda1() const
{
    return lambda1;
}

void DSModel::SetLambda1(double value)
{
    lambda1 = value;
}

double DSModel::GetLambda2() const
{
    return lambda2;
}

void DSModel::SetLambda2(double value)
{
    lambda2 = value;
}

double DSModel::GetK() const
{
    return k;
}

void DSModel::SetK(double value)
{
    k = value;
}

double DSModel::GetC() const
{
    return c;
}

void DSModel::SetC(double value)
{
    c = value;
}

double DSModel::GetRho() const
{
    return rho;
}

void DSModel::SetRho(double value)
{
    rho = value;
}

double DSModel::GetGamma() const
{
    return gamma;
}

void DSModel::SetGamma(double value)
{
    gamma = value;
}

double DSModel::GetNu() const
{
    return nu;
}

void DSModel::SetNu(double value)
{
    nu = value;
}

double DSModel::GetTimeStep() const
{
    return timeStep;
}

void DSModel::SetTimeStep(double value)
{
    timeStep = value;
}

double DSModel::GetActivatorAccuracy() const
{
    return activatorAccuracy;
}

void DSModel::SetActivatorAccuracy(double value)
{
    activatorAccuracy = value;
}

double DSModel::GetInhibitorAccuracy() const
{
    return inhibitorAccuracy;
}

void DSModel::SetInhibitorAccuracy(double value)
{
    inhibitorAccuracy = value;
}

int DSModel::GetGridDimension() const
{
    return gridDimension;
}

void DSModel::SetGridDimension(int value)
{
    gridDimension = value;
}

int DSModel::GetIterationsLimit() const
{
    return iterationsLimit;
}

void DSModel::SetIterationsLimit(int value)
{
    iterationsLimit = value;
}

int DSModel::GetCurrentLayerIndex() const
{
    return currentLayerIndex;
}

void DSModel::SetCurrentLayerIndex(int value)
{
    if (value < 0)
        currentLayerIndex = 0;
    else if (value >= GetLayerCount())
        currentLayerIndex = GetLayerCount() - 1;
    else
        currentLayerIndex = value;
    emit layerIndexChanged();
}

int DSModel::GetLayerStep() const
{
    return layerStep;
}

void DSModel::SetLayerStep(int value)
{
    layerStep = value;
}


/*
 * Other methods implementation
 */
vector<double> DSModel::GetActivatorInitialConditions() const
{
    return activatorInitConditionsCoeffs;
}

void DSModel::SetActivatorInitialConditions(vector<double> value)
{
    activatorInitConditionsCoeffs = value;
}

vector<double> DSModel::GetInhibitorInitialConditions() const
{
    return inhibitorInitConditionsCoeffs;
}

void DSModel::SetInhibitorInitialConditions(vector<double> value)
{
    inhibitorInitConditionsCoeffs = value;
}

void DSModel::StartRun(SchemeSolvingMode mode)
{
    task->SetLambda1(GetLambda1());
    task->SetLambda2(GetLambda2());
    task->SetK(GetK());
    task->SetC(GetC());
    task->SetRho(GetRho());
    task->SetMu(GetGamma());
    task->SetNu(GetNu());
    task->SetStepTime(GetTimeStep());
    task->SetAccuracyU1(GetActivatorAccuracy());
    task->SetAccuracyU2(GetInhibitorAccuracy());
    task->SetMaximumIterations(GetIterationsLimit());

    SchemeLayerGeneratorInitial initialLayerGenerator;
    initialLayerGenerator.SetIntervalsCount(GetGridDimension());

    initialLayerGenerator.SetCoefficients(GetActivatorInitialConditions());
    SchemeLayer activatorInitLayer = initialLayerGenerator.Generate();

    initialLayerGenerator.SetCoefficients(GetInhibitorInitialConditions());
    SchemeLayer inhibitorInitLayer = initialLayerGenerator.Generate();

    task->SetInitialLayers(activatorInitLayer, inhibitorInitLayer);

    switch (mode)
    {
    case AllLayers:
        solver->SetSolvingMode(AllLayers);
        break;
    case StableLayer:
        solver->SetSolvingMode(StableLayer);
    }

    currentLayerIndex = 0;
    solverThread->start();
}

const SchemeLayer DSModel::GetCurrentActivatorLayer()
{
    int layerIndex = GetCurrentLayerIndex();
    auto solutionActivator = result->GetSolutionU1();
    return solutionActivator.GetLayer(layerIndex);
}

const SchemeLayer DSModel::GetCurrentInhibitorLayer()
{
    int layerIndex = GetCurrentLayerIndex();
    auto solutionInhibitor = result->GetSolutionU2();
    return solutionInhibitor.GetLayer(layerIndex);
}

double DSModel::GetActivatorMaximum() const
{
    auto solutionActivator = result->GetSolutionU1();
    return solutionActivator.GetMaximum();
}

double DSModel::GetActivatorMinimum() const
{
    auto solutionActivator = result->GetSolutionU1();
    return solutionActivator.GetMinimum();
}

double DSModel::GetInhibitorMaximum() const
{
    auto solutionInhibitor = result->GetSolutionU2();
    return solutionInhibitor.GetMaximum();
}

double DSModel::GetInhibitorMinimum() const
{
    auto solutionInhibitor = result->GetSolutionU2();
    return solutionInhibitor.GetMinimum();
}

int DSModel::GetLayerCount() const
{
    return result->GetLayersCount();
}


void DSModel::UpdateSolver(SchemeSolver* slvr)
{
    slvr->RegisterTask(task);
    solver.reset(slvr);
    solverThread.reset(new DSSolverThread(solver));

    connect(solverThread.get(), SIGNAL(solverFinished(SchemeSolverResult&)),
            this, SLOT(solverThreadFinished(SchemeSolverResult&)));

    connect(solverThread.get(), SIGNAL(iterationDone(DSSolverIterationInfo&)),
            this, SLOT(solverThreadIterationDone(DSSolverIterationInfo&)));
}

/*
 * Slots implementation
 */
void DSModel::stopSolver()
{
    solverThread->StopSolver();
}

void DSModel::selectImplicitSolver()
{
    UpdateSolver(new SchemeSolverImplicit());
}

void DSModel::selectExplicitSolver()
{
    UpdateSolver(new SchemeSolverExplicit());
}


void DSModel::solverThreadFinished(SchemeSolverResult& res)
{
    result.reset(new SchemeSolverResult(res));
    emit resultAcquired();
}

void DSModel::solverThreadIterationDone(DSSolverIterationInfo& info)
{
    emit iterationDone(info);
}

