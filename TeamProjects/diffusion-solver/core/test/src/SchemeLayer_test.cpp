#include <cmath>
#include <random>
#include <algorithm>
#include <gtest/gtest.h>
#include <diffusioncore>
#include "TestsCommon.hpp"
using namespace diffusioncore;

void TestInitialization(size_t length, int seed) {
   double* vector = new double[length];
   FillArrayRand(vector, length, seed);
   SchemeLayer layer(vector, length);

   for (size_t i = 0; i < length; ++i) {
      ASSERT_TRUE(layer[i] == vector[i]);
      ASSERT_TRUE(layer.Get(i) == vector[i]);
   }

   delete[] vector;   
}

double FindMaxDifference(double* src1, 
                         double* src2, 
                         size_t length,
                         size_t factor) {
   size_t i, j;
   double maxDiff = 0;
   for (i = 0, j = 0; i < length; i++, j += factor) {
      double diff = std::abs(src1[i] - src2[j]);
      maxDiff = std::max(maxDiff, diff);
   }
   return maxDiff;
}

void TestMaxDifference(size_t length,
                       size_t factor,
                       int seed1,
                       int seed2) {
   size_t length2 = (length - 1) * factor + 1;
   double* vector1 = new double[length];
   double* vector2 = new double[length2];

   FillArrayRand(vector1, length, seed1);
   SchemeLayer layer1(vector1, length);

   FillArrayRand(vector2, length2, seed2);
   SchemeLayer layer2(vector2, length2);

   double rightRes = FindMaxDifference(
      vector1, vector2, length, factor);
   double res = SchemeLayer::MaxDifference(
      layer1, layer2);

   delete[] vector2;
   delete[] vector1;

   ASSERT_TRUE(rightRes == res);
}

TEST(SchemeLayer, Initialization) {
   for (int i = 2; i <= 1000; ++i) {
      for (int j = 0; j <= 50; ++j)
         TestInitialization(i, j * 100);
   }
}

TEST(SchemeLayer, FindMaxDifference) {
   for (int f = 1; f <= 5; f++) {
      for (int i = 2; i <= 100; ++i) {
         for (int j = 0; j <= 50; ++j)
            TestMaxDifference(i, f, j * 20, j * 50);
      }
   }
}
