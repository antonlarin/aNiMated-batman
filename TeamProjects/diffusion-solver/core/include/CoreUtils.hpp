#ifndef CoreUtils_H
#define CoreUtils_H

namespace diffusioncore { 
namespace utils {

   template<typename T>
   struct array_deleter {
      void operator()(T const * p) {
         delete[] p;
      }
   };

   template<typename T>
   void PointerSwap(T** a, T** b) {
      T *c = *a;
      *a = *b;
      *b = c;
   }

   double Max(const double* source, size_t size);
   double Min(const double* source, size_t size);
   double MaxDifference(
      const double* lhs, 
      const double* rhs, 
      size_t size);


}}

#endif