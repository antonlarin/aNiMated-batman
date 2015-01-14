#include <limits>
#include <memory>
#include <cassert>
#include <algorithm>
#include "CoreUtils.hpp"
#include "SchemeSolutionBuilder.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolutionBuilder::SchemeSolutionBuilder() {
   mIsInitialized = false;
   ResetMinMaxValues();
}

SchemeSolutionBuilder::SchemeSolutionBuilder(const SchemeTask& task, 
                                             SchemeSolverMode solverMode) {
   mTask = task;
   mSolverMode = solverMode;
   mIsInitialized = true;
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
   CheckIsInitialized();
   mIterationsCount = itersCount;
}


SchemeSolution SchemeSolutionBuilder::Build(const SchemeGrid& grid) const {
   CheckIsInitialized();
   
   int layersCount;
   SharedVector solution;

   switch (mSolverMode) {
      case SchemeSolverMode::AllLayers:
         layersCount = mIterationsCount + 1;
         solution = grid.Source();
         break;

      case SchemeSolverMode::StableLayer:
         layersCount = 1;
         solution = CopyShared(grid.GetCurrentLayer(), 
                               mTask.GetIntervalsCount() + 1);
         break;

      default:
         throw std::runtime_error("Invalid solving mode");
   }

   return SchemeSolution(mTask, solution, layersCount, 
                         mCurrentMin, mCurrentMax);
}

void SchemeSolutionBuilder::CheckIsInitialized() const {
   if (!mIsInitialized)
      throw std::runtime_error("Object is not initialized");
}