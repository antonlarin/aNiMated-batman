#include <memory>
#include <functional>
#include "dssolverthread.hpp"

#define SLEEP_INTERVAL_MS 1

using namespace std::placeholders;

DSSolverThread::DSSolverThread(std::shared_ptr<SchemeSolver> solver):
    solver(solver),
    solverNeedStop(false)
{
    std::call_once(registerMetaTypeFlag, []() -> void {
        qRegisterMetaType<SchemeSolverResult>();
        qRegisterMetaType<SchemeSolverResult>("SchemeSolverResult&");
        qRegisterMetaType<DSSolverIterationInfo>();
        qRegisterMetaType<DSSolverIterationInfo>("DSSolverIterationInfo&");
    });

    std::function<bool(SchemeSolverIterationInfo&)> iterCallback =
            std::bind(&DSSolverThread::AcquireIterationInfo, this, _1);
    solver->RegisterIterationCallback(iterCallback);

    connect(this, SIGNAL(finished()),
            this, SLOT(threadFinished()));
}

DSSolverThread::~DSSolverThread()
{
}


void DSSolverThread::StopSolver()
{
    mtx.lock();
    solverNeedStop = true;
    mtx.unlock();
}

void DSSolverThread::run()
{
    mtx.lock();
    solverNeedStop = false;
    mtx.unlock();

    result = solver->Solve();
}

void DSSolverThread::threadFinished()
{
    emit solverFinished(result);
}

bool DSSolverThread::AcquireIterationInfo(SchemeSolverIterationInfo& info)
{
    emit iterationDone(DSSolverIterationInfo(info));
    QThread::msleep(SLEEP_INTERVAL_MS);

    mtx.lock();
    bool stop = solverNeedStop;
    mtx.unlock();

    // to exit from solver need return false
    return !stop;
}
