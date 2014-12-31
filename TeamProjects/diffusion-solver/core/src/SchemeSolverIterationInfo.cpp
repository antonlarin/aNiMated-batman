#include <cassert>
#include <stdexcept>
#include "SchemeSolverIterationInfo.hpp"
using namespace diffusioncore;

SchemeSolverIterationInfo::SchemeSolverIterationInfo() {
   mIsInitialized = false;
}

SchemeSolverIterationInfo::SchemeSolverIterationInfo(int iterNumber, 
                                                     int totalIters, 
                                                     double accuracyU1, 
                                                     double accuracyU2) {
   assert(iterNumber > 0);
   assert(totalIters > 0);
   assert(accuracyU1 >= 0);
   assert(accuracyU2 >= 0);

   mIterNumber = iterNumber;
   mTotalIters = totalIters;
   mAccuracyU1 = accuracyU1;
   mAccuracyU2 = accuracyU2;

   mIsInitialized = true;
}

SchemeSolverIterationInfo::~SchemeSolverIterationInfo() { }


int SchemeSolverIterationInfo::GetIterationNumber() const {
   CheckIsInitialized();
   return mIterNumber;
}

int SchemeSolverIterationInfo::GetTotalIterations() const {
   CheckIsInitialized();
   return mTotalIters;
}

double SchemeSolverIterationInfo::GetAccuracyU1() const {
   CheckIsInitialized();
   return mAccuracyU1;
}

double SchemeSolverIterationInfo::GetAccuracyU2() const {
   CheckIsInitialized();
   return mAccuracyU2;
}


void SchemeSolverIterationInfo::CheckIsInitialized() const {
   if (!mIsInitialized)
      throw std::runtime_error("Object is not initialized");
}