#ifndef SchemeLayerGeneratorInitial_H
#define SchemeLayerGeneratorInitial_H

#include <vector>
#include "CoreGlobal.hpp"
#include "ISchemeLayerGenerator.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeLayerGeneratorInitial : 
      public ISchemeLayerGenerator {
   private:
      std::vector<double> mCoeff;
      int mIntervalsCount;

   public:
      SchemeLayerGeneratorInitial();
      ~SchemeLayerGeneratorInitial();
      
      void SetCoefficients(std::vector<double> coeff);
      void SetIntervalsCount(int intervalsCount);

      virtual SchemeLayer Generate();

   private:
      double GetValue(double x);

   };
}

#endif