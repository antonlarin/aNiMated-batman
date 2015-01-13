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

   auto builderU1 = new SchemeSolutionBuilder(task, GetSolverMode());
   mBuilderU1.reset(builderU1);

   auto builderU2 = new SchemeSolutionBuilder(task, GetSolverMode());
   mBuilderU2.reset(builderU2);
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
   PrepareSolver(task);

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

   mBuilderU1->SetIterationsCount(iterationsCount);
   mBuilderU2->SetIterationsCount(iterationsCount);

   SchemeSolution solutionU1 = mBuilderU1->Build(*mGridU1.get());
   SchemeSolution solutionU2 = mBuilderU2->Build(*mGridU2.get());
   SchemeStatistic statistic(iterationsCount, mMaxDiffU1, mMaxDiffU2);
   SchemeSolverResult result(solutionU1, solutionU2, statistic, task);

   CleanupSolver(task);
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
void SchemeSolverCommon::CleanupSolverOverride(const SchemeTask& task) { }
void SchemeSolverCommon::PrepareSolverOverride(const SchemeTask& task) { }


void SchemeSolverCommon::PrepareSolver(const SchemeTask& task) { 
   mBuilderU1->ResetMinMaxValues();
   mBuilderU2->ResetMinMaxValues();

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto initialLayerU2 = task.GetInitialLayerU2();
   mBuilderU1->UpdateMinMaxValues(initialLayerU1);
   mBuilderU2->UpdateMinMaxValues(initialLayerU2);

   PrepareSolverOverride(task);
}

void SchemeSolverCommon::CleanupSolver(const SchemeTask& task) {
   CleanupSolverOverride(task);
}

void SchemeSolverCommon::UpdateCurrentLayersInfoInternal() {
   SchemeLayer layerU1(mCurrLayerU1, mIntervalsCount + 1);
   SchemeLayer layerU2(mCurrLayerU2, mIntervalsCount + 1);
   UpdateCurrentLayersInfo(layerU1, layerU2);   
}