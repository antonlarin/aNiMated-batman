#ifndef SchemeLayer_H
#define SchemeLayer_H

#ifdef _MSC_VER
#pragma warning( disable: 4251 )
#endif

#include <vector>
#include <memory>
#include "CoreGlobal.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeLayer final {
   private:
      std::shared_ptr<double> mHolder;
      int mLength;

   public:
      static double MaxDifference(
         const SchemeLayer& lhs, 
         const SchemeLayer& rhs);

      SchemeLayer();
      
      SchemeLayer(const std::vector<double>& v);
      SchemeLayer(const double* v, int length);
      ~SchemeLayer();

      int GetLength() const;
      double Get(int index) const;
      double operator[] (int index) const;

   private: 
      void Initialize(const double* v, int length);

   };
}

#endif