#include "CoreUtils.hpp"
#include "SchemeSolverCommon.hpp"
using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolverCommon::SchemeSolverCommon() { }
SchemeSolverCommon::~SchemeSolverCommon() { }

SchemeSolverResult SchemeSolverCommon::SolveOverride(SchemeTask task) {
   PrepareSolver(task);

   bool needToStop = false;
   int iterationsCount = task.GetStartIterationIndex();
   int maxIterations = task.GetEndIterationIndex();
   while (iterationsCount <= maxIterations && !needToStop) {
      mCurrLayerU1 = mGridU1->GetCurrentLayer();
      mCurrLayerU2 = mGridU2->GetCurrentLayer();
      mPrevLayerU1 = mGridU1->GetPrevousLayer();
      mPrevLayerU2 = mGridU2->GetPrevousLayer();
      
      iterationsCount++;
      DoSolverIteration();
      UpdateCurrentLayersDifference();

      needToStop = 
         CheckStopCondition() ||
         UpdateCurrentIteratonInfo(iterationsCount, task);

      mGridU1->NextLayer();
      mGridU2->NextLayer();
   }

   SchemeSolverResult result = ConstructSolverResult(
      iterationsCount, task);
   CleanupSolver(task);
   return result;
}

void SchemeSolverCommon::InitializeGrid(const SchemeTask& task) {
   auto initialLayerU1 = task.GetInitialLayerU1();
   auto initialLayerU2 = task.GetInitialLayerU2();
   mGridU1.reset(new SchemeGrid(task, initialLayerU1, this)); 
   mGridU2.reset(new SchemeGrid(task, initialLayerU2, this));
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
   mIterationsCount = task.GetIterationsCount();
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

   SchemeSolverIterationInfo info = 
      ConstructSolverIntermediateResult(itersCount);
   return !UpdateIterationInfo(info);
}

inline SchemeSolverResult SchemeSolverCommon::ConstructSolverResult(
   int itersCount, const SchemeTask& task) {
   SchemeSolution solutionU1 = mGridU1->BuildSolution();
   SchemeSolution solutionU2 = mGridU2->BuildSolution();
   SchemeStatistic statistic(
      mIterationsCount, itersCount, mMaxDiffU1, mMaxDiffU2);
   return SchemeSolverResult(solutionU1, solutionU2, statistic, task);
}

inline SchemeSolverIterationInfo 
SchemeSolverCommon::ConstructSolverIntermediateResult(int itersCount) {
   SchemeStatistic statistic(
      mIterationsCount, itersCount, mMaxDiffU1, mMaxDiffU2);
   SchemeWeakLayer currentLayerU1(mCurrLayerU1, mIntervalsCount);
   SchemeWeakLayer currentLayerU2(mCurrLayerU2, mIntervalsCount);
   return SchemeSolverIterationInfo(
      currentLayerU1, currentLayerU2, statistic, 
      mGridU1->GetMinValue(), mGridU1->GetMaxValue(),
      mGridU2->GetMinValue(), mGridU2->GetMaxValue());
}

void SchemeSolverCommon::PrepareSolver(const SchemeTask& task) { 
   mSolverMode = GetSolverMode();
   mUpdateStep = GetIterationInfoUpdateStep();

   InitializeSchemeParameters(task);
   InitializeGrid(task);
   PrepareSolverOverride(task);
}

void SchemeSolverCommon::CleanupSolver(const SchemeTask& task) {
   CleanupSolverOverride(task);
}
