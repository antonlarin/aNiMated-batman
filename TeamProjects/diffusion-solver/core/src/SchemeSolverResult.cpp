#include <cassert>
#include <stdexcept>
#include "SchemeSolverResult.hpp"
using namespace diffusioncore;

const double NOT_INITIALIZED = std::numeric_limits<double>::infinity();

SchemeSolverResult::SchemeSolverResult() {
   mIsInitialized = false;
}

SchemeSolverResult::SchemeSolverResult(std::shared_ptr<double> solutionU1,
                                       std::shared_ptr<double> solutionU2,
                                       const SchemeTask& task,
                                       int iterationsCount,
                                       int layersCount) {
   assert(layersCount > 0);
   assert(iterationsCount > 0);

   int intervalsCount = task.GetIntervalsCount();
   double timeStep = task.GetStepTime();

   mSolutionU1 = SchemeSolution(
      solutionU1, 
      intervalsCount, 
      layersCount, 
      timeStep);

   mSolutionU2 = SchemeSolution(
      solutionU2, 
      intervalsCount, 
      layersCount, 
      timeStep);

   mTask = task;
   mLayersCount = layersCount;
   mIterationsCount = iterationsCount;
   
   mIsInitialized = true;
}

SchemeSolverResult::SchemeSolverResult(const SchemeSolution& solutionU1,
                                       const SchemeSolution& solutionU2,
                                       const SchemeTask& task,
                                       int iterationsCount) {
   assert(iterationsCount > 0);
   assert(solutionU1.GetLayersCount() == solutionU2.GetLayersCount());
   assert(solutionU1.GetIntervalsCount() == solutionU2.GetIntervalsCount());

   mTask = task;
   mSolutionU1 = solutionU1;
   mSolutionU2 = solutionU2;
   mIterationsCount = iterationsCount;
   mLayersCount = solutionU1.GetLayersCount();
   
   mIsInitialized = true;
}

SchemeSolverResult::~SchemeSolverResult() { }


int SchemeSolverResult::GetIterationsCount() const {
   CheckIsInitialized();
   return mIterationsCount;
}

int SchemeSolverResult::GetLayersCount() const {
   CheckIsInitialized();
   return mLayersCount;
}


SchemeSolution SchemeSolverResult::GetSolutionU1() const {
   CheckIsInitialized();
   return mSolutionU1;
}

SchemeSolution SchemeSolverResult::GetSolutionU2() const {
   CheckIsInitialized();
   return mSolutionU2;
}

SchemeTask SchemeSolverResult::GetTask() const {
   CheckIsInitialized();
   return mTask;
}


void SchemeSolverResult::CheckIsInitialized() const {
   if (!mIsInitialized)
      throw std::runtime_error("Result is not initialized");
}