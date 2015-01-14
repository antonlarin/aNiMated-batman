#ifndef DSSOLVERTHREAD_HPP
#define DSSOLVERTHREAD_HPP

#include <mutex>
#include <memory>
#include <functional>
#include <diffusioncore>
#include <chrono>

#include <QMutex>
#include <QThread>

using namespace std::chrono;

using diffusioncore::SchemeSolver;
using diffusioncore::SchemeSolverResult;

Q_DECLARE_METATYPE(SchemeSolverResult)

class DSSolverThread : public QThread
{
    Q_OBJECT

public:
    DSSolverThread(std::shared_ptr<SchemeSolver> solver);
    ~DSSolverThread();

    void StopSolver();

signals:
    void resultChanged(const SchemeSolverResult&);
    void solverFinished(const SchemeSolverResult&);

private slots:
    void threadFinished();

protected:
    void run();

private:
    bool UpdateCurrentSolverResult(SchemeSolverResult& info);

    milliseconds GetMaxUpdateIterationSpan() const { return milliseconds(10); }

    QMutex mtx;
    bool solverNeedStop;
    SchemeSolverResult result;
    std::once_flag registerMetaTypeFlag;
    std::shared_ptr<SchemeSolver> solver;
    high_resolution_clock::time_point updateIterationInfoPoint;
};

#endif // DSSOLVERTHREAD_HPP
