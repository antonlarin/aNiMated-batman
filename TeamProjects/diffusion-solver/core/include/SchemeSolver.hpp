#ifndef SchemeSolver_H
#define SchemeSolver_H

#include <mutex>
#include <memory>
#include <thread>
#include <exception>
#include <functional>
#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"
#include "SchemeResult.hpp"
#include "ISchemeInitialConditions.hpp"

namespace diffusioncore {

   typedef std::function<void(SchemeResult&)> SolverCallback;
   typedef std::function<void(std::exception&)> ExceptionCallback;

   typedef std::shared_ptr<ISchemeInitialConditions> InitialConditionsPtr;

   enum SchemeSolvingMode {
      AllLayers,
      StableLayer
   };

   class EXPORT_API SchemeSolver {
   private:
      double mK;
      double mC;
      double mNu;
      double mMu;
      double mRho;
      double mLambda1;
      double mLambda2;
      double mStepTime;
      double mAccuracy;
      int mIntervalsCount;
      int mMaximumIterations;
      SchemeSolvingMode mSolvingMode;
      InitialConditionsPtr mInitialConditions;
   
      bool mIsSolving;
      bool mIsStop;
      std::thread mSolverThread;
      std::mutex mSolverMutex;

   protected:
      const double SCHEME_X_BEGIN = 0.0;
      const double SCHEME_X_END = 1.0;

      int mIterationsCount;

   public:
      SchemeSolver();
      virtual ~SchemeSolver();

      PROPERTY(double, K);
      PROPERTY(double, C);
      PROPERTY(double, Nu);
      PROPERTY(double, Mu);
      PROPERTY(double, Rho);
      PROPERTY(double, Lambda1);
      PROPERTY(double, Lambda2);
      PROPERTY(double, StepTime);
      PROPERTY(double, Accuracy);
      PROPERTY(int, IntervalsCount);
      PROPERTY(int, MaximumIterations);
      PROPERTY(SchemeSolvingMode, SolvingMode);
      PROPERTY(InitialConditionsPtr, InitialConditions);

      double GetMaximumTime();
      int GetIterationsCount();

      bool IsSolving();
      void StopSolving();

      void BeginSolve(
         SolverCallback callback,
         ExceptionCallback exCallback);

      void WaitSolve();

   protected:

      bool IsStoped();
      virtual SchemeResult SolveOverride() = 0;
      virtual void CheckParametersOverride() = 0;

   private:
      void CheckSolverThreadStatus();
      void SolveNewThread(SolverCallback callback, 
                          ExceptionCallback exCallback);
      void CheckParameters();

   };
}

#endif