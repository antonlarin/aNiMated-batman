#include <cmath>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include "SchemeLayer.hpp"
using namespace diffusioncore;

SchemeLayer::SchemeLayer(const double* v, int length) {
   assert(length > 0);

   double* vbegin = const_cast<double*>(v);
   double* vend = vbegin + length;
   mVector = std::vector<double>(vbegin, vend);
}

SchemeLayer::~SchemeLayer() { }


double SchemeLayer::Get(int index) const {
   return mVector[index];
}

double SchemeLayer::operator[] (int index) const {
   return mVector[index];
}

int SchemeLayer::GetLength() const {
   return static_cast<int>(mVector.size());
}


double SchemeLayer::MaxDifference(const SchemeLayer& lhs, 
                                  const SchemeLayer& rhs) {
   SchemeLayer& minLayer = const_cast<SchemeLayer&>(lhs);
   SchemeLayer& maxLayer = const_cast<SchemeLayer&>(rhs);
   if (lhs.GetLength() > rhs.GetLength()) {
      minLayer = rhs;
      maxLayer = lhs;
   }

   int minSize = minLayer.GetLength() - 1;
   int maxSize = maxLayer.GetLength() - 1;
   int overflow = maxSize % minSize;
   if (overflow > 0)
      throw std::runtime_error("Vectors are incompatible");

   int i, j;
   double maxDifference = 0;
   int step = maxSize / minSize;
   for (i = 0, j = 0; i <= minSize; i++, j += step) {
      double diff = std::abs(minLayer[i] - maxLayer[j]);
      maxDifference = std::max(maxDifference, diff);
   }

   return maxDifference;
}