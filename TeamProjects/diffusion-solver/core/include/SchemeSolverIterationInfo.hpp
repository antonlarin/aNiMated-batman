#ifndef SchemeSolverIterationInfo_H
#define SchemeSolverIterationInfo_H

#include "CoreGlobal.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverIterationInfo final {
   private:
      int mIterNumber;
      int mTotalIters;
      double mAccuracyU1;
      double mAccuracyU2;
      bool mIsInitialized;

   public:
      SchemeSolverIterationInfo();
      SchemeSolverIterationInfo(int iterNumber, 
                                int totalIters, 
                                double accuracyU1, 
                                double accuracyU2);
      ~SchemeSolverIterationInfo();

      int GetIterationNumber() const;
      int GetTotalIterations() const;
      double GetAccuracyU1() const;
      double GetAccuracyU2() const;
   
   private:
      void CheckIsInitialized() const;

   };
}

#endif