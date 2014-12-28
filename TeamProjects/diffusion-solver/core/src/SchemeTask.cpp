#include <vector>
#include <cassert>
#include "SchemeTask.hpp"
using namespace diffusioncore;

SchemeTask::SchemeTask() {
   mC = 0;
   mK = 0;
   mMu = 0;
   mNu = 0;
   mRho = 0;
   mLambda1 = 1;
   mLambda2 = 1;
   mAccuracyU1 = 0.001;
   mAccuracyU2 = 0.001;
   mStepTime = 0.00001;
   mIntervalsCount = 100;
   mMaximumIterations = 1000;

   std::vector<double> defaultValues(mIntervalsCount + 1, 1);
   mStartLayerU1 = SchemeLayer(defaultValues);
   mStartLayerU2 = SchemeLayer(defaultValues);
}

SchemeTask::~SchemeTask() { }


void SchemeTask::SetK(double value) {
   assert(value >= 0);
   mK = value;
}
double SchemeTask::GetK() const {
   return mK;
}

void SchemeTask::SetC(double value) {
   assert(value >= 0);
   mC = value;
}
double SchemeTask::GetC() const {
   return mC;
}

void SchemeTask::SetNu(double value) {
   assert(value >= 0);
   mNu = value;
}
double SchemeTask::GetNu() const {
   return mNu;
}

void SchemeTask::SetMu(double value) {
   assert(value >= 0);
   mMu = value;
} 
double SchemeTask::GetMu() const {
   return mMu;
}

void SchemeTask::SetRho(double value) {
   assert(value >= 0);
   mRho = value;
}
double SchemeTask::GetRho() const {
   return mRho;
}

void SchemeTask::SetLambda1(double value) {
   assert(value > 0);
   mLambda1 = value;
}
double SchemeTask::GetLambda1() const {
   return mLambda1;
}

void SchemeTask::SetLambda2(double value) {
   assert(value > 0);
   mLambda2 = value;
}
double SchemeTask::GetLambda2() const {
   return mLambda2;
}

void SchemeTask::SetStepTime(double value) {
   assert(value > 0);
   mStepTime = value;
}
double SchemeTask::GetStepTime() const {
   return mStepTime;
}

void SchemeTask::SetAccuracyU1(double value) {
   assert(value > 0);
   mAccuracyU1 = value;
}
double SchemeTask::GetAccuracyU1() const {
   return mAccuracyU1;
}

void SchemeTask::SetAccuracyU2(double value) {
   assert(value > 0);
   mAccuracyU2 = value;
}
double SchemeTask::GetAccuracyU2() const {
   return mAccuracyU2;
}

void SchemeTask::SetMaximumIterations(int value) {
   assert(value > 0);
   mMaximumIterations = value;
}


double SchemeTask::GetSegmentBegin() const {
   return 0.0;
}

double SchemeTask::GetSegmentEnd() const {
   return 1.0;
}


int SchemeTask::GetMaximumIterations() const {
   return mMaximumIterations;
}

int SchemeTask::GetIntervalsCount() const {
   return mIntervalsCount;
}

SchemeLayer SchemeTask::GetInitialLayerU1() const {
   return mStartLayerU1;
}

SchemeLayer SchemeTask::GetInitialLayerU2() const {
   return mStartLayerU2;
}


void SchemeTask::SetInitialLayers(const SchemeLayer& u1,
                                        const SchemeLayer& u2) {
   assert(u1.GetLength() > 1);
   assert(u1.GetLength() == u2.GetLength());
   mStartLayerU1 = u1;
   mStartLayerU2 = u2;
   mIntervalsCount = u1.GetLength() - 1;
}

SchemeTask SchemeTask::Clone() {
   return *this;
}