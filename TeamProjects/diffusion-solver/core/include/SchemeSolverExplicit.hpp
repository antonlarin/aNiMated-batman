#ifndef SchemeSolverExplicit_H
#define SchemeSolverExplicit_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeGrid.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverExplicit : public SchemeSolver {
   private:
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
      SchemeSolverExplicit();
      ~SchemeSolverExplicit();
   
   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual void CheckParametersOverride(SchemeTask task);
      
   private:
      void InitializeSchemeParameters(SchemeTask& task);
      void InitializeGrid(SchemeTask& task); 
      bool CheckStopCondition(int iterCount);
      void DoSolverIteration();      

   };
}

#endif