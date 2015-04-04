#include <cassert>
#include <stdexcept>
#include "SchemeSolverResult.hpp"

using namespace diffusioncore;

SchemeSolverResult::SchemeSolverResult() {
   mLayersCount = 0;
   mIntervalsCount = 0;
}

SchemeSolverResult::SchemeSolverResult(const SchemeSolution& solutionU1,
                                       const SchemeSolution& solutionU2,
                                       const SchemeStatistic& statistic,
                                       const SchemeTask& task) {
   assert(solutionU1.GetLayersCount() == solutionU2.GetLayersCount());
   mTask = task;
   mStatistic = statistic;
   mSolutionU1 = solutionU1;
   mSolutionU2 = solutionU2;

   mLayersCount = solutionU1.GetLayersCount();
   mIntervalsCount = task.GetIntervalsCount();
}

SchemeSolverResult::~SchemeSolverResult() { }


int SchemeSolverResult::GetLayersCount() const {
   return mLayersCount;
}

int SchemeSolverResult::GetIntervalsCount() const {
   return mIntervalsCount;
}

SchemeSolution SchemeSolverResult::GetSolutionU1() const {
   return mSolutionU1;
}

SchemeSolution SchemeSolverResult::GetSolutionU2() const {
   return mSolutionU2;
}

SchemeStatistic SchemeSolverResult::GetStatistic() const {
   return mStatistic;
}

SchemeTask SchemeSolverResult::GetTask() const {
   return mTask;
}