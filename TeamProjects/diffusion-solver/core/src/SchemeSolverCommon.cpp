#include "CoreUtils.hpp"
#include "SchemeSolverCommon.hpp"
using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolverCommon::SchemeSolverCommon() { }
SchemeSolverCommon::~SchemeSolverCommon() { }
   

void SchemeSolverCommon::InitializeGrid(const SchemeTask& task) {
   int layersCount = task.GetMaximumLayers();
   SchemeSolverMode solvingMode = GetSolverMode();

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto initialLayerU2 = task.GetInitialLayerU2();
   mGridU1 = SchemeGrid(layersCount, initialLayerU1, solvingMode); 
   mGridU2 = SchemeGrid(layersCount, initialLayerU2, solvingMode);
}

bool SchemeSolverCommon::UpdateCurrentSolution(int itersCount, 
                                               const SchemeTask& task) {
   int n = mIntervalsCount;
   mMaxDiffU1 = MaxDifference(mCurrLayerU1, mPrevLayerU1, n + 1);
   mMaxDiffU2 = MaxDifference(mCurrLayerU2, mPrevLayerU2, n + 1);

   SchemeSolution solutionU1 = mBuilderU1.Build(mGridU1);
   SchemeSolution solutionU2 = mBuilderU2.Build(mGridU2);
   SchemeStatistic statistic(itersCount, mMaxDiffU1, mMaxDiffU2);
   SchemeSolverResult result(solutionU1, solutionU2, statistic, task, true);
   
   if (!UpdateIterationInfo(result))
      return true;

   if (GetSolverMode() == SchemeSolverMode::StableLayer)
      return mMaxDiffU1 < mAccuracyU1 && mMaxDiffU2 < mAccuracyU2;

   return false;
}


SchemeSolverResult SchemeSolverCommon::SolveOverride(SchemeTask task) {
   PrepareSolver(task);

   mPerformedIterationsCount = 0;
   SolvingLoop(task);
   SchemeSolverResult result = ConstructSolvingResult(task);

   CleanupSolver(task);
   return result;
}

SchemeSolverResult SchemeSolverCommon::ContinueSolvingOverride(SchemeTask task) {
   PrepareSolverOverride(task);

   mGridU1.NextLayer();
   mGridU2.NextLayer();
   SolvingLoop(task);

   SchemeSolverResult result = ConstructSolvingResult(task);

   CleanupSolverOverride(task);
   return result;
}

void SchemeSolverCommon::InitializeSchemeParameters(const SchemeTask& task) {
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

void SchemeSolverCommon::CheckParametersOverride(const SchemeTask& task) { }
void SchemeSolverCommon::CleanupSolverOverride(const SchemeTask& task) { }
void SchemeSolverCommon::PrepareSolverOverride(const SchemeTask& task) { }


void SchemeSolverCommon::SolvingLoop(const SchemeTask& task) {
   int maxIterations = task.GetMaximumIterations();
   int updateStep = GetIterationInfoUpdateStep();
   while (mPerformedIterationsCount < maxIterations) {
      mCurrLayerU1 = mGridU1.GetCurrentLayer();
      mCurrLayerU2 = mGridU2.GetCurrentLayer();
      mPrevLayerU1 = mGridU1.GetPrevousLayer();
      mPrevLayerU2 = mGridU2.GetPrevousLayer();
      
      mPerformedIterationsCount++;
      mBuilderU1.SetIterationsCount(mPerformedIterationsCount);
      mBuilderU2.SetIterationsCount(mPerformedIterationsCount);
      
      DoSolverIteration();
      if (mPerformedIterationsCount % updateStep) {
         if (UpdateCurrentSolution(mPerformedIterationsCount, task))
            break;
      }

      mGridU1.NextLayer();
      mGridU2.NextLayer();
   }
}

SchemeSolverResult SchemeSolverCommon::ConstructSolvingResult(
      const SchemeTask& task) {
   SchemeSolution solutionU1 = mBuilderU1.Build(mGridU1);
   SchemeSolution solutionU2 = mBuilderU2.Build(mGridU2);
   SchemeStatistic statistic(mPerformedIterationsCount, mMaxDiffU1, mMaxDiffU2);
   
   int maxIterations = task.GetMaximumIterations();
   bool isContinuationAvailable;
   if (maxIterations > mPerformedIterationsCount)
   {
      isContinuationAvailable = true;
   }
   else
   {
      isContinuationAvailable = false;
   }

   return SchemeSolverResult(solutionU1, solutionU2, statistic, task,
                             isContinuationAvailable);
}

void SchemeSolverCommon::PrepareSolver(const SchemeTask& task) { 
   InitializeSchemeParameters(task);
   InitializeGrid(task);

   mBuilderU1 = SchemeSolutionBuilder(task, GetSolverMode());
   mBuilderU2 = SchemeSolutionBuilder(task, GetSolverMode());

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto initialLayerU2 = task.GetInitialLayerU2();
   mBuilderU1.UpdateMinMaxValues(initialLayerU1);
   mBuilderU2.UpdateMinMaxValues(initialLayerU2);

   PrepareSolverOverride(task);
}

void SchemeSolverCommon::CleanupSolver(const SchemeTask& task) {
   CleanupSolverOverride(task);
}
