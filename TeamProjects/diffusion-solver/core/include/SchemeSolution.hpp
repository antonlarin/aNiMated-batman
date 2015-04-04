#ifndef SchemeSolution_H
#define SchemeSolution_H

#include <vector>
#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeLayer.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolution {
   private:
      std::vector<SchemeLayer> mLayers;
      double mMinValue;
      double mMaxValue;

   public:
      SchemeSolution();
      SchemeSolution(const std::vector<SchemeLayer>& layers,
                     double minValue, double maxValue);
      SchemeSolution(const SchemeLayer& layer, 
                     double minValue, double maxValue);
      ~SchemeSolution();

      SchemeLayer GetLayer(int index) const;
      SchemeLayer operator[] (int index) const;

      SchemeLayer GetFirstLayer() const;
      SchemeLayer GetLastLayer() const;

      double GetMaximum() const;
      double GetMinimum() const;
      int GetLayersCount() const;
   };

}

#endif