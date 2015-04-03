#ifndef SchemeSolver_H
#define SchemeSolver_H

#include <memory>
#include <functional>
#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeSolverMode.hpp"
#include "SchemeSolverResult.hpp"

namespace diffusioncore {

   typedef std::function<bool(SchemeSolverResult&)> 
      SolverIterationCallback;

   class EXPORT_API SchemeSolver {
   private:
      SchemeSolverMode mSolverMode;
      std::shared_ptr<SchemeTask> mCurrentTask;
      SolverIterationCallback mIterationCallback;
      int mIterationInfoUpdateStep;

   public:
      SchemeSolver();
      virtual ~SchemeSolver();

      PROPERTY(SchemeSolverMode, SolverMode);
      PROPERTY(std::shared_ptr<SchemeTask>, CurrentTask);
      PROPERTY(SolverIterationCallback, IterationCallback);
      PROPERTY(int, IterationInfoUpdateStep);

      void RegisterIterationCallback(SolverIterationCallback callback);

      SchemeSolverResult Solve();
      SchemeSolverResult ContinueSolving();

   protected:
      bool UpdateIterationInfo(SchemeSolverResult& result);

      virtual SchemeSolverResult SolveOverride(SchemeTask task) = 0;
      virtual SchemeSolverResult ContinueSolvingOverride(SchemeTask task) = 0;
      virtual void CheckParametersOverride(SchemeTask task);

   private:
      void CheckParameters(SchemeTask task);

   };
}

#endif
