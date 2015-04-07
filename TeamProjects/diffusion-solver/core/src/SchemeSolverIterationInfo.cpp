#include "SchemeSolverIterationInfo.hpp"

using namespace diffusioncore;

SchemeSolverIterationInfo::SchemeSolverIterationInfo() { }

SchemeSolverIterationInfo::SchemeSolverIterationInfo(
   const SchemeWeakLayer& layerU1,
   const SchemeWeakLayer& layerU2,
   const SchemeStatistic& statistic,
   double minValueU1, double maxValueU1,
   double minValueU2, double maxValueU2) {
   mLayerU1 = layerU1;
   mLayerU2 = layerU2;
   mStatistic = statistic;
   mMinValueU1 = minValueU1;
   mMaxValueU1 = maxValueU1;
   mMinValueU2 = minValueU2;
   mMaxValueU2 = maxValueU2;
}

SchemeSolverIterationInfo::~SchemeSolverIterationInfo() { }


SchemeWeakLayer SchemeSolverIterationInfo::GetCurrentLayerU1() const {
   return mLayerU1;
}

SchemeWeakLayer SchemeSolverIterationInfo::GetCurrentLayerU2() const {
   return mLayerU2;
}

SchemeStatistic SchemeSolverIterationInfo::GetStatistic() const {
   return mStatistic;
}

double SchemeSolverIterationInfo::GetMinValueU1() const {
   return mMinValueU1;
}

double SchemeSolverIterationInfo::GetMaxValueU1() const {
   return mMaxValueU1;
}

double SchemeSolverIterationInfo::GetMinValueU2() const {
   return mMinValueU2;
}

double SchemeSolverIterationInfo::GetMaxValueU2() const {
   return mMaxValueU2;
}
