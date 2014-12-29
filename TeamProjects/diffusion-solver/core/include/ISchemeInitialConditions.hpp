#ifndef ISchemeInitialConditions_H
#define ISchemeInitialConditions_H

#ifdef _MSC_VER
#pragma warning( disable: 4251 )
#endif

#include "CoreGlobal.hpp"

namespace diffusioncore {
   class EXPORT_API ISchemeInitialConditions {
   public:
      virtual bool CheckConditions(
         double start, 
         double end, 
         int intervals) = 0;

      virtual double GetValueU1(double x) = 0;
      virtual double GetValueU2(double x) = 0;
   };
}

#endif