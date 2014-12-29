#ifndef SchemeInitialConditions_H
#define SchemeInitialConditions_H

#ifdef _MSC_VER
#pragma warning( disable: 4251 )
#endif

#include <vector>
#include "CoreGlobal.hpp"
#include "ISchemeInitialConditions.hpp"

namespace diffusioncore {
   class EXPORT_API DefaultSchemeInitialConditions : 
      public ISchemeInitialConditions {
   private:
      std::vector<double> mCoeffU1;
      std::vector<double> mCoeffU2;

   public:
      DefaultSchemeInitialConditions(
         std::vector<double> coeffU1,
         std::vector<double> coeffU2);

      ~DefaultSchemeInitialConditions();


      virtual bool CheckConditions(
         double start, 
         double end, 
         int intervals);

      virtual double GetValueU1(double x);
      virtual double GetValueU2(double x);
   };
}

#endif