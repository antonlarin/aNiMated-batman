#ifndef SchemeSolver_H
#define SchemeSolver_H

#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"
#include "SchemeInitialConditions.hpp"

namespace diffusioncore {
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
      SchemeInitialConditions mInitialConditions;
   
      bool mIsSolved;

   protected:
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
      PROPERTY(SchemeInitialConditions, InitialConditions);

      bool IsSolved() const;
      double GetMaximumTime() const;
      int GetIterationsCount() const;

      SchemeLayer GetSolutionU1(int index);
      SchemeLayer GetSolutionU2(int index);
      SchemeLayer GetSolutionU1(double time);
      SchemeLayer GetSolutionU2(double time);

      double GetSolutionU1Maximum();
      double GetSolutionU1Minimum();
      double GetSolutionU2Maximum();
      double GetSolutionU2Minimum();

      void Solve();

   protected:
      virtual void SolveOverride() = 0;
      virtual SchemeLayer GetSolutionU1Override(int index) = 0;
      virtual SchemeLayer GetSolutionU2Override(int index) = 0;

   private:
      int TimeToIndex(double t);

   };
}

#endif