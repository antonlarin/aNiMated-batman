#ifndef SchemeSolver_H
#define SchemeSolver_H

#include <mutex>
#include <memory>
#include <thread>
#include <exception>
#include <functional>
#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeLayer.hpp"
#include "SchemeSolverResult.hpp"
#include "SchemeSolverIterationInfo.hpp"

namespace diffusioncore {

   typedef std::function<bool(SchemeSolverIterationInfo&)> 
      SolverIterationCallback;

   enum SchemeSolvingMode {
      AllLayers,
      StableLayer
   };


   class EXPORT_API SchemeSolver {
   private:
      std::shared_ptr<SchemeTask> mTask;
      SolverIterationCallback mIterationCallback;
      SchemeSolvingMode mSolvingMode;


   public:
      SchemeSolver();
      virtual ~SchemeSolver();

      PROPERTY(SchemeSolvingMode, SolvingMode);

      void RegisterTask(std::shared_ptr<SchemeTask> task);
      void RegisterIterationCallback(SolverIterationCallback callback);

      SchemeSolverResult Solve();

   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task) = 0;
      virtual void CheckParametersOverride(SchemeTask task);
      bool UpdateIterationInfo(SchemeSolverIterationInfo& info);

   private:
      void CheckParameters(SchemeTask task);

   };
}

#endif