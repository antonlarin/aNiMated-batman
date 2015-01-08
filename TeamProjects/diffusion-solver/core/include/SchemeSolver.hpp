#ifndef SchemeSolver_H
#define SchemeSolver_H

#include <memory>
#include <functional>
#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeSolverMode.hpp"
#include "SchemeSolverResult.hpp"
#include "SchemeSolverIterationInfo.hpp"

namespace diffusioncore {

   typedef std::function<bool(SchemeSolverIterationInfo&)> 
      SolverIterationCallback;

   typedef std::function<void(SchemeLayer&, SchemeLayer&)>
      SolverLayersChangedCallback;

   class EXPORT_API SchemeSolver {
   private:
      SchemeSolverMode mSolverMode;
      std::shared_ptr<SchemeTask> mTask;
      SolverIterationCallback mIterationCallback;
      SolverLayersChangedCallback mLayersChangedCallback;

   public:
      SchemeSolver();
      virtual ~SchemeSolver();

      PROPERTY(SchemeSolverMode, SolverMode);

      void RegisterTask(std::shared_ptr<SchemeTask> task);
      void RegisterIterationCallback(SolverIterationCallback callback);
      void RegisterLayerChangedCallback(SolverLayersChangedCallback callback);

      SchemeSolverResult Solve();

   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task) = 0;
      virtual void CheckParametersOverride(SchemeTask task);
      bool UpdateIterationInfo(SchemeSolverIterationInfo& info);
      void UpdateCurrentLayersInfo(SchemeLayer& u1, SchemeLayer& u2);

   private:
      void CheckParameters(SchemeTask task);

   };
}

#endif