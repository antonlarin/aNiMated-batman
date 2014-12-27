#ifndef SchemeLayer_H
#define SchemeLayer_H

#include <vector>
#include "CoreGlobal.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeLayer final {
   private:
      std::vector<double> mVector;

   public:
      static double MaxDifference(
         const SchemeLayer& lhs, 
         const SchemeLayer& rhs);

      SchemeLayer(const double* v, int length);
      ~SchemeLayer();

      int GetLength() const;
      double Get(int index) const;
      double operator[] (int index) const;
   };
}

#endif