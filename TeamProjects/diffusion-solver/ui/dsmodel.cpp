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
    solver(new ExplicitSchemeSolver()),
    result(nullptr),
    currentLayerIndex(0),
    layerStep(1)
{}

void DSModel::RegisterView(IObserver *view)
{
    views.push_back(view);
}

//void DSModel::modelChanged()
//{
//    for (IObserver* view : views)
//        view->update();
//}



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
    emit modelChanged();
}

int DSModel::GetLayerStep() const
{
    return layerStep;
}

void DSModel::SetLayerStep(int value)
{
    layerStep = value;
}

SolverType DSModel::GetSolverType() const
{
    return solverType;
}

void DSModel::SetSolverType(SolverType value)
{
    SolverType oldType = solverType;
    solverType = value;
    if (oldType != solverType)
    {
        if (solverType == SolverType::EXPLICIT_SOLVER)
        {
            solver.reset(new ExplicitSchemeSolver());
        }
        else
        {
            solver.reset(new ImplicitSchemeSolver());
        }
    }
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

    vector<double> activatorInitLayer;
    vector<double> inhibitorInitLayer;
    for (int i = 0; i <= gridDimension; ++i)
    {
        double activatorValue = 0.0;
        double inhibitorValue = 0.0;
        double x = i * 1.0 / double(gridDimension);

        for (int j = 0; j < activatorInitConditionsCoeffs.size(); ++j)
        {
            activatorValue += activatorInitConditionsCoeffs[j] *
                    cos(M_PI * j * x);
            activatorValue += activatorInitConditionsCoeffs[j] *
                    cos(M_PI * j * x);
        }

        activatorInitLayer.push_back(activatorValue);
        inhibitorInitLayer.push_back(inhibitorValue);
    }

    SchemeLayer activatorIC(activatorInitLayer);
    SchemeLayer inhibitorIC(activatorInitLayer);

    task->SetInitialLayers(activatorIC, inhibitorIC);

    solver->BindTask(task);
    switch (mode)
    {
    case AllLayers:
        solver->SetSolvingMode(AllLayers);
        break;
    case StableLayer:
        solver->SetSolvingMode(StableLayer);
    }

    currentLayerIndex = 0;

    std::function<void(SchemeResult&)> acquireResult =
            std::bind(&DSModel::AcquireResult, this, _1);
    std::function<void(std::exception&)> exceptionCallback =
            [&](std::exception&) -> void {};
    solver->SolveAsync(acquireResult, exceptionCallback);
}

const SchemeLayer DSModel::GetCurrentActivatorLayer()
{
    return result->GetSolutionU1(GetCurrentLayerIndex());
}

const SchemeLayer DSModel::GetCurrentInhibitorLayer()
{
    return result->GetSolutionU2(GetCurrentLayerIndex());
}

double DSModel::GetActivatorMaximum() const
{
    return result->GetSolutionU1Maximum();
}

double DSModel::GetActivatorMinimum() const
{
    return result->GetSolutionU1Minimum();
}

double DSModel::GetInhibitorMaximum() const
{
    return result->GetSolutionU2Maximum();
}

double DSModel::GetInhibitorMinimum() const
{
    return result->GetSolutionU2Minimum();
}

int DSModel::GetLayerCount() const
{
    return result->GetLayersCount();
}

int DSModel::GetPerformedIterationsCount() const
{
    return /*solver->GetIterationsCount()*/1;
}

void DSModel::AcquireResult(SchemeResult &newResult)
{
    result.reset(new SchemeResult(newResult));
    emit modelChanged();
}
