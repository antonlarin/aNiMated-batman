#include "dsmodel.hpp"

#include <functional>
#include <iostream>

using namespace std::placeholders;

DSModel::DSModel() :
    QObject(),
    parameters(),
    task(new SchemeTask),
    currentLayerIndex(0),
    layerStep(1),
    firstComparedLayerIndex(0),
    secondComparedLayerIndex(0)
{
    selectExplicitSolver();
}

/*
 * Properties implementation
 */
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
    if (value > 0)
        layerStep = value;
}

int DSModel::GetFirstComparedLayerIndex() const
{
    return firstComparedLayerIndex;
}

void DSModel::SetFirstComparedLayerIndex(int value)
{
    if (value < 0)
        firstComparedLayerIndex = 0;
    else if (value >= GetLayerCount())
        firstComparedLayerIndex = GetLayerCount() - 1;
    else
        firstComparedLayerIndex = value;
    emit comparedLayersChanged();
}

int DSModel::GetSecondComparedLayerIndex() const
{
    return secondComparedLayerIndex;
}

void DSModel::SetSecondComparedLayerIndex(int value)
{
    if (value < 0)
        secondComparedLayerIndex = 0;
    else if (value >= GetLayerCount())
        secondComparedLayerIndex = GetLayerCount() - 1;
    else
        secondComparedLayerIndex = value;
    emit comparedLayersChanged();
}



/*
 * Other methods implementation
 */
DSParameterSet* DSModel::AccessParameters()
{
    return &parameters;
}

void DSModel::StartRun(SchemeSolverMode mode)
{
    task->SetLambda1(AccessParameters()->GetLambda1());
    task->SetLambda2(AccessParameters()->GetLambda2());
    task->SetK(AccessParameters()->GetK());
    task->SetC(AccessParameters()->GetC());
    task->SetRho(AccessParameters()->GetRho());
    task->SetMu(AccessParameters()->GetGamma());
    task->SetNu(AccessParameters()->GetNu());
    task->SetStepTime(AccessParameters()->GetTimeStep());
    task->SetAccuracyU1(AccessParameters()->GetActivatorAccuracy());
    task->SetAccuracyU2(AccessParameters()->GetInhibitorAccuracy());
    task->SetMaximumIterations(AccessParameters()->GetIterationsLimit());

    SchemeLayerGeneratorInitial initialLayerGenerator;
    initialLayerGenerator.SetIntervalsCount(
                AccessParameters()->GetGridDimension());

    initialLayerGenerator.SetCoefficients(
                AccessParameters()->GetActivatorInitialConditions());
    SchemeLayer activatorInitLayer = initialLayerGenerator.Generate();

    initialLayerGenerator.SetCoefficients(
                AccessParameters()->GetInhibitorInitialConditions());
    SchemeLayer inhibitorInitLayer = initialLayerGenerator.Generate();

    task->SetInitialLayers(activatorInitLayer, inhibitorInitLayer);

    switch (mode)
    {
    case AllLayers:
        solver->SetSolverMode(AllLayers);
        break;
    case StableLayer:
        solver->SetSolverMode(StableLayer);
    }

    currentLayerIndex = 0;
    solverThread->start();
}

const SchemeLayer DSModel::GetActivatorLayer(int index)
{
    SchemeSolution solutionActivator = result->GetSolutionU1();
    return solutionActivator.GetLayer(index);
}

const SchemeLayer DSModel::GetInhibitorLayer(int index)
{
    SchemeSolution solutionInhibitor = result->GetSolutionU2();
    return solutionInhibitor.GetLayer(index);
}

const SchemeLayer DSModel::GetCurrentActivatorLayer()
{
    return GetActivatorLayer(GetCurrentLayerIndex());
}

const SchemeLayer DSModel::GetCurrentInhibitorLayer()
{
    return GetInhibitorLayer(GetCurrentLayerIndex());
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

    connect(solverThread.get(), SIGNAL(layersChanged(SchemeLayer&, SchemeLayer&)),
            this, SLOT(solverThreadLayersChanged(SchemeLayer&, SchemeLayer&)));
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

void DSModel::solverThreadLayersChanged(SchemeLayer& u1, SchemeLayer& u2)
{
    emit currentLayersChanged(u1, u2);
}

