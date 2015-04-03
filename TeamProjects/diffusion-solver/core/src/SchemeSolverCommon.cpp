#include "CoreUtils.hpp"
#include "SchemeSolverCommon.hpp"
using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolverCommon::SchemeSolverCommon() { }
SchemeSolverCommon::~SchemeSolverCommon() { }

SchemeSolverResult SchemeSolverCommon::SolveOverride(SchemeTask task) {
   PrepareSolver(task);

   bool needToStop = false;
   int iterationsCount = 0;
   int maxIterations = task.GetMaximumIterations();
   while (iterationsCount < maxIterations && !needToStop) {
      mCurrLayerU1 = mGridU1.GetCurrentLayer();
      mCurrLayerU2 = mGridU2.GetCurrentLayer();
      mPrevLayerU1 = mGridU1.GetPrevousLayer();
      mPrevLayerU2 = mGridU2.GetPrevousLayer();
      
      iterationsCount++;
      mBuilderU1.SetIterationsCount(iterationsCount);
      mBuilderU2.SetIterationsCount(iterationsCount);
      
      DoSolverIteration();
      UpdateCurrentLayersDifference();

      needToStop = 
         CheckStopCondition() ||
         UpdateCurrentIteratonInfo(iterationsCount, task);

      mGridU1.NextLayer();
      mGridU2.NextLayer();
   }

   SchemeSolverResult result = ConstructSolverResult(
      iterationsCount, task);
   CleanupSolver(task);
   return result;
}

void SchemeSolverCommon::InitializeGrid(const SchemeTask& task) {
   int layersCount = task.GetMaximumLayers();
   SchemeSolverMode solvingMode = GetSolverMode();

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto initialLayerU2 = task.GetInitialLayerU2();
   mGridU1 = SchemeGrid(layersCount, initialLayerU1, solvingMode); 
   mGridU2 = SchemeGrid(layersCount, initialLayerU2, solvingMode);
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


inline void SchemeSolverCommon::UpdateCurrentLayersDifference() {
   int n = mIntervalsCount;
   mMaxDiffU1 = MaxDifference(mCurrLayerU1, mPrevLayerU1, n + 1);
   mMaxDiffU2 = MaxDifference(mCurrLayerU2, mPrevLayerU2, n + 1);
}

inline bool SchemeSolverCommon::CheckStopCondition() {
   if (mSolverMode == SchemeSolverMode::StableLayer)
      return mMaxDiffU1 < mAccuracyU1 && mMaxDiffU2 < mAccuracyU2;

   return false;
}

inline bool SchemeSolverCommon::UpdateCurrentIteratonInfo(
   int itersCount, const SchemeTask& task) {
   if (itersCount % mUpdateStep)
      return false;

   SchemeSolverResult result = ConstructSolverResult(itersCount, task);
   return !UpdateIterationInfo(result);
}

inline SchemeSolverResult SchemeSolverCommon::ConstructSolverResult(
   int itersCount, const SchemeTask& task) {
   SchemeSolution solutionU1 = mBuilderU1.Build(mGridU1);
   SchemeSolution solutionU2 = mBuilderU2.Build(mGridU2);
   SchemeStatistic statistic(itersCount, mMaxDiffU1, mMaxDiffU2);
   return SchemeSolverResult(solutionU1, solutionU2, statistic, task);
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

   mSolverMode = GetSolverMode();
   mUpdateStep = GetIterationInfoUpdateStep();

   PrepareSolverOverride(task);
}

void SchemeSolverCommon::CleanupSolver(const SchemeTask& task) {
   CleanupSolverOverride(task);
}
