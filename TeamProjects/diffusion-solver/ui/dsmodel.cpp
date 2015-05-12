#include "dsmodel.hpp"

#include <functional>
#include <iostream>

using namespace std::placeholders;

DSModel::DSModel() :
    QObject(),
    parameters(),
    settingsManager(&parameters),
    task(new SchemeTask),
    continuationAvailable(false),
    currentLayerIndex(0),
    layerStep(1),
    firstComparedLayerIndex(0),
    secondComparedLayerIndex(0),
    lastRunStatistic(),
    resultsStorage(new SchemeResultsStorage())
{
    connect(&solverThread, SIGNAL(solverFinished(SchemeSolverResult&)),
            this, SLOT(SolverThreadFinished(SchemeSolverResult&)));

    connect(&solverThread, SIGNAL(resultChanged(const SchemeSolverIterationInfo&)),
            this, SLOT(SolverThreadResultChanged(const SchemeSolverIterationInfo&)));

    connect(&solverThread, SIGNAL(solverError(const DSSolverException&)),
            this, SLOT(SolverThreadHandleError(const DSSolverException&)));

    SelectExplicitSolver();
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
    emit LayerIndexChanged();
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
    emit ComparedLayersChanged();
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
    emit ComparedLayersChanged();
}



/*
 * Other methods implementation
 */
DSParameterSet* DSModel::AccessParameters()
{
    return &parameters;
}

void DSModel::SetupSolverSettings()
{
    solver->SetSolverMode(AccessParameters()->GetSolvingMode());
    solver->SetSaveLayerStep(AccessParameters()->GetLayerSavingStep());
}

void DSModel::SetupInitialConditions()
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

void DSModel::SetupTask()
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
    SetupTask();
    SetupInitialConditions();
    SetupSolverSettings();

    resultsStorage->Drop();
    solverThread.start();
}

void DSModel::ContinueRun()
{
    SetupTask();

    int lastLayerIndex = resultsStorage->GetLayersCount() - 1;
    SchemeLayer lastActivatorLayer = resultsStorage->GetLayerU1(lastLayerIndex);
    SchemeLayer lastInhibitorLayer = resultsStorage->GetLayerU2(lastLayerIndex);
    task->SetInitialLayers(lastActivatorLayer, lastInhibitorLayer);
    task->SetStartIterationIndex(lastLayerIndex);

    SetupSolverSettings();

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
    return lastRunStatistic.GetPerformedIterationsCount();
}

double DSModel::GetAchievedActivatorAccuracy() const
{
    return lastRunStatistic.GetStopAccuracyU1();
}

double DSModel::GetAchievedInhibitorAccuracy() const
{
    return lastRunStatistic.GetStopAccuracyU2();
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
void DSModel::StopSolver()
{
    solverThread.StopSolver();
}

void DSModel::SelectImplicitSolver()
{
    UpdateSolver(new SchemeSolverImplicit());
}

void DSModel::SelectExplicitSolver()
{
    UpdateSolver(new SchemeSolverExplicit());
}


void DSModel::SolverThreadFinished(const SchemeSolverResult& res)
{
    resultsStorage->AddResult(res);
    lastRunStatistic = res.GetStatistic();
    continuationAvailable = true;
    if(!solverThread.IsFinishedWithError())
        emit ResultAcquired();
}

void DSModel::SolverThreadResultChanged(const SchemeSolverIterationInfo& res)
{
    emit ResultChanged(res);
}

void DSModel::SolverThreadHandleError(const DSSolverException& ex)
{
    emit SolverError(ex);
}
DSSettingsManager* DSModel::AccessSettingsManager()
{
    return &settingsManager;
}
