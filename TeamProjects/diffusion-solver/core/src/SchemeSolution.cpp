#include <cmath>
#include <cassert>
#include <stdexcept>
#include "CoreUtils.hpp"
#include "SchemeSolution.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;


SchemeSolution::SchemeSolution() {
   mSolutionSize = 0;
}

SchemeSolution::SchemeSolution(const SchemeTask& task, 
                               SharedVector solutionSource, 
                               int layersCount, 
                               double minValue, 
                               double maxValue) {
   assert(layersCount > 0);

   mMinValue = minValue;
   mMaxValue = maxValue;

   mSolution = solutionSource;
   mLayersCount = layersCount;
   mTimeStep = task.GetStepTime();
   mIntervalsCount = task.GetIntervalsCount();
   mSolutionSize = (mIntervalsCount + 1) * mLayersCount;
}

SchemeSolution::~SchemeSolution() { }


SchemeLayer SchemeSolution::GetLayer(int index) const {
   assert(index < mLayersCount);
   assert(index >= 0);

   CheckIsInitialized();
   int pointsCount = mIntervalsCount + 1;
   double* layer = mSolution.get() + pointsCount * index;
   return SchemeLayer(layer, pointsCount);
}

SchemeLayer SchemeSolution::GetLayerByTime(double t) const {
   CheckIsInitialized();
   int index = TimeToIndex(t);
   return GetLayer(index);
}

SchemeLayer SchemeSolution::GetLastLayer() const {
   CheckIsInitialized();
   return GetLayer(mLayersCount - 1);
}

SchemeLayer SchemeSolution::operator[] (int index) const {
   return GetLayer(index);
}

double SchemeSolution::GetMaximum() const {
   CheckIsInitialized();
   return mMaxValue;
}

double SchemeSolution::GetMinimum() const {
   CheckIsInitialized();
   return mMinValue;
}

int SchemeSolution::GetLayersCount() const {
   return mLayersCount;
}

int SchemeSolution::GetIntervalsCount() const {
   return mIntervalsCount;
}


int SchemeSolution::TimeToIndex(double t) const {
   int index = static_cast<int>(std::round(t / mTimeStep));
   if (index >= mLayersCount)
      index = mLayersCount - 1;

   return index;
}

void SchemeSolution::CheckIsInitialized() const {
   if (mSolutionSize == 0)
      throw std::runtime_error("Solution is not initialized");
}
