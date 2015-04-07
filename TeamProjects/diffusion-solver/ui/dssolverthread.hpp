#ifndef DSSOLVERTHREAD_HPP
#define DSSOLVERTHREAD_HPP

#include <mutex>
#include <memory>
#include <functional>
#include <diffusioncore>
#include <chrono>

#include <QMutex>
#include <QThread>

#include "dssolverexception.hpp"

using namespace std::chrono;

using diffusioncore::SchemeSolver;
using diffusioncore::SchemeSolverResult;
using diffusioncore::SchemeSolverIterationInfo;

Q_DECLARE_METATYPE(SchemeSolverIterationInfo)
Q_DECLARE_METATYPE(SchemeSolverResult)
Q_DECLARE_METATYPE(DSSolverException)

class DSSolverThread : public QThread
{
    Q_OBJECT

public:
    DSSolverThread();
    DSSolverThread(std::shared_ptr<SchemeSolver> solver);
    ~DSSolverThread();

    void StopSolver();
    void UpdateSolver(std::shared_ptr<SchemeSolver> solver);
    void SetContinuationFlag(bool isContinuation);

signals:
    void resultChanged(const SchemeSolverIterationInfo&);
    void solverFinished(const SchemeSolverResult&);
    void solverError(const DSSolverException&);

private slots:
    void threadFinished();

protected:
    void run();

private:
    void Initialize();
    bool UpdateCurrentSolverResult(SchemeSolverIterationInfo& info);

    inline milliseconds GetMaxUpdateIterationSpan() const;

    QMutex mtx;
    bool solverNeedStop;
    bool isContinuationRun;
    SchemeSolverResult result;
    std::once_flag registerMetaTypeFlag;
    std::shared_ptr<SchemeSolver> solver;
    high_resolution_clock::time_point updateIterationInfoPoint;
};

#endif // DSSOLVERTHREAD_HPP
