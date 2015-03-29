#ifndef SchemeSolverCommon_H
#define SchemeSolverCommon_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeGrid.hpp"
#include "SchemeSolver.hpp"
#include "SchemeSolutionBuilder.hpp"

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

      SchemeGrid mGridU1;
      SchemeGrid mGridU2;
      SchemeSolutionBuilder mBuilderU1;
      SchemeSolutionBuilder mBuilderU2;

      double* mPrevLayerU1;
      double* mCurrLayerU1;
      double* mPrevLayerU2;
      double* mCurrLayerU2;

      double mMaxDiffU1;
      double mMaxDiffU2;
      int mPerformedIterationsCount;

   public:
      SchemeSolverCommon();
      virtual ~SchemeSolverCommon();
      
   protected:
      virtual void DoSolverIteration() = 0;
      virtual void InitializeGrid(const SchemeTask& task); 
      virtual void InitializeSchemeParameters(const SchemeTask& task);
      virtual bool UpdateCurrentSolution(int iterCount, 
                                         const SchemeTask& task);

      virtual void CheckParametersOverride(const SchemeTask& task);
      virtual void PrepareSolverOverride(const SchemeTask& task);
      virtual void CleanupSolverOverride(const SchemeTask& task);
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual SchemeSolverResult ContinueSolvingOverride(SchemeTask task);

   private:
      void SolvingLoop(const SchemeTask& task);
      SchemeSolverResult ConstructSolvingResult(const SchemeTask& task);

      void PrepareSolver(const SchemeTask& task);
      void CleanupSolver(const SchemeTask& task);

   };
}

#endif
