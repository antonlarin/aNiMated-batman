#include <cassert>
#include <stdexcept>
#include "SchemeSolver.hpp"
using namespace diffusioncore;

SchemeSolver::SchemeSolver() {
   mIsSolving = false;
   mIsStop = true;

   mSolvingMode = SchemeSolvingMode::StableLayer;
   mTask = std::make_shared<SchemeTask>();
}

SchemeSolver::~SchemeSolver() {
   SolveCancel();
}


void SchemeSolver::SetSolvingMode(SchemeSolvingMode value) {
   CheckSolverThreadStatus();
   mSolvingMode = value;
}
SchemeSolvingMode SchemeSolver::GetSolvingMode() const {
   return mSolvingMode;
}


bool SchemeSolver::SolveIsInProgress   () {
   mSolverMutex.lock();
   bool isSolving = mIsSolving;
   mSolverMutex.unlock();
   return isSolving;
}


void SchemeSolver::BindTask(std::shared_ptr<SchemeTask> task) {
   CheckSolverThreadStatus();
   mTask = task;
}

void SchemeSolver::SolveCancel() {
   mSolverMutex.lock();
   mIsStop = true;
   mSolverMutex.unlock();
   SolveWait();
}

void SchemeSolver::SolveAsync(SolverCallback callback, 
                              ExceptionCallback exCallback) {  
   CheckSolverThreadStatus();
   if (mSolverThread.joinable())
      mSolverThread.detach();

   auto task = mTask->Clone();
   CheckParameters(task);

   mIsSolving = true;
   mIsStop = false;
   mSolverThread = std::thread(
      &SchemeSolver::SolveNewThread,
      this, callback, exCallback, task);
}

void SchemeSolver::SolveWait() {
   if (mSolverThread.joinable())
      mSolverThread.join();
}


bool SchemeSolver::IsStoped() {
   mSolverMutex.lock();
   bool isStoped = mIsStop;
   mSolverMutex.unlock(); 
   return isStoped;
}

void SchemeSolver::CheckParametersOverride(SchemeTask task) { }


void SchemeSolver::SolveNewThread(SolverCallback callback,
                                  ExceptionCallback exCallback,
                                  SchemeTask task) {
   bool isSolved = true;
   SchemeSolverResult res;
   try {
      res = SolveOverride(task);    
   }
   catch (std::exception ex) {
      exCallback(ex);
      isSolved = false;
   }

   if (isSolved)
      callback(res);

   mSolverMutex.lock();
   mIsSolving = false;
   mSolverMutex.unlock();
}

void SchemeSolver::CheckParameters(SchemeTask task) {
   CheckParametersOverride(task);
}

void SchemeSolver::CheckSolverThreadStatus() {
   mSolverMutex.lock();
   bool isNotFinished = mIsSolving;
   mSolverMutex.unlock();

   if (isNotFinished)
      throw std::runtime_error("Solving in progress");
}
