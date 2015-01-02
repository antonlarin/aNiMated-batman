#include <cmath>
#include <stdexcept>
#include "SchemeStatistic.hpp"
#include "SchemeSolverImplicit.hpp"
using namespace diffusioncore;

SchemeSolverImplicit::SchemeSolverImplicit() { }
SchemeSolverImplicit::~SchemeSolverImplicit() { }


void SchemeSolverImplicit::DoSolverIteration() {
   int n = mIntervalsCount;
   double k = mStepTime;
   double h = 1.0 / n;
   double tmp1, tmp2;

   for (int i = 1; i < n; i++) {
      tmp1 = 2 * mLambda1 + h * h / k - mAlpha[i - 1] * mLambda1;
      mAlpha[i] = mLambda1 / tmp1;
      tmp2 = h * h * (mPrevLayerU1[i] / k + mRho - mMu * mPrevLayerU1[i] + 
             mK * std::pow(mPrevLayerU1[i], 2) / mPrevLayerU2[i]);
      mBeta[i] = (tmp2 + mBeta[i - 1] * mLambda1) / tmp1;
   }
   
   mCurrLayerU1[n] = mBeta[n - 1] / (-mAlpha[n - 1] + 1);
   mGridU1->UpdateMinMaxValues(mCurrLayerU1[n]);
   for (int i = n; i > 0; i--) {
      mCurrLayerU1[i - 1] = mAlpha[i - 1] * mCurrLayerU1[i] + mBeta[i - 1];
      mGridU1->UpdateMinMaxValues(mCurrLayerU1[i - 1]);
   }

   for (int i = 1; i < n; i++) {
      tmp1 = 2 * mLambda2 + h * h / k - mAlpha[i - 1] * mLambda2;
      mAlpha[i] = mLambda2 / tmp1;
      tmp2 = h * h *(mPrevLayerU2[i] / k - mNu * mPrevLayerU2[i] + 
             mC * std::pow(mPrevLayerU1[i], 2));
      mBeta[i] = (tmp2 + mBeta[i - 1] * mLambda2) / tmp1;
   }

   mCurrLayerU2[n] = mBeta[n - 1] / (-mAlpha[n - 1] + 1);
   mGridU2->UpdateMinMaxValues(mCurrLayerU2[n]);
   for (int i = n; i > 0; i--) {
      mCurrLayerU2[i - 1] = mAlpha[i - 1] * mCurrLayerU2[i] + mBeta[i - 1];
      mGridU2->UpdateMinMaxValues(mCurrLayerU2[i - 1]);
   }
}

void SchemeSolverImplicit::PrepareSolver() {
   int n = mIntervalsCount;
   mAlpha = new double[n + 1];
   mBeta = new double[n + 1];
   
   mAlpha[0] = 1;
   mBeta[0] = 0;
}

void SchemeSolverImplicit::CleanupSolver() { 
   delete[] mAlpha;
   delete[] mBeta;
}
