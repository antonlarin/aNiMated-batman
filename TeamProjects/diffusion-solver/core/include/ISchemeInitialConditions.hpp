#ifndef ISchemeInitialConditions_H
#define ISchemeInitialConditions_H

#include "CoreGlobal.hpp"

namespace diffusioncore {
   class EXPORT_API ISchemeInitialConditions {
   public:
      virtual double GetValueU1(double x) = 0;
      virtual double GetValueU2(double x) = 0;
   };
}

#endif