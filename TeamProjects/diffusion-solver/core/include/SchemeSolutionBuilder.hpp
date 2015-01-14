#ifndef SchemeSolutionBuilder_H
#define SchemeSolutionBuilder_H

#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeGrid.hpp"
#include "SchemeLayer.hpp"
#include "SchemeSolver.hpp"
#include "SchemeSolution.hpp"

namespace diffusioncore {
   class SchemeSolutionBuilder final {
   private:
      SchemeTask mTask;
      double mCurrentMin;
      double mCurrentMax;
      int mIterationsCount;
      SchemeSolverMode mSolverMode;

      bool mIsInitialized;

   public:
      SchemeSolutionBuilder();
      SchemeSolutionBuilder(const SchemeTask& task, 
                            SchemeSolverMode solverMode);
      ~SchemeSolutionBuilder();

      void UpdateMinMaxValues(double value);
      void UpdateMinMaxValues(const SchemeLayer& layer);
      void ResetMinMaxValues();

      void SetIterationsCount(int itersCount);

      SchemeSolution Build(const SchemeGrid& grid) const;
      
   private:
      void CheckIsInitialized() const;

   };
}

#endif