#include <cmath>
#include <cassert>
#include "SchemeSolver.hpp"
using namespace diffusioncore;

SchemeSolver::SchemeSolver() {
   // TODO: Set default parameters
}

SchemeSolver::~SchemeSolver() { }


void SchemeSolver::SetK(double value) {
   assert(value >= 0);
   mK = value;
   mIsSolved = false;
}
double SchemeSolver::GetK() const {
   return mK;
}

void SchemeSolver::SetC(double value) {
   assert(value >= 0);
   mC = value;
   mIsSolved = false;
}
double SchemeSolver::GetC() const {
   return mC;
}

void SchemeSolver::SetNu(double value) {
   assert(value >= 0);
   mNu = value;
   mIsSolved = false;
}
double SchemeSolver::GetNu() const {
   return mNu;
}

void SchemeSolver::SetMu(double value) {
   assert(value >= 0);
   mMu = value;
   mIsSolved = false;
} 
double SchemeSolver::GetMu() const {
   return mMu;
}

void SchemeSolver::SetRho(double value) {
   assert(value >= 0);
   mRho = value;
   mIsSolved = false;
}
double SchemeSolver::GetRho() const {
   return mRho;
}

void SchemeSolver::SetLambda1(double value) {
   assert(value > 0);
   mLambda1 = value;
   mIsSolved = false;
}
double SchemeSolver::GetLambda1() const {
   return mLambda1;
}

void SchemeSolver::SetLambda2(double value) {
   assert(value > 0);
   mLambda2 = value;
   mIsSolved = false;
}
double SchemeSolver::GetLambda2() const {
   return mLambda2;
}

void SchemeSolver::SetStepTime(double value) {
   assert(value > 0);
   mStepTime = value;
   mIsSolved = false;
}
double SchemeSolver::GetStepTime() const {
   return mStepTime;
}

void SchemeSolver::SetAccuracy(double value) {
   assert(value > 0);
   mAccuracy = value;
   mIsSolved = false;
}
double SchemeSolver::GetAccuracy() const {
   return mAccuracy;
}

void SchemeSolver::SetIntervalsCount(int value) {
   assert(value > 0);
   mIntervalsCount = value;
   mIsSolved = false;
}
int SchemeSolver::GetIntervalsCount() const {
   return mIntervalsCount;
}

void SchemeSolver::SetMaximumIterations(int value) {
   assert(value > 0);
   mMaximumIterations = value;
   mIsSolved = false;
}
int SchemeSolver::GetMaximumIterations() const {
   return mMaximumIterations;
}

void SchemeSolver::SetInitialConditions(SchemeInitialConditions value) {
   mInitialConditions = value;
   mIsSolved = false;
}
SchemeInitialConditions SchemeSolver::GetInitialConditions() const {
   return mInitialConditions;
}


bool SchemeSolver::IsSolved() const {
   return mIsSolved;
}

double SchemeSolver::GetMaximumTime() const {
   assert(IsSolved());
   return (GetIterationsCount() - 1) * GetStepTime();
}

int SchemeSolver::GetIterationsCount() const {
   assert(IsSolved());
   return mIterationsCount;
}


SchemeLayer SchemeSolver::GetSolutionU1(int index) {
   assert(IsSolved());
   return GetSolutionU1Override(index);
}

SchemeLayer SchemeSolver::GetSolutionU2(int index) {
   assert(IsSolved());
   return GetSolutionU2Override(index);
}

SchemeLayer SchemeSolver::GetSolutionU1(double t) {
   assert(IsSolved());
   assert(t >= 0);
   assert(t <= GetMaximumTime());

   int index = TimeToIndex(t);
   return GetSolutionU1Override(index);
}

SchemeLayer SchemeSolver::GetSolutionU2(double t) {
   assert(IsSolved());
   assert(t >= 0);
   assert(t <= GetMaximumTime());

   int index = TimeToIndex(t);
   return GetSolutionU2Override(index);
}


double SchemeSolver::GetSolutionU1Maximum() {
   assert(IsSolved());
   // TODO
   return 1;
}

double SchemeSolver::GetSolutionU1Minimum() {
   assert(IsSolved());
   // TODO
   return 0;
}

double SchemeSolver::GetSolutionU2Maximum() {
   assert(IsSolved());
   // TODO
   return 1;
}

double SchemeSolver::GetSolutionU2Minimum() {
   assert(IsSolved());
   // TODO
   return 0;
}


void SchemeSolver::Solve() {
   SolveOverride();
   mIsSolved = true;
}

int SchemeSolver::TimeToIndex(double t) {
   int index = static_cast<int>(std::round(t / GetStepTime()));
   int iterationsCount = GetIterationsCount();
   if (index >= iterationsCount)
      index = iterationsCount - 1;

   return index;
}
