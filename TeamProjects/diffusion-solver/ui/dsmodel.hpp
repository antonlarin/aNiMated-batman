#ifndef DSVIEWMODEL_HPP
#define DSVIEWMODEL_HPP

#include <vector>
#include <memory>

#include <QObject>

#include <diffusioncore>
#include "SchemeResultsStorage.hpp"
#include "dssolverthread.hpp"
#include "dsparameterset.hpp"
#include "dssettingsmanager.hpp"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using namespace diffusioncore;

class DSModel : public QObject
{
    Q_OBJECT

public:
    explicit DSModel();

    DSParameterSet* AccessParameters();
    DSSettingsManager* AccessSettingsManager();

    PROPERTY(int, CurrentLayerIndex)
    PROPERTY(int, LayerStep)
    PROPERTY(int, FirstComparedLayerIndex)
    PROPERTY(int, SecondComparedLayerIndex)

    void AcquireResult(SchemeSolverResult& newResult);

    void StartRun();
    void ContinueRun();

    int GetAvailableLayerCount() const;
    double GetCurrentLayerTime() const;
    double GetLastLayerTime() const;
    const SchemeLayer GetActivatorLayer(int index);
    const SchemeLayer GetInhibitorLayer(int index);

    const SchemeLayer GetCurrentActivatorLayer();
    const SchemeLayer GetCurrentInhibitorLayer();
    double GetActivatorMaximum() const;
    double GetActivatorMinimum() const;
    double GetInhibitorMaximum() const;
    double GetInhibitorMinimum() const;

    int GetLayerCount() const;
    int GetPerformedIterationsCount() const;
    double GetAchievedActivatorAccuracy() const;
    double GetAchievedInhibitorAccuracy() const;

    bool IsContinuationAvailable() const;
signals:
    void LayerIndexChanged();
    void ResultAcquired();
    void ResultChanged(const SchemeSolverIterationInfo&);
    void ComparedLayersChanged();
    void SolverError(const DSSolverException&);

public slots:
    void StopSolver();
    void SelectImplicitSolver();
    void SelectExplicitSolver();

private slots:
    void SolverThreadFinished(const SchemeSolverResult&);
    void SolverThreadResultChanged(const SchemeSolverIterationInfo&);
    void SolverThreadHandleError(const DSSolverException&);

private:
    void UpdateSolver(SchemeSolver* solver);    
    void SetupTask();
    void SetupSolverSettings();
    void SetupInitialConditions();

private:
    DSParameterSet parameters;
    DSSettingsManager settingsManager;
    DSSolverThread solverThread;

    shared_ptr<SchemeTask> task;
    shared_ptr<SchemeSolver> solver;
    unique_ptr<SchemeSolverResult> result;
    unique_ptr<SchemeResultsStorage> resultsStorage;

    bool continuationAvailable;
    int currentLayerIndex;
    int layerStep;

    int firstComparedLayerIndex;
    int secondComparedLayerIndex;
};

#endif // DSVIEWMODEL_HPP

