#include <random>
#include <memory>
#include <gtest/gtest.h>
#include <diffusioncore>
#include "TestsCommon.hpp"
using namespace diffusioncore;

SchemeResult RandSchemeResult(int testSize, int seed1, int seed2) {
   auto solutionU1 = MakeSharedArray(testSize);
   FillArrayRand(solutionU1.get(), testSize, seed1);

   auto solutionU2 = MakeSharedArray(testSize);
   FillArrayRand(solutionU2.get(), testSize, seed2);

   SchemeResult res(solutionU1, solutionU2, 
                    testSize - 1, 1, 0.001);
   return res;
}

SchemeResult FillSchemeResult(int testSize, double val1, double val2) {
   auto solutionU1 = MakeSharedArray(testSize);
   FillArray(solutionU1.get(), testSize, val1);

   auto solutionU2 = MakeSharedArray(testSize);
   FillArray(solutionU2.get(), testSize, val2);

   SchemeResult res(solutionU1, solutionU2, 
                    testSize - 1, 1, 0.001);
   return res;
}

void CompareSchemeResult(SchemeResult& res, double val1, double val2) {
   int layersCount = res.GetLayersCount();
   int intervalsCount = res.GetIntervalsCount();
   for (int i = 0; i < layersCount; ++i) {
      auto layerU1 = res.GetSolutionU1(i);
      auto layerU2 = res.GetSolutionU2(i);
      for (int j = 0; j <= intervalsCount; ++j) {
         ASSERT_DOUBLE_EQ(val1, layerU1[j]);
         ASSERT_DOUBLE_EQ(val2, layerU2[j]);
      }
   }
}

TEST(SchemeResult, Initialization) {
   for (int i = 10; i < 100; ++i) {
      RandSchemeResult(i, 0, 1);
   }
}

TEST(SchemeResult, DataAccessing) {
   double val1 = 1.5;
   double val2 = 2.5;
   int length = 1000;
   for (int i = 0; i < 50; ++i) {
      auto res = FillSchemeResult(length, val1 * i, val2 * i);
      CompareSchemeResult(res, val1 * i, val2 * i);
   }
}

TEST(SchemeResult, MinMax) {
   double targetVal = 100;
   auto result = FillSchemeResult(100, targetVal, targetVal);
   ASSERT_DOUBLE_EQ(targetVal, result.GetSolutionU1Maximum());
   ASSERT_DOUBLE_EQ(targetVal, result.GetSolutionU2Maximum());
   ASSERT_DOUBLE_EQ(targetVal, result.GetSolutionU1Minimum());
   ASSERT_DOUBLE_EQ(targetVal, result.GetSolutionU2Minimum());
}

TEST(SchemeResult, Copying) {
   double targetVal = 100;
   auto result = FillSchemeResult(100, targetVal, targetVal);
   auto resultPtr = new SchemeResult(result);
   CompareSchemeResult(*resultPtr, targetVal, targetVal);
   delete resultPtr;
}
