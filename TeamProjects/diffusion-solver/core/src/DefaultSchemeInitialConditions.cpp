#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <stdexcept>
#include "DefaultSchemeInitialConditions.hpp"
using namespace diffusioncore;

inline double EvaluateCos(double x, int index) {
   return std::cos(M_PI * x * index);
}

DefaultSchemeInitialConditions::DefaultSchemeInitialConditions(
   std::vector<double> coeffU1,
   std::vector<double> coeffU2) {
   assert(coeffU1.size() > 0);
   assert(coeffU1.size() == coeffU2.size());

   mCoeffU1 = coeffU1;
   mCoeffU2 = coeffU2;
}

DefaultSchemeInitialConditions::~DefaultSchemeInitialConditions() { }


bool DefaultSchemeInitialConditions::CheckConditions(double start, 
                                                     double end, 
                                                     int intervals) {
   assert(intervals > 0);
   assert(end > start);

   double h = (end - start) / intervals;
   double x = start;
   for (int i = 0; i <= intervals; ++i) {
      if (GetValueU1(x) <= 0 || GetValueU2(x) <= 0)
         return false;

      x += h;
   }

   return true;
}

double DefaultSchemeInitialConditions::GetValueU1(double x) {
   double sum = 0;
   for (int i = 0; i < mCoeffU1.size(); ++i)
      sum += mCoeffU1[i] * EvaluateCos(x, i);

   return sum;
}

double DefaultSchemeInitialConditions::GetValueU2(double x) {
   double sum = 0;
   for (int i = 0; i < mCoeffU2.size(); ++i)
      sum += mCoeffU2[i] * EvaluateCos(x, i);

   return sum;
}