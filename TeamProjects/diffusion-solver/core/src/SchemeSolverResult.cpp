#include <cassert>
#include <stdexcept>
#include "SchemeSolverResult.hpp"
using namespace diffusioncore;

SchemeSolverResult::SchemeSolverResult() {
   mIsInitialized = false;
}

SchemeSolverResult::SchemeSolverResult(const SchemeSolution& solutionU1,
                                       const SchemeSolution& solutionU2,
                                       const SchemeStatistic& statistic,
                                       const SchemeTask& task) {
   assert(solutionU1.GetLayersCount() == solutionU2.GetLayersCount());
   assert(solutionU1.GetIntervalsCount() == solutionU2.GetIntervalsCount());

   mTask = task;
   mStatistic = statistic;
   mSolutionU1 = solutionU1;
   mSolutionU2 = solutionU2;

   mLayersCount = solutionU1.GetLayersCount();
   mIntervalsCount = solutionU1.GetIntervalsCount();

   mIsInitialized = true;
}

SchemeSolverResult::~SchemeSolverResult() { }


int SchemeSolverResult::GetLayersCount() const {
   return mLayersCount;
}

int SchemeSolverResult::GetIntervalsCount() const {
   return mIntervalsCount;
}

SchemeSolution SchemeSolverResult::GetSolutionU1() const {
   CheckIsInitialized();
   return mSolutionU1;
}

SchemeSolution SchemeSolverResult::GetSolutionU2() const {
   CheckIsInitialized();
   return mSolutionU2;
}

SchemeStatistic SchemeSolverResult::GetStatistic() const {
   CheckIsInitialized();
   return mStatistic;
}

SchemeTask SchemeSolverResult::GetTask() const {
   CheckIsInitialized();
   return mTask;
}


void SchemeSolverResult::CheckIsInitialized() const {
   if (!mIsInitialized)
      throw std::runtime_error("Result is not initialized");
}
