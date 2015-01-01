#include <cmath>
#include <stdexcept>
#include "CoreUtils.hpp"
#include "SchemeGrid.hpp"
#include "SchemeStatistic.hpp"
#include "SchemeSolverExplicit.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolverExplicit::SchemeSolverExplicit() { }
SchemeSolverExplicit::~SchemeSolverExplicit() { }

SchemeSolverResult SchemeSolverExplicit::SolveOverride(SchemeTask task) {
   InitializeSchemeParameters(task);
   InitializeGrid(task);

   int iterationsCount = 0;
   int maxIterations = task.GetMaximumIterations();
   while (iterationsCount < maxIterations) {
      mCurrLayerU1 = mGridU1->GetCurrentLayer();
      mCurrLayerU2 = mGridU2->GetCurrentLayer();
      mPrevLayerU1 = mGridU1->GetPrevousLayer();
      mPrevLayerU2 = mGridU2->GetPrevousLayer();
      
      iterationsCount++;
      DoSolverIteration();
      if (CheckStopCondition(iterationsCount))
         break;

      mGridU1->NextLayer();
      mGridU2->NextLayer();
   }

   SchemeSolution solutionU1 = mGridU1->Solution(task);
   SchemeSolution solutionU2 = mGridU2->Solution(task);
   SchemeStatistic statistic(iterationsCount, mMaxDiffU1, mMaxDiffU2);
   SchemeSolverResult result(solutionU1, solutionU2, statistic, task);
   return result;
}

void SchemeSolverExplicit::CheckParametersOverride(SchemeTask task) {
   double k = task.GetStepTime();
   int n = task.GetIntervalsCount();
   double h = 1.0 / n;

   if (k > h * h / 2)
      throw std::runtime_error("Incompatible intervlas count and time step");
}


void SchemeSolverExplicit::InitializeSchemeParameters(SchemeTask& task) {
   mK = task.GetK();
   mC = task.GetC();
   mMu = task.GetMu();
   mNu = task.GetNu();
   mRho = task.GetRho();
   mLambda1 = task.GetLambda1();
   mLambda2 = task.GetLambda2();
   mStepTime = task.GetStepTime();
   mAccuracyU1 = task.GetAccuracyU1();
   mAccuracyU2 = task.GetAccuracyU2();
   mIntervalsCount = task.GetIntervalsCount();
   mIterationsCount = task.GetMaximumIterations();
}

void SchemeSolverExplicit::InitializeGrid(SchemeTask& task) {
   int layersCount = task.GetMaximumIterations() + 1;
   SchemeSolverMode solvingMode = GetSolverMode();

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto gridU1 = new SchemeGrid(layersCount, initialLayerU1, solvingMode);
   mGridU1.reset(gridU1); 

   auto initialLayerU2 = task.GetInitialLayerU2();
   auto gridU2 = new SchemeGrid(layersCount, initialLayerU2, solvingMode);
   mGridU2.reset(gridU2);
}

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

      mGridU1->UpdateMinMaxValues(mCurrLayerU1[i]);
      mGridU2->UpdateMinMaxValues(mCurrLayerU2[i]);
   }

   mCurrLayerU1[0] = (4 * mCurrLayerU1[1] - mCurrLayerU1[2]) / 3;
   mCurrLayerU1[n] = (4 * mCurrLayerU1[n - 1] - mCurrLayerU1[n - 2]) / 3;
   mGridU1->UpdateMinMaxValues(mCurrLayerU1[0]);
   mGridU1->UpdateMinMaxValues(mCurrLayerU1[n]);

   mCurrLayerU2[0] = (4 * mCurrLayerU2[1] - mCurrLayerU2[2]) / 3;
   mCurrLayerU2[n] = (4 * mCurrLayerU2[n - 1] - mCurrLayerU2[n - 2]) / 3;
   mGridU2->UpdateMinMaxValues(mCurrLayerU2[0]);
   mGridU2->UpdateMinMaxValues(mCurrLayerU2[n]);
}

bool SchemeSolverExplicit::CheckStopCondition(int itersCount) {
   int n = mIntervalsCount;
   mMaxDiffU1 = MaxDifference(mCurrLayerU1, mPrevLayerU1, n + 1);
   mMaxDiffU2 = MaxDifference(mCurrLayerU2, mPrevLayerU2, n + 1);
   SchemeSolverIterationInfo iterInfo(itersCount, 
                                      mIterationsCount,
                                      mMaxDiffU1,
                                      mMaxDiffU2);
   if (!UpdateIterationInfo(iterInfo))
      return true;

   if (GetSolverMode() == SchemeSolverMode::StableLayer)
      return mMaxDiffU1 < mAccuracyU1 && mMaxDiffU2 < mAccuracyU2;

   return false;
}