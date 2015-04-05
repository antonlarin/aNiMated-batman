#include <cmath>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include "CoreUtils.hpp"
#include "SchemeLayer.hpp"
using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeLayer::SchemeLayer() {
   mLength = 0;
}

SchemeLayer::SchemeLayer(const std::vector<double>& v) {
   Initialize(v.data(), (int)v.size());
}

SchemeLayer::SchemeLayer(const double* v, int length) {
   assert(length > 0);
   Initialize(v, length);
}

SchemeLayer::~SchemeLayer() { }


double* SchemeLayer::Raw() const {
   return mHolder.get();
}

double SchemeLayer::Get(int index) const {
   assert(index < mLength);
   assert(index >= 0);
   return mHolder.get()[index];
}

double SchemeLayer::operator[] (int index) const {
   return Get(index);
}

SchemeWeakLayer SchemeLayer::Weak() const {
   return SchemeWeakLayer(mHolder.get(), mLength);
}

int SchemeLayer::GetLength() const {
   return mLength;
}

double SchemeLayer::GetMinValue() const {
   return mMinValue;
}

double SchemeLayer::GetMaxValue() const {
   return mMaxValue;
}

bool SchemeLayer::CheckIsPositive() const {
   return GetMinValue() >= 0;
}


void SchemeLayer::Initialize(const double* v, int length) {
   mLength = length;
   double* dest = new double[length];
   memcpy(dest, v, length * sizeof(double));
   mHolder = SharedVector(
      dest, array_deleter<double>());

   mMinValue = Min(v, length);
   mMaxValue = Max(v, length);
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
