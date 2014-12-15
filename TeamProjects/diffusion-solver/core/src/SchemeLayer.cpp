#include <cassert>
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
   return mVector.size();
}


double SchemeLayer::MaxDifference(const SchemeLayer& lhs, 
                                  const SchemeLayer& rhs) {
   // TODO
   return 0;
}