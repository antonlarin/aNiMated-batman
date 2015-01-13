#include <cmath>
#include <stdexcept>
#include "SchemeStatistic.hpp"
#include "SchemeSolverExplicit.hpp"
using namespace diffusioncore;

SchemeSolverExplicit::SchemeSolverExplicit() { }
SchemeSolverExplicit::~SchemeSolverExplicit() { }


void SchemeSolverExplicit::DoSolverIteration() {
   int n = mIntervalsCount;
   double k = mStepTime;
   double h = 1.0 / n;
   for (int i = 1; i < n; i++) {
      mCurrLayerU1[i] = k * (mLambda1 * (mPrevLayerU1[i - 1] - 
                        2 * mPrevLayerU1[i] + mPrevLayerU1[i + 1]) / 
                        (h * h) + mK * std::pow(mPrevLayerU1[i], 2) / 
                        mPrevLayerU2[i] - mMu * mPrevLayerU1[i] + mRho) + 
                        mPrevLayerU1[i];

      mCurrLayerU2[i] = k * (mLambda2 * (mPrevLayerU2[i - 1] - 
                        2 * mPrevLayerU2[i] + mPrevLayerU2[i + 1]) / 
                        (h * h) + mC * std::pow(mPrevLayerU1[i], 2) - 
                        mNu * mPrevLayerU2[i]) + mPrevLayerU2[i];

      mBuilderU1->UpdateMinMaxValues(mCurrLayerU1[i]);
      mBuilderU2->UpdateMinMaxValues(mCurrLayerU2[i]);
   }

   mCurrLayerU1[0] = (4 * mCurrLayerU1[1] - mCurrLayerU1[2]) / 3;
   mCurrLayerU1[n] = (4 * mCurrLayerU1[n - 1] - mCurrLayerU1[n - 2]) / 3;
   mBuilderU1->UpdateMinMaxValues(mCurrLayerU1[0]);
   mBuilderU1->UpdateMinMaxValues(mCurrLayerU1[n]);

   mCurrLayerU2[0] = (4 * mCurrLayerU2[1] - mCurrLayerU2[2]) / 3;
   mCurrLayerU2[n] = (4 * mCurrLayerU2[n - 1] - mCurrLayerU2[n - 2]) / 3;
   mBuilderU2->UpdateMinMaxValues(mCurrLayerU2[0]);
   mBuilderU2->UpdateMinMaxValues(mCurrLayerU2[n]);
}

void SchemeSolverExplicit::CheckParametersOverride(SchemeTask task) {
   double k = task.GetStepTime();
   int n = task.GetIntervalsCount();
   double h = 1.0 / n;

   if (k > h * h / 2)
      throw std::runtime_error("Incompatible intervlas count and time step");
}
