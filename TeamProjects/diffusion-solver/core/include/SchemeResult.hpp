#ifndef SchemeResult_H
#define SchemeResult_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeResult {
   private:
      int mIntervalsCount;
      int mLayersCount;
      double mTimeStep;

      std::shared_ptr<double> mSolutionU1;
      std::shared_ptr<double> mSolutionU2;

   public:
      SchemeResult(std::shared_ptr<double> solutionU1,
                   std::shared_ptr<double> solutionU2,
                   int intervalsCount,
                   int layersCount,
                   double timeStep);
      virtual ~SchemeResult();

      int GetLayersCount() const;
      int GetIntervalsCount() const;

      SchemeLayer GetSolutionU1(int index);
      SchemeLayer GetSolutionU2(int index);
      SchemeLayer GetSolutionU1(double time);
      SchemeLayer GetSolutionU2(double time);
      SchemeLayer GetLastLayerU1();
      SchemeLayer GetLastLayerU2();

      double GetSolutionU1Maximum();
      double GetSolutionU1Minimum();
      double GetSolutionU2Maximum();
      double GetSolutionU2Minimum();

   private:
      int TimeToIndex(double t);

   };
}

#endif