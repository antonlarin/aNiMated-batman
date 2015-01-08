#include <memory>
#include <functional>
#include "dssolverthread.hpp"

using namespace std::placeholders;

DSSolverThread::DSSolverThread(std::shared_ptr<SchemeSolver> solver):
    solverNeedStop(false),
    solver(solver)
{
    std::call_once(registerMetaTypeFlag, []() -> void {
        qRegisterMetaType<SchemeSolverResult>();
        qRegisterMetaType<SchemeSolverResult>("SchemeSolverResult&");
        qRegisterMetaType<DSSolverIterationInfo>();
        qRegisterMetaType<DSSolverIterationInfo>("DSSolverIterationInfo&");
        qRegisterMetaType<SchemeLayer>();
        qRegisterMetaType<SchemeLayer>("SchemeLayer&");
    });

    auto iterMethod = &DSSolverThread::AcquireIterationInfo;
    auto iterCallback = std::bind(iterMethod, this, _1);
    solver->RegisterIterationCallback(iterCallback);

    auto layersMethod = &DSSolverThread::AcquireCurrentLayers;
    auto layersCallback = std::bind(layersMethod, this, _1, _2);
    solver->RegisterLayerChangedCallback(layersCallback);

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

    updateIterationInfoPoint = high_resolution_clock::now();
    result = solver->Solve();
}

void DSSolverThread::threadFinished()
{
    emit solverFinished(result);
}

bool DSSolverThread::AcquireIterationInfo(SchemeSolverIterationInfo& info)
{
    high_resolution_clock::time_point anotherPoint = high_resolution_clock::now();
    milliseconds sinceLastIterationInfoUpdate =
            duration_cast<milliseconds>(anotherPoint - updateIterationInfoPoint);
    if (sinceLastIterationInfoUpdate > GetMaxUpdateIterationSpan())
    {
        updateIterationInfoPoint = anotherPoint;
        emit iterationDone(DSSolverIterationInfo(info));
    }

    mtx.lock();
    bool stop = solverNeedStop;
    mtx.unlock();

    // to exit from solver need return false
    return !stop;
}

void DSSolverThread::AcquireCurrentLayers(SchemeLayer& u1, SchemeLayer& u2)
{
    high_resolution_clock::time_point anotherPoint = high_resolution_clock::now();
    milliseconds sinceLastIterationInfoUpdate =
            duration_cast<milliseconds>(anotherPoint - updateIterationInfoPoint);
    if (sinceLastIterationInfoUpdate > GetMaxUpdateIterationSpan())
    {
        updateIterationInfoPoint = anotherPoint;
        emit layersChanged(u1, u2);
    }
}
