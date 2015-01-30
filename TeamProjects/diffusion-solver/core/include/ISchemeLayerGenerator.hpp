#ifndef ISchemeLayerGenerator_H
#define ISchemeLayerGenerator_H

#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"

namespace diffusioncore {
   class EXPORT_API ISchemeLayerGenerator
   {
   public:
      ISchemeLayerGenerator();
      virtual ~ISchemeLayerGenerator();
      
      virtual SchemeLayer Generate() = 0;
      virtual bool CheckParameters() = 0;

   };
}

#endif