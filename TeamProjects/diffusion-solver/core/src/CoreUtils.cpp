#include <cmath>
#include <algorithm>
#include "CoreUtils.hpp"

namespace diffusioncore { 
namespace utils {

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