#include <limits>
#include <memory>
#include <cassert>
#include <algorithm>
#include "CoreUtils.hpp"
#include "SchemeSolutionBuilder.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolutionBuilder::SchemeSolutionBuilder(const SchemeTask& task, 
                                             SchemeSolverMode solverMode) {
   mTask = task;
   mSolverMode = solverMode;
   ResetMinMaxValues();
}

SchemeSolutionBuilder::~SchemeSolutionBuilder() { }


void SchemeSolutionBuilder::UpdateMinMaxValues(double value) {
   mCurrentMin = std::min(mCurrentMin, value);
   mCurrentMax = std::max(mCurrentMax, value);
}

void SchemeSolutionBuilder::UpdateMinMaxValues(const SchemeLayer& layer) {
   UpdateMinMaxValues(layer.GetMinValue());
   UpdateMinMaxValues(layer.GetMaxValue());
}

void SchemeSolutionBuilder::ResetMinMaxValues() {
   mCurrentMin =  std::numeric_limits<double>::infinity();
   mCurrentMax = -std::numeric_limits<double>::infinity(); 
}

void SchemeSolutionBuilder::SetIterationsCount(int itersCount) {
   assert(itersCount > 0);
   mIterationsCount = itersCount;
}


SchemeSolution SchemeSolutionBuilder::Build(const SchemeGrid& grid) const {
   auto source = grid.Source();
   double k = mTask.GetStepTime();
   int n = mTask.GetIntervalsCount();
   int layersCount = mIterationsCount + 1;
   
   std::shared_ptr<double> solution;
   switch (mSolverMode) {
      case SchemeSolverMode::AllLayers:
         return SchemeSolution(source, n, layersCount, k, 
                               mCurrentMin, mCurrentMax);

      case SchemeSolverMode::StableLayer:
         solution = CopyShared(grid.GetCurrentLayer(), n + 1);
         return SchemeSolution(solution, n, 1, k, 
                               mCurrentMin, mCurrentMax);

      default:
         throw std::runtime_error("Invalid solving mode");
   }
}
