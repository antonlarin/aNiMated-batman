#ifndef SchemeInitialConditions_H
#define SchemeInitialConditions_H

#include "CoreGlobal.hpp"
#include "ISchemeInitialConditions.hpp"

namespace diffusioncore {
   class EXPORT_API DefaultSchemeInitialConditions : 
      public ISchemeInitialConditions {
   public:
      DefaultSchemeInitialConditions();
      ~DefaultSchemeInitialConditions();

      virtual double GetValueU1(double x);
      virtual double GetValueU2(double x);
   };
}

#endif