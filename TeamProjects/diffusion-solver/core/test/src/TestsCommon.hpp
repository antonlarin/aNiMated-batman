#ifndef TestsCommon_H
#define TestsCommon_H

#include <memory>

template<typename T>
struct array_deleter {
   void operator()(T const * p) {
      delete[] p;
   }
};

void FillArrayRand(double* dest, size_t length, int seed);
void FillArray(double* dest, size_t length, double value);

std::shared_ptr<double> MakeSharedArray(size_t size);

#endif