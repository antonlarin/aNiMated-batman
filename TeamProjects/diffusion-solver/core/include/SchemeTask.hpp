#ifndef SchemeTask_H
#define SchemeTask_H

#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeTask final
   {
   private:
      double mK;
      double mC;
      double mNu;
      double mMu;
      double mRho;
      double mLambda1;
      double mLambda2;
      double mStepTime;
      double mAccuracyU1;
      double mAccuracyU2;
      int mIntervalsCount;
      int mMaximumIterations;
      SchemeLayer mStartLayerU1;
      SchemeLayer mStartLayerU2;

   public:
      SchemeTask();
      ~SchemeTask();
      
      PROPERTY(double, K);
      PROPERTY(double, C);
      PROPERTY(double, Nu);
      PROPERTY(double, Mu);
      PROPERTY(double, Rho);
      PROPERTY(double, Lambda1);
      PROPERTY(double, Lambda2);
      PROPERTY(double, StepTime);
      PROPERTY(double, AccuracyU1);
      PROPERTY(double, AccuracyU2);
      PROPERTY(int, MaximumIterations);

      double GetSegmentEnd() const;
      double GetSegmentBegin() const;

      int GetMaximumLayers() const;
      int GetIntervalsCount() const;
      SchemeLayer GetInitialLayerU1() const;
      SchemeLayer GetInitialLayerU2() const;

      void SetInitialLayers(
         const SchemeLayer& u1,
         const SchemeLayer& u2);

      SchemeTask Clone();

   };
}

#endif