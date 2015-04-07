#include "dsmodel.hpp"

#include <functional>
#include <iostream>

using namespace std::placeholders;

DSModel::DSModel() :
    QObject(),
    parameters(),
    task(new SchemeTask),
    continuationAvailable(false),
    currentLayerIndex(0),
    layerStep(1),
    firstComparedLayerIndex(0),
    secondComparedLayerIndex(0),
    resultsStorage(new SchemeResultsStorage())
{
    connect(&solverThread, SIGNAL(solverFinished(SchemeSolverResult&)),
            this, SLOT(solverThreadFinished(SchemeSolverResult&)));

    connect(&solverThread, SIGNAL(resultChanged(const SchemeSolverIterationInfo&)),
            this, SLOT(solverThreadResultChanged(const SchemeSolverIterationInfo&)));

    connect(&solverThread, SIGNAL(solverError(const DSSolverException&)),
            this, SLOT(solverThreadHandleError(const DSSolverException&)));

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

void DSModel::setupSolverSettings()
{
    solver->SetSolverMode(AccessParameters()->GetSolvingMode());
    solver->SetSaveLayerStep(AccessParameters()->GetLayerSavingStep());
}

void DSModel::setupInitialConditions()
{
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
}

void DSModel::setupTask()
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
    task->SetEndIterationIndex(AccessParameters()->GetIterationsLimit());
}

void DSModel::StartRun()
{
    setupTask();
    setupInitialConditions();
    setupSolverSettings();

    resultsStorage->Drop();
    solverThread.start();
}

void DSModel::ContinueRun()
{
    setupTask();

    int lastLayerIndex = resultsStorage->GetLayersCount() - 1;
    SchemeLayer lastActivatorLayer = resultsStorage->GetLayerU1(lastLayerIndex);
    SchemeLayer lastInhibitorLayer = resultsStorage->GetLayerU2(lastLayerIndex);
    task->SetInitialLayers(lastActivatorLayer, lastInhibitorLayer);
    task->SetStartIterationIndex(lastLayerIndex);

    setupSolverSettings();

    solverThread.start();
}

int DSModel::GetAvailableLayerCount() const
{
    return resultsStorage->GetLayersCount();
}

double DSModel::GetCurrentLayerTime() const
{
    return resultsStorage->TimeByIndex(currentLayerIndex);
}

double DSModel::GetLastLayerTime() const
{
    return resultsStorage->TimeByIndex(GetAvailableLayerCount() - 1);
}

const SchemeLayer DSModel::GetActivatorLayer(int index)
{
    return resultsStorage->GetLayerU1(index);
}

const SchemeLayer DSModel::GetInhibitorLayer(int index)
{
    return resultsStorage->GetLayerU2(index);
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
    return resultsStorage->GetU1Maximum();
}

double DSModel::GetActivatorMinimum() const
{
    return resultsStorage->GetU1Minimum();
}

double DSModel::GetInhibitorMaximum() const
{
    return resultsStorage->GetU2Maximum();
}

double DSModel::GetInhibitorMinimum() const
{
    return resultsStorage->GetU2Minimum();
}

int DSModel::GetPerformedIterationsCount() const
{
    SchemeStatistic stat = result->GetStatistic();
    return stat.GetPerformedIterationsCount();
}

double DSModel::GetAchievedActivatorAccuracy() const
{
    SchemeStatistic stat = result->GetStatistic();
    return stat.GetStopAccuracyU1();
}

double DSModel::GetAchievedInhibitorAccuracy() const
{
    SchemeStatistic stat = result->GetStatistic();
    return stat.GetStopAccuracyU2();
}

bool DSModel::IsContinuationAvailable() const
{
    return continuationAvailable;
}

int DSModel::GetLayerCount() const
{
    return resultsStorage->GetLayersCount();
}


void DSModel::UpdateSolver(SchemeSolver* slvr)
{
    solver.reset(slvr);
    solver->SetCurrentTask(task);
    solverThread.UpdateSolver(solver);
}

/*
 * Slots implementation
 */
void DSModel::stopSolver()
{
    solverThread.StopSolver();
}

void DSModel::selectImplicitSolver()
{
    UpdateSolver(new SchemeSolverImplicit());
}

void DSModel::selectExplicitSolver()
{
    UpdateSolver(new SchemeSolverExplicit());
}


void DSModel::solverThreadFinished(const SchemeSolverResult& res)
{
    resultsStorage->AddResult(res);
    result.reset(new SchemeSolverResult(res));
    continuationAvailable = true;
    emit resultAcquired();
}

void DSModel::solverThreadResultChanged(const SchemeSolverIterationInfo& res)
{
    emit resultChanged(res);
}

void DSModel::solverThreadHandleError(const DSSolverException& ex)
{
    emit solverError(ex);
}
