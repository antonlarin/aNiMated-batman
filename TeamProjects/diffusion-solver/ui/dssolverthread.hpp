#ifndef DSSOLVERTHREAD_HPP
#define DSSOLVERTHREAD_HPP

#include <memory>
#include <functional>
#include <diffusioncore>
#include <chrono>

#include <QMutex>
#include <QThread>

#include "dssolveriterationinfo.hpp"

using namespace std::chrono;

using diffusioncore::SchemeSolver;
using diffusioncore::SchemeSolverResult;
using diffusioncore::SchemeSolverIterationInfo;

Q_DECLARE_METATYPE(SchemeSolverResult)
Q_DECLARE_METATYPE(DSSolverIterationInfo)

class DSSolverThread : public QThread
{
    Q_OBJECT

public:
    DSSolverThread(std::shared_ptr<SchemeSolver> solver);
    ~DSSolverThread();

    void StopSolver();

signals:
    void iterationDone(const DSSolverIterationInfo&);
    void solverFinished(SchemeSolverResult&);

private slots:
    void threadFinished();

protected:
    void run();

private:
    bool AcquireIterationInfo(SchemeSolverIterationInfo& info);

    milliseconds GetMaxUpdateIterationSpan() const { return milliseconds(10); }

    QMutex mtx;
    bool solverNeedStop;
    SchemeSolverResult result;
    std::once_flag registerMetaTypeFlag;
    std::shared_ptr<SchemeSolver> solver;
    high_resolution_clock::time_point updateIterationInfoPoint;
};

#endif // DSSOLVERTHREAD_HPP
