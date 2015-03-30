#ifndef SchemeSolverResult_H
#define SchemeSolverResult_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeSolution.hpp"
#include "SchemeStatistic.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverResult final {
   private:
      SchemeTask mTask;
      SchemeSolution mSolutionU1;
      SchemeSolution mSolutionU2;
      SchemeStatistic mStatistic;

      int mLayersCount;
      int mIntervalsCount;
      bool mIsInitialized;
      bool mIsContinuationAvailable;

   public:
      SchemeSolverResult();
      SchemeSolverResult(const SchemeSolution& solutionU1,
                         const SchemeSolution& solutionU2,
                         const SchemeStatistic& statistic,
                         const SchemeTask& task,
                         bool isContinuationAvailable);
      ~SchemeSolverResult();

      int GetLayersCount() const;
      int GetIntervalsCount() const;

      SchemeSolution GetSolutionU1() const;
      SchemeSolution GetSolutionU2() const;
      SchemeStatistic GetStatistic() const;
      SchemeTask GetTask() const;

      bool IsContinuationAvailable() const;

   private:
      void CheckIsInitialized() const;

   };
}

#endif
