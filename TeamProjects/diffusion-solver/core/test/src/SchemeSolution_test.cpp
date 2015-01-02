#include <random>
#include <memory>
#include <gtest/gtest.h>
#include <diffusioncore>
#include "TestsCommon.hpp"
using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolution RandSchemeSolution(int testSize, int seed) {
   auto solution = MakeSharedArray(testSize);
   FillArrayRand(solution.get(), testSize, seed);
   double min = Min(solution.get(), testSize);
   double max = Max(solution.get(), testSize);
   return SchemeSolution(solution, testSize - 1, 1, 0.001, min, max);
}

SchemeSolution FillSchemeSolution(int testSize, double val) {
   auto solution = MakeSharedArray(testSize);
   FillArray(solution.get(), testSize, val);
   return SchemeSolution(solution, testSize - 1, 1, 0.001, val, val);
}

void CompareSchemeSolution(SchemeSolution& res, double val) {
   int layersCount = res.GetLayersCount();
   int intervalsCount = res.GetIntervalsCount();
   for (int i = 0; i < layersCount; ++i) {
      auto layer = res.GetLayer(i);
      for (int j = 0; j <= intervalsCount; ++j) {
         ASSERT_DOUBLE_EQ(val, layer[j]);
      }
   }
}

TEST(SchemeSolution, Initialization) {
   for (int i = 10; i < 100; ++i) {
      RandSchemeSolution(i, 0);
   }
}

TEST(SchemeSolution, DataAccessing) {
   double val = 1.5;
   int length = 1000;
   for (int i = 0; i < 50; ++i) {
      auto res = FillSchemeSolution(length, val * i);
      CompareSchemeSolution(res, val * i);
   }
}

TEST(SchemeSolution, MinMax) {
   double targetVal = 100;
   auto result = FillSchemeSolution(100, targetVal);
   ASSERT_DOUBLE_EQ(targetVal, result.GetMaximum());
   ASSERT_DOUBLE_EQ(targetVal, result.GetMinimum());
}

TEST(SchemeSolution, Copying) {
   double targetVal = 100;
   auto result = FillSchemeSolution(100, targetVal);
   auto resultPtr = new SchemeSolution(result);
   CompareSchemeSolution(*resultPtr, targetVal);
   delete resultPtr;
}
