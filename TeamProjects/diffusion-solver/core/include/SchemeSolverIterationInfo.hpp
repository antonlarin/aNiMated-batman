#ifndef SchemeSolverIterationInfo_H
#define SchemeSolverIterationInfo_H

#include "CoreGlobal.hpp"
#include "SchemeWeakLayer.hpp"
#include "SchemeStatistic.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverIterationInfo final {
   private:
      SchemeWeakLayer mLayerU1;
      SchemeWeakLayer mLayerU2;
      SchemeStatistic mStatistic;
      double mMinValueU1;
      double mMaxValueU1;
      double mMinValueU2;
      double mMaxValueU2;

   public:
      SchemeSolverIterationInfo();
      SchemeSolverIterationInfo(const SchemeWeakLayer& layerU1,
                                const SchemeWeakLayer& layerU2,
                                const SchemeStatistic& statistic,
                                double minValueU1, double maxValueU1,
                                double minValueU2, double maxValueU2);
      ~SchemeSolverIterationInfo();

      SchemeWeakLayer GetCurrentLayerU1() const;
      SchemeWeakLayer GetCurrentLayerU2() const;
      SchemeStatistic GetStatistic() const;
      double GetMinValueU1() const;
      double GetMaxValueU1() const;
      double GetMinValueU2() const;
      double GetMaxValueU2() const;
   };
}

#endif
