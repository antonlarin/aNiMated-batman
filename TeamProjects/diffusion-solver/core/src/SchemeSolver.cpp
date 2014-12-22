#include <cmath>
#include <cassert>
#include <stdexcept>
#include "SchemeSolver.hpp"
using namespace diffusioncore;

SchemeSolver::SchemeSolver() {
   mIsSolving = false;
   mIsStop = true;
}

SchemeSolver::~SchemeSolver() { }


void SchemeSolver::SetK(double value) {
   CheckSolverThreadStatus();
   assert(value >= 0);
   mK = value;
}
double SchemeSolver::GetK() const {
   return mK;
}

void SchemeSolver::SetC(double value) {
   CheckSolverThreadStatus();
   assert(value >= 0);
   mC = value;
}
double SchemeSolver::GetC() const {
   return mC;
}

void SchemeSolver::SetNu(double value) {
   CheckSolverThreadStatus();
   assert(value >= 0);
   mNu = value;
}
double SchemeSolver::GetNu() const {
   return mNu;
}

void SchemeSolver::SetMu(double value) {
   CheckSolverThreadStatus();
   assert(value >= 0);
   mMu = value;
} 
double SchemeSolver::GetMu() const {
   return mMu;
}

void SchemeSolver::SetRho(double value) {
   CheckSolverThreadStatus();
   assert(value >= 0);
   mRho = value;
}
double SchemeSolver::GetRho() const {
   return mRho;
}

void SchemeSolver::SetLambda1(double value) {
   CheckSolverThreadStatus();
   assert(value > 0);
   mLambda1 = value;
}
double SchemeSolver::GetLambda1() const {
   return mLambda1;
}

void SchemeSolver::SetLambda2(double value) {
   CheckSolverThreadStatus();
   assert(value > 0);
   mLambda2 = value;
}
double SchemeSolver::GetLambda2() const {
   return mLambda2;
}

void SchemeSolver::SetStepTime(double value) {
   CheckSolverThreadStatus();
   assert(value > 0);
   mStepTime = value;
}
double SchemeSolver::GetStepTime() const {
   return mStepTime;
}

void SchemeSolver::SetAccuracy(double value) {
   CheckSolverThreadStatus();
   assert(value > 0);
   mAccuracy = value;
}
double SchemeSolver::GetAccuracy() const {
   return mAccuracy;
}

void SchemeSolver::SetIntervalsCount(int value) {
   CheckSolverThreadStatus();
   assert(value > 0);
   mIntervalsCount = value;
}
int SchemeSolver::GetIntervalsCount() const {
   return mIntervalsCount;
}

void SchemeSolver::SetMaximumIterations(int value) {
   CheckSolverThreadStatus();
   assert(value > 0);
   mMaximumIterations = value;
}
int SchemeSolver::GetMaximumIterations() const {
   return mMaximumIterations;
}

void SchemeSolver::SetSolvingMode(SchemeSolvingMode value) {
   CheckSolverThreadStatus();
   mSolvingMode = value;
}
SchemeSolvingMode SchemeSolver::GetSolvingMode() const {
   return mSolvingMode;
}

void SchemeSolver::SetInitialConditions(ISchemeInitialConditions* value) {
   CheckSolverThreadStatus();
   mInitialConditions = value;
}
ISchemeInitialConditions* SchemeSolver::GetInitialConditions() const {
   return mInitialConditions;
}


double SchemeSolver::GetMaximumTime() {
   CheckSolverThreadStatus();
   return (GetIterationsCount() - 1) * GetStepTime();
}

int SchemeSolver::GetIterationsCount() {
   CheckSolverThreadStatus();
   return mIterationsCount;
}


bool SchemeSolver::IsSolving() {
   mSolverMutex.lock();
   bool isSolving = mIsSolving;
   mSolverMutex.unlock();
   return isSolving;
}

void SchemeSolver::StopSolving() {
   mSolverMutex.lock();
   mIsStop = true;
   mSolverMutex.unlock();
   mSolverThread.join();
}


void SchemeSolver::BeginSolve(SolverCallback callback) {
   CheckSolverThreadStatus();

   mIsSolving = true;
   mIsStop = false;
   mSolverThread = std::thread(
      &SchemeSolver::SolveNewThread,
      this, callback);
}

bool SchemeSolver::IsStoped() {
   mSolverMutex.lock();
   bool isStoped = mIsStop;
   mSolverMutex.unlock(); 
   return isStoped;
}


void SchemeSolver::SolveNewThread(SolverCallback callback) {
   SolveOverride([&](SchemeResult& res) -> void {
      mSolverMutex.lock();
      mIsSolving = false;
      mSolverMutex.unlock(); 
      callback(res);
   });
}

void SchemeSolver::CheckSolverThreadStatus() {
   mSolverMutex.lock();
   bool isNotFinished = mIsSolving;
   mSolverMutex.unlock();

   if (isNotFinished)
      throw std::runtime_error("Solving in progress");
}
