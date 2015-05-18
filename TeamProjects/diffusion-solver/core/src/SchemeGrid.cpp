#include <limits>
#include <cassert>
#include <cstring>
#include <algorithm>
#include "CoreUtils.hpp"
#include "SchemeGrid.hpp"

using namespace diffusioncore;

SchemeGrid::SchemeGrid(const SchemeTask& task,
                       const SchemeLayer& initialLayer,
                       const SchemeSolver* solver) {
   mSwapCounter    = 0;
   mSaveLayerStep  = solver->GetSaveLayerStep();
   mSolverMode     = solver->GetSolverMode();
   mPointsCount    = initialLayer.GetLength();
   mLayersMaxCount = CalculateLayersCount(task);
   mRawGridSize    = mLayersMaxCount * mPointsCount;

   mMaxValue = -std::numeric_limits<double>::infinity();
   mMinValue = std::numeric_limits<double>::infinity();

   InitializeGrid(initialLayer);
}

SchemeGrid::~SchemeGrid() { }


void SchemeGrid::UpdateMinMaxValues(double value) {
   mMaxValue = std::max(mMaxValue, value);
   mMinValue = std::min(mMinValue, value);
}

double* SchemeGrid::GetPrevousLayer() const {
   return mPrevLayer.get();
}

double* SchemeGrid::GetCurrentLayer() const {
   return mCurrLayer.get();
}

double SchemeGrid::GetMinValue() const {
   return mMinValue;
}

double SchemeGrid::GetMaxValue() const {
   return mMaxValue;
}

void SchemeGrid::NextLayer() {
   if (mSolverMode == SchemeSolverMode::AllLayers) {
      if (mSwapCounter % mSaveLayerStep == 0)
         SaveCurrentLayer();
   }

   std::swap(mCurrLayer, mPrevLayer);
   mSwapCounter++;
}

SchemeSolution SchemeGrid::BuildSolution() {
   SchemeSolution solution;
   if (mSolverMode == SchemeSolverMode::StableLayer) {
      SchemeLayer layer(mCurrLayer.get(), mPointsCount);
      solution = SchemeSolution(layer, mMinValue, mMaxValue);
   }
   else if (mSolverMode == SchemeSolverMode::AllLayers) {
      solution = SchemeSolution(mLayers, mMinValue, mMaxValue);
      mLayers.clear();
   }
   return solution;
}

int SchemeGrid::CalculateLayersCount(const SchemeTask& task) const {
   int iterationsCount = task.GetIterationsCount();
   return (iterationsCount + mSaveLayerStep - 1) / mSaveLayerStep + 1;
}

void SchemeGrid::InitializeGrid(const SchemeLayer& initialLayer) {
   mPrevLayer.reset(new double[mPointsCount]);
   mCurrLayer.reset(new double[mPointsCount]);

   size_t rawLayerSize = sizeof(double) * mPointsCount;
   std::memcpy(mPrevLayer.get(), initialLayer.Raw(), rawLayerSize);
}

void SchemeGrid::SaveCurrentLayer() {
   assert(mSolverMode == SchemeSolverMode::AllLayers);
   mLayers.push_back(SchemeLayer(mCurrLayer.get(), mPointsCount));
}
