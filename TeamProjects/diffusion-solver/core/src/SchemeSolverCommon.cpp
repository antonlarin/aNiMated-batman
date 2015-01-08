#include "CoreUtils.hpp"
#include "SchemeSolverCommon.hpp"
#include "SchemeSolverIterationInfo.hpp"
using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolverCommon::SchemeSolverCommon() { }
SchemeSolverCommon::~SchemeSolverCommon() { }
   

void SchemeSolverCommon::InitializeGrid(SchemeTask& task) {
   int layersCount = task.GetMaximumIterations() + 1;
   SchemeSolverMode solvingMode = GetSolverMode();

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto gridU1 = new SchemeGrid(layersCount, initialLayerU1, solvingMode);
   mGridU1.reset(gridU1); 

   auto initialLayerU2 = task.GetInitialLayerU2();
   auto gridU2 = new SchemeGrid(layersCount, initialLayerU2, solvingMode);
   mGridU2.reset(gridU2);
}

bool SchemeSolverCommon::CheckStopCondition(int itersCount) {
   int n = mIntervalsCount;
   mMaxDiffU1 = MaxDifference(mCurrLayerU1, mPrevLayerU1, n + 1);
   mMaxDiffU2 = MaxDifference(mCurrLayerU2, mPrevLayerU2, n + 1);
   SchemeSolverIterationInfo iterInfo(itersCount, 
                                      mIterationsCount,
                                      mMaxDiffU1,
                                      mMaxDiffU2);
   UpdateCurrentLayersInfoInternal();
   if (!UpdateIterationInfo(iterInfo))
      return true;

   if (GetSolverMode() == SchemeSolverMode::StableLayer)
      return mMaxDiffU1 < mAccuracyU1 && mMaxDiffU2 < mAccuracyU2;

   return false;
}


SchemeSolverResult SchemeSolverCommon::SolveOverride(SchemeTask task) {
   InitializeSchemeParameters(task);
   InitializeGrid(task);
   PrepareSolver();

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

   CleanupSolver();
   return result;
}

void SchemeSolverCommon::InitializeSchemeParameters(SchemeTask& task) {
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

void SchemeSolverCommon::CheckParametersOverride(SchemeTask task) { }
void SchemeSolverCommon::PrepareSolver() { }
void SchemeSolverCommon::CleanupSolver() { }


void SchemeSolverCommon::UpdateCurrentLayersInfoInternal() {
   SchemeLayer layerU1(mCurrLayerU1, mIntervalsCount + 1);
   SchemeLayer layerU2(mCurrLayerU2, mIntervalsCount + 1);
   UpdateCurrentLayersInfo(layerU1, layerU2);   
}