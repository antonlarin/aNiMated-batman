#include <random>
#include "TestsCommon.hpp"

void FillArrayRand(double* dest, size_t length, int seed) {
   std::default_random_engine generator(seed);
   std::uniform_real_distribution<double> distribution(0.0, 1.0);
   for (size_t i = 0; i < length; ++i) 
      dest[i] = distribution(generator);
}

void FillArray(double* dest, size_t length, double value) {
   for (size_t i = 0; i < length; ++i) 
      dest[i] = value;
}

std::shared_ptr<double> MakeSharedArray(size_t size) {
   auto arrSource = new double[size];
   return std::shared_ptr<double>(arrSource, array_deleter<double>());  
}
