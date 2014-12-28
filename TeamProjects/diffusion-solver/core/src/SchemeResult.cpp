#include <limits>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include "SchemeResult.hpp"
using namespace diffusioncore;

const double NOT_INITIALIZED = std::numeric_limits<double>::infinity();

SchemeResult::SchemeResult() {
   mIsInitialized = false;
}

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
   mSolutionLength = (intervalsCount + 1) * layersCount;

   InitializeDefault();
   mIsInitialized = true;
}

SchemeResult::~SchemeResult() { }


int SchemeResult::GetLayersCount() const {
   CheckIsInitialized();
   return mLayersCount;
}

int SchemeResult::GetIntervalsCount() const {
   CheckIsInitialized();
   return mIntervalsCount;
}


SchemeLayer SchemeResult::GetSolutionU1(int index) {
   CheckIsInitialized();
   int pointsCount = mIntervalsCount + 1;
   double* layer = mSolutionU1.get() + pointsCount * index;
   return SchemeLayer(layer, pointsCount);
}

SchemeLayer SchemeResult::GetSolutionU2(int index) {
   CheckIsInitialized();
   int pointsCount = mIntervalsCount + 1;
   double* layer = mSolutionU2.get() + pointsCount * index;
   return SchemeLayer(layer, pointsCount);
}

SchemeLayer SchemeResult::GetSolutionU1(double t) {
   CheckIsInitialized();
   assert(t >= 0);

   int index = TimeToIndex(t);
   return GetSolutionU1(index);
}

SchemeLayer SchemeResult::GetSolutionU2(double t) {
   CheckIsInitialized();
   assert(t >= 0);

   int index = TimeToIndex(t);
   return GetSolutionU2(index);
}

SchemeLayer SchemeResult::GetLastLayerU1() {
   CheckIsInitialized();
   return GetSolutionU1(mLayersCount - 1);
}

SchemeLayer SchemeResult::GetLastLayerU2() {
   CheckIsInitialized();
   return GetSolutionU2(mLayersCount - 1);
}


double SchemeResult::GetSolutionU1Maximum() {
   CheckIsInitialized();
   if (mU1Max != NOT_INITIALIZED)
      return mU1Max;

   double* u1 = mSolutionU1.get();
   double u1Max = u1[0];
   for (int i = 1; i < mSolutionLength; ++i)
      u1Max = std::max(u1Max, u1[i]);

   mU1Max = u1Max;
   return u1Max;
}

double SchemeResult::GetSolutionU1Minimum() {
   CheckIsInitialized();
   if (mU1Min != NOT_INITIALIZED)
      return mU1Min;

   double* u1 = mSolutionU1.get();
   double u1Min = u1[0];
   for (int i = 1; i < mSolutionLength; ++i)
      u1Min = std::min(u1Min, u1[i]);

   mU1Min = u1Min;
   return u1Min;
}

double SchemeResult::GetSolutionU2Maximum() {
   CheckIsInitialized();
   if (mU2Max != NOT_INITIALIZED)
      return mU2Max;

   double* u2 = mSolutionU2.get();
   double u2Max = u2[0];
   for (int i = 1; i < mSolutionLength; ++i)
      u2Max = std::max(u2Max, u2[i]);

   mU2Max = u2Max;
   return u2Max;
}

double SchemeResult::GetSolutionU2Minimum() {
   CheckIsInitialized();
   if (mU2Min != NOT_INITIALIZED)
      return mU2Min;

   double* u2 = mSolutionU2.get();
   double u2Min = u2[0];
   for (int i = 1; i < mSolutionLength; ++i)
      u2Min = std::min(u2Min, u2[i]);

   mU2Min = u2Min;
   return u2Min;
}


int SchemeResult::TimeToIndex(double t) {
   CheckIsInitialized();
   int index = static_cast<int>(std::round(t / mTimeStep));
   int iterationsCount = mLayersCount;
   if (index >= iterationsCount)
      index = iterationsCount - 1;

   return index;
}

void SchemeResult::InitializeDefault() {
   mU1Max = NOT_INITIALIZED;
   mU1Min = NOT_INITIALIZED;
   mU2Max = NOT_INITIALIZED;
   mU2Min = NOT_INITIALIZED;
}

void SchemeResult::CheckIsInitialized() const {
   if (!mIsInitialized)
      throw std::runtime_error("Result is not initialized");
}

// TODO
// double SchemeSolver::GetMaximumTime() {
//    CheckSolverThreadStatus();
//    return (mIterationsCount - 1) * mStepTime;
// }
// int SchemeSolver::GetIterationsCount() {
//    CheckSolverThreadStatus();
//    return mIterationsCount;
// }
