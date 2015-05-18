#ifndef SchemeSolverCommon_H
#define SchemeSolverCommon_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeGrid.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverCommon : public SchemeSolver {
   protected:
      double mK;
      double mC;
      double mMu;
      double mNu;
      double mRho;
      double mLambda1;
      double mLambda2;
      double mStepTime;
      double mAccuracyU1;
      double mAccuracyU2;
      int mIntervalsCount;
      int mIterationsCount;

      SchemeSolverMode mSolverMode;
      int mUpdateStep;

      std::unique_ptr<SchemeGrid> mGridU1;
      std::unique_ptr<SchemeGrid> mGridU2;

      double* mPrevLayerU1;
      double* mCurrLayerU1;
      double* mPrevLayerU2;
      double* mCurrLayerU2;

      double mMaxDiffU1;
      double mMaxDiffU2;

   public:
      SchemeSolverCommon();
      virtual ~SchemeSolverCommon();

   protected:
      virtual void DoSolverIteration() = 0;
      virtual void InitializeSchemeParameters(const SchemeTask& task);
      virtual void InitializeGrid(const SchemeTask& task);
      virtual void CheckParametersOverride(const SchemeTask& task);
      virtual void PrepareSolverOverride(const SchemeTask& task);
      virtual void CleanupSolverOverride(const SchemeTask& task);
      virtual SchemeSolverResult SolveOverride(SchemeTask task);

   private:
      void UpdateCurrentLayersDifference();
      SchemeSolverResult ConstructSolverResult(int itersCount,
                                               const SchemeTask& task);
      SchemeSolverIterationInfo ConstructSolverIntermediateResult(
         int itersCount);
      bool UpdateCurrentIteratonInfo(int itersCount,
                                     const SchemeTask& task);
      bool CheckStopCondition();

      void PrepareSolver(const SchemeTask& task);
      void CleanupSolver(const SchemeTask& task);

   };
}

#endif
