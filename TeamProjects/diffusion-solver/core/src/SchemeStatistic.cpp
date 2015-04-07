#include <cassert>
#include <stdexcept>
#include "SchemeStatistic.hpp"
using namespace diffusioncore;

SchemeStatistic::SchemeStatistic() {
   mIsInitialized = false;
}

SchemeStatistic::SchemeStatistic(int iterationsCount,
                                 int performedIterationsCount,
                                 double stopAccuracyU1,
                                 double stopAccuracyU2) {
   assert(iterationsCount > 0);
   assert(performedIterationsCount > 0);
   assert(stopAccuracyU1 >= 0);
   assert(stopAccuracyU2 >= 0);

   mIterationsCount = iterationsCount;
   mPerformedIterationsCount = performedIterationsCount;
   mStopAccuracyU1 = stopAccuracyU1;
   mStopAccuracyU2 = stopAccuracyU2;

   mIsInitialized = true;
}

SchemeStatistic::~SchemeStatistic() { }


int SchemeStatistic::GetIterationsCount() const {
   CheckIsInitialized();
   return mIterationsCount;
}

int SchemeStatistic::GetPerformedIterationsCount() const {
   CheckIsInitialized();
   return mPerformedIterationsCount;
}

double SchemeStatistic::GetStopAccuracyU1() const {
   CheckIsInitialized();
   return mStopAccuracyU1;
}

double SchemeStatistic::GetStopAccuracyU2() const {
   CheckIsInitialized();
   return mStopAccuracyU2;
}


void SchemeStatistic::CheckIsInitialized() const {
   if (!mIsInitialized) 
      throw std::runtime_error("SchemeStatistic is not initialized");
}