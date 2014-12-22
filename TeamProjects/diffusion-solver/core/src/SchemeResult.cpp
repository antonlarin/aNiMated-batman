#include <cmath>
#include <limits>
#include <cassert>
#include "SchemeResult.hpp"
using namespace diffusioncore;

const double SIGNALING_NAN = std::numeric_limits<double>::signaling_NaN();

SchemeResult::SchemeResult(std::shared_ptr<double> solutionU1,
                           std::shared_ptr<double> solutionU2,
                           int intervalsCount,
                           int layersCount,
                           double timeStep) {
   assert(intervalsCount > 0);
   assert(layersCount > 0);

   mU1Max = SIGNALING_NAN;
   mU1Min = SIGNALING_NAN;
   mU2Max = SIGNALING_NAN;
   mU2Min = SIGNALING_NAN;

   mSolutionU1 = solutionU1;
   mSolutionU2 = solutionU2;

   mTimeStep = timeStep;
   mLayersCount = layersCount;
   mIntervalsCount = intervalsCount;
   mSolutionLength = (intervalsCount + 1) * layersCount;
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
   if (mU1Max != SIGNALING_NAN)
      return mU1Max;

   double* u1 = mSolutionU1.get();
   double u1Max = u1[0];
   for (int i = 1; i < mSolutionLength; ++i)
      u1Max = std::max(u1Max, u1[i]);

   mU1Max = u1Max;
   return u1Max;
}

double SchemeResult::GetSolutionU1Minimum() {
   if (mU1Min != SIGNALING_NAN)
      return mU1Min;

   double* u1 = mSolutionU1.get();
   double u1Min = u1[0];
   for (int i = 1; i < mSolutionLength; ++i)
      u1Min = std::min(u1Min, u1[i]);

   mU1Min = u1Min;
   return u1Min;
}

double SchemeResult::GetSolutionU2Maximum() {
   if (mU2Max != SIGNALING_NAN)
      return mU2Max;

   double* u2 = mSolutionU2.get();
   double u2Max = u2[0];
   for (int i = 2; i < mSolutionLength; ++i)
      u2Max = std::max(u2Max, u2[i]);

   mU2Max = u2Max;
   return u2Max;
}

double SchemeResult::GetSolutionU2Minimum() {
   if (mU2Min != SIGNALING_NAN)
      return mU2Min;

   double* u2 = mSolutionU2.get();
   double u2Min = u2[0];
   for (int i = 2; i < mSolutionLength; ++i)
      u2Min = std::min(u2Min, u2[i]);

   mU2Min = u2Min;
   return u2Min;
}


int SchemeResult::TimeToIndex(double t) {
   int index = static_cast<int>(std::round(t / mTimeStep));
   int iterationsCount = mLayersCount;
   if (index >= iterationsCount)
      index = iterationsCount - 1;

   return index;
}
