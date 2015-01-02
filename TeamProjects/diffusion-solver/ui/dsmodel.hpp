#ifndef DSVIEWMODEL_HPP
#define DSVIEWMODEL_HPP

#include <vector>
#include <memory>

#include <QObject>

#include <diffusioncore>
#include "dssolverthread.hpp"
#include "dsparameterset.hpp"

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

    PROPERTY(int, CurrentLayerIndex)
    PROPERTY(int, LayerStep)
    PROPERTY(int, FirstComparedLayerIndex)
    PROPERTY(int, SecondComparedLayerIndex)

    void AcquireResult(SchemeSolverResult& newResult);
    void AcquireIterationInfo(SchemeSolverIterationInfo& info);

    void StartRun(SchemeSolverMode mode);

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


signals:
    void layerIndexChanged();
    void resultAcquired();
    void iterationDone(DSSolverIterationInfo&);
    void comparedLayersChanged();

public slots:
    void stopSolver();
    void selectImplicitSolver();
    void selectExplicitSolver();

private slots:
    void solverThreadFinished(SchemeSolverResult&);
    void solverThreadIterationDone(DSSolverIterationInfo&);

private:
    void UpdateSolver(SchemeSolver* solver);

private:
    DSParameterSet parameters;

    shared_ptr<SchemeTask> task;
    shared_ptr<SchemeSolver> solver;
    unique_ptr<SchemeSolverResult> result;
    unique_ptr<SchemeSolverIterationInfo> iterInfo;
    unique_ptr<DSSolverThread> solverThread;

    int currentLayerIndex;
    int layerStep;

    int firstComparedLayerIndex;
    int secondComparedLayerIndex;
};

#endif // DSVIEWMODEL_HPP

