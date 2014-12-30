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

   };
}

#endif