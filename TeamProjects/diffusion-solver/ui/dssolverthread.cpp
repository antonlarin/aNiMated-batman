#include <memory>
#include <functional>
#include "dssolverthread.hpp"

using namespace std::placeholders;

DSSolverThread::DSSolverThread()
{
    Initialize();
}

DSSolverThread::DSSolverThread(std::shared_ptr<SchemeSolver> solver)
{
    Initialize();
    UpdateSolver(solver);
}

DSSolverThread::~DSSolverThread()
{
}

void DSSolverThread::Initialize()
{
    solverNeedStop = false;
    std::call_once(registerMetaTypeFlag, []() -> void
    {
        qRegisterMetaType<SchemeSolverResult>();
        qRegisterMetaType<SchemeSolverResult>("SchemeSolverResult&");

        qRegisterMetaType<DSSolverException>();
        qRegisterMetaType<DSSolverException>("DSSolverException&");
    });

    connect(this, SIGNAL(finished()),
            this, SLOT(threadFinished()));
}

void DSSolverThread::StopSolver()
{
    mtx.lock();
    solverNeedStop = true;
    mtx.unlock();
}

void DSSolverThread::UpdateSolver(std::shared_ptr<SchemeSolver> slvr)
{
    if (isRunning())
        throw std::runtime_error("Solver thread is running");

    solver = slvr;
    auto iterMethod = &DSSolverThread::UpdateCurrentSolverResult;
    auto iterCallback = std::bind(iterMethod, this, _1);
    solver->SetIterationCallback(iterCallback);
}

void DSSolverThread::run()
{
    if (!solver)
        throw std::runtime_error("Solver is not set");

    mtx.lock();
    solverNeedStop = false;
    mtx.unlock();

    updateIterationInfoPoint = high_resolution_clock::now();

    try
    {
        result = solver->Solve();
    }
    catch (std::exception ex)
    {
        DSSolverException qex(ex);
        emit solverError(qex);
    }
}

void DSSolverThread::threadFinished()
{
    emit solverFinished(result);
}

bool DSSolverThread::UpdateCurrentSolverResult(SchemeSolverResult& result)
{
    high_resolution_clock::time_point anotherPoint = high_resolution_clock::now();
    milliseconds sinceLastIterationInfoUpdate =
            duration_cast<milliseconds>(anotherPoint - updateIterationInfoPoint);
    if (sinceLastIterationInfoUpdate > GetMaxUpdateIterationSpan())
    {
        updateIterationInfoPoint = anotherPoint;
        emit resultChanged(result);
    }

    mtx.lock();
    bool stop = solverNeedStop;
    mtx.unlock();

    // to exit from solver need return false
    return !stop;
}

