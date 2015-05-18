#ifndef SchemeWeakLayer_H
#define SchemeWeakLayer_H

#include "CoreGlobal.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeWeakLayer final {
   private:
      double const *mSource;
      int mSourceSize;

   public:
      SchemeWeakLayer();
      SchemeWeakLayer(double const *source, int sourceSize);
      ~SchemeWeakLayer();

      double Get(int index) const;
      double operator[](int index) const;
      int GetLength() const;
   };
}

#endif
