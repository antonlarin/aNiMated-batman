#ifndef DSSOLVERTHREAD_HPP
#define DSSOLVERTHREAD_HPP

#include <mutex>
#include <memory>
#include <functional>
#include <diffusioncore>

#include <QMutex>
#include <QThread>

#include "dssolveriterationinfo.hpp"

using diffusioncore::SchemeSolver;
using diffusioncore::SchemeSolverResult;
using diffusioncore::SchemeSolverIterationInfo;

Q_DECLARE_METATYPE(SchemeLayer)
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
    void layersChanged(SchemeLayer& activator,
                       SchemeLayer& inhibitor);

private slots:
    void threadFinished();

protected:
    void run();

private:
    bool AcquireIterationInfo(SchemeSolverIterationInfo& info);
    void AcquireCurrentLayers(SchemeLayer& u1, SchemeLayer& u2);

    QMutex mtx;
    bool solverNeedStop;
    SchemeSolverResult result;
    std::once_flag registerMetaTypeFlag;
    std::shared_ptr<SchemeSolver> solver;

};

#endif // DSSOLVERTHREAD_HPP
