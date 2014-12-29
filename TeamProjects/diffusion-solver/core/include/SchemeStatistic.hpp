#ifndef SchemeStatistic_H
#define SchemeStatistic_H

#include "CoreGlobal.hpp"

namespace diffusioncore {

   class EXPORT_API SchemeStatistic {
   private:
      int mIterationsCount;
      double mStopAccuracyU1;
      double mStopAccuracyU2;

      bool mIsInitialized;

   public:
      SchemeStatistic();
      SchemeStatistic(int iterationsCount, 
                      double stopAccuracyU1,
                      double stopAccuracyU2);
      ~SchemeStatistic();
      
      int GetIterationsCount() const;
      double GetStopAccuracyU1() const;
      double GetStopAccuracyU2() const;

   private:
      void CheckIsInitialized() const;

   };

}

#endif