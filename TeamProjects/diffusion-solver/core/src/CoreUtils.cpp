#include <cmath>
#include <algorithm>
#include "CoreUtils.hpp"

namespace diffusioncore { 
namespace utils {

   double Max(const double* source, size_t size) {
      double* begin = const_cast<double*>(source);
      double* end = begin + size;
      double maxVal = *begin++;
      for (auto i = begin; i < end; i++)
         maxVal = std::max(maxVal, *i);
      return maxVal;
   }

   double Min(const double* source, size_t size) {
      double* begin = const_cast<double*>(source);
      double* end = begin + size;
      double maxVal = *begin++;
      for (auto i = begin; i < end; i++)
         maxVal = std::min(maxVal, *i);
      return maxVal;
   }

   double MaxDifference(const double* lhs, 
                        const double* rhs, 
                        size_t size) {
      double currentDiff;
      double maxDiff = 0;
      for (size_t i = 0; i < size; i++) {
         currentDiff = std::abs(lhs[i] - rhs[i]);
         maxDiff = std::max(maxDiff, currentDiff);
      }
      return maxDiff;
   }

}}