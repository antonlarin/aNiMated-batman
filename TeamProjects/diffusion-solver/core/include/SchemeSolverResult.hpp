#ifndef SchemeSolverResult_H
#define SchemeSolverResult_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeSolution.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverResult final {
   private:
      SchemeTask mTask;
      SchemeSolution mSolutionU1;
      SchemeSolution mSolutionU2;
      int mLayersCount;
      int mIterationsCount;
      bool mIsInitialized;

   public:
      SchemeSolverResult();
      SchemeSolverResult(std::shared_ptr<double> solutionU1,
                         std::shared_ptr<double> solutionU2,
                         const SchemeTask& task,
                         int iterationsCount,
                         int layersCount);
      SchemeSolverResult(const SchemeSolution& solutionU1,
                         const SchemeSolution& solutionU2,
                         const SchemeTask& task,
                         int iterationsCount);
      ~SchemeSolverResult();

      int GetIterationsCount() const;
      int GetLayersCount() const;

      SchemeSolution GetSolutionU1() const;
      SchemeSolution GetSolutionU2() const;
      SchemeTask GetTask() const;
   };
}

#endif