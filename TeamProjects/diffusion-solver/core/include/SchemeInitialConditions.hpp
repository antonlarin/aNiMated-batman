#ifndef SchemeInitialConditions_H
#define SchemeInitialConditions_H

#include "CoreGlobal.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeInitialConditions final {
   public:
      SchemeInitialConditions();
      ~SchemeInitialConditions();

      double GetValueU1(double x);
      double GetValueU2(double x);
   };
}

#endif