#ifndef CoreUtils_H
#define CoreUtils_H

#include <memory>
#include <cstdlib>

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

   template<typename T>
   void MoveMemory(T* source, size_t begin, 
                   size_t end, size_t distance) {
      for (size_t i = begin; i <= end; ++i)
         source[i] = source[i + distance];
   }

   double Max(const double* source, size_t size);
   double Min(const double* source, size_t size);
   double MaxDifference(
      const double* lhs, 
      const double* rhs, 
      size_t size);

   std::shared_ptr<double> CopyShared(
      const double* source, 
      size_t size);

}}

#endif
