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
      virtual void PrepareSolver();
      virtual void CleanupSolver();
      virtual void DoSolverIteration() = 0;
      virtual void InitializeGrid(SchemeTask& task); 
      virtual bool CheckStopCondition(int iterCount);
      virtual void CheckParametersOverride(SchemeTask task);
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual void InitializeSchemeParameters(SchemeTask& task);

   private:
      void UpdateCurrentLayersInfoInternal();

   };
}

#endif