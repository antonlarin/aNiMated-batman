#include <cassert>
#include "SchemeWeakLayer.hpp"

using namespace diffusioncore;

SchemeWeakLayer::SchemeWeakLayer() {
   mSource = nullptr;
   mSourceSize = 0;
}

SchemeWeakLayer::SchemeWeakLayer(double const *source, 
                                 int sourceSize) {
   mSource = source;
   mSourceSize = sourceSize;
}

SchemeWeakLayer::~SchemeWeakLayer() { }


double SchemeWeakLayer::Get(int index) const {
   assert(index >= 0);
   assert(index < mSourceSize);
   return mSource[index];
}

double SchemeWeakLayer::operator[](int index) const {
   return Get(index);
}

int SchemeWeakLayer::GetLength() const {
   return mSourceSize;
}