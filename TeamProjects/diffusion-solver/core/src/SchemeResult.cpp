#include <cassert>
#include <cmath>
#include "SchemeResult.hpp"
using namespace diffusioncore;

SchemeResult::SchemeResult(std::shared_ptr<double> solutionU1,
                           std::shared_ptr<double> solutionU2,
                           int intervalsCount,
                           int layersCount,
                           double timeStep) {
   assert(intervalsCount > 0);
   assert(layersCount > 0);

   mSolutionU1 = solutionU1;
   mSolutionU2 = solutionU2;

   mTimeStep = timeStep;
   mLayersCount = layersCount;
   mIntervalsCount = intervalsCount;
}

SchemeResult::~SchemeResult() { }


int SchemeResult::GetLayersCount() const{
   return mLayersCount;
}

int SchemeResult::GetIntervalsCount() const {
   return mIntervalsCount;
}


SchemeLayer SchemeResult::GetSolutionU1(int index) {
   int pointsCount = mIntervalsCount + 1;
   double* layer = mSolutionU1.get() + pointsCount * index;
   return SchemeLayer(layer, pointsCount);
}

SchemeLayer SchemeResult::GetSolutionU2(int index) {
   int pointsCount = mIntervalsCount + 1;
   double* layer = mSolutionU2.get() + pointsCount * index;
   return SchemeLayer(layer, pointsCount);
}

SchemeLayer SchemeResult::GetSolutionU1(double t) {
   assert(t >= 0);

   int index = TimeToIndex(t);
   return GetSolutionU1(index);
}

SchemeLayer SchemeResult::GetSolutionU2(double t) {
   assert(t >= 0);

   int index = TimeToIndex(t);
   return GetSolutionU2(index);
}

SchemeLayer SchemeResult::GetLastLayerU1() {
   return GetSolutionU1(mLayersCount - 1);
}

SchemeLayer SchemeResult::GetLastLayerU2() {
   return GetSolutionU2(mLayersCount - 1);
}


double SchemeResult::GetSolutionU1Maximum() {
   // TODO
   return 1;
}

double SchemeResult::GetSolutionU1Minimum() {
   // TODO
   return 1;
}

double SchemeResult::GetSolutionU2Maximum() {
   // TODO
   return 1;
}

double SchemeResult::GetSolutionU2Minimum() {
   // TODO
   return 1;
}


int SchemeResult::TimeToIndex(double t) {
   int index = static_cast<int>(std::round(t / mTimeStep));
   int iterationsCount = mLayersCount;
   if (index >= iterationsCount)
      index = iterationsCount - 1;

   return index;
}
