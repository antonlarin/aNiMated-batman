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

namespace diffusioncore {

   typedef std::function<void(SchemeSolverResult&)> SolverCallback;
   typedef std::function<void(std::exception&)> ExceptionCallback;

   enum SchemeSolvingMode {
      AllLayers,
      StableLayer
   };

   class EXPORT_API SchemeSolver {
   private:
      std::shared_ptr<SchemeTask> mTask;
      SchemeSolvingMode mSolvingMode;

      bool mIsStop;
      bool mIsSolving;
      std::mutex mSolverMutex;
      std::thread mSolverThread;

   public:
      SchemeSolver();
      virtual ~SchemeSolver();

      PROPERTY(SchemeSolvingMode, SolvingMode);

      void BindTask(std::shared_ptr<SchemeTask> task);

      void SolveCancel();
      void SolveAsync(
         SolverCallback callback,
         ExceptionCallback exCallback);
      void SolveWait();
      bool SolveIsInProgress();

   protected:

      bool IsStoped();
      virtual SchemeSolverResult SolveOverride(SchemeTask task) = 0;
      virtual void CheckParametersOverride(SchemeTask task) = 0;

   private:
      void CheckSolverThreadStatus();
      void SolveNewThread(SolverCallback callback, 
                          ExceptionCallback exCallback,
                          SchemeTask task);
      void CheckParameters(SchemeTask task);

   };
}

#endif