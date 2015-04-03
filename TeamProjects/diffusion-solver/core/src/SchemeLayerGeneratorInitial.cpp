#include <cmath>
#include "SchemeLayerGeneratorInitial.hpp"
using namespace diffusioncore;

const double PI_CONST = 3.14159265358979323846;
inline double EvaluateCos(double x, size_t index) {
   return std::cos(PI_CONST * x * index);
}

SchemeLayerGeneratorInitial::SchemeLayerGeneratorInitial() {
   mCoeff = std::vector<double>(1, 0);
   mIntervalsCount = 1;
}

SchemeLayerGeneratorInitial::~SchemeLayerGeneratorInitial() { }


void SchemeLayerGeneratorInitial::SetCoefficients(std::vector<double> coeff) {
   mCoeff = coeff;
}

void SchemeLayerGeneratorInitial::SetIntervalsCount(int intervalsCount) {
   mIntervalsCount = intervalsCount;
}

SchemeLayer SchemeLayerGeneratorInitial::Generate() {
   double dx = 1.0 / mIntervalsCount;
   std::vector<double> res(mIntervalsCount + 1, 0);
   for (int i = 0; i <= mIntervalsCount; ++i)
      res[i] = GetValue(dx * i);
   
   return res;
}

bool SchemeLayerGeneratorInitial::CheckParameters() {
   SchemeLayer genLayer = Generate();
   return genLayer.CheckIsPositive();
}


double SchemeLayerGeneratorInitial::GetValue(double x) {
   double sum = 0;
   for (size_t i = 0; i < mCoeff.size(); ++i)
      sum += mCoeff[i] * EvaluateCos(x, i);

   return sum;   
}