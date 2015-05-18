#include <cassert>
#include <stdexcept>
#include "SchemeSolver.hpp"
using namespace diffusioncore;

SchemeSolver::SchemeSolver() {
   mSolverMode = SchemeSolverMode::StableLayer;
   mCurrentTask = std::make_shared<SchemeTask>();
   mIterationInfoUpdateStep = 10;
   mSaveLayerStep = 10;
}

SchemeSolver::~SchemeSolver() { }


void SchemeSolver::SetSolverMode(SchemeSolverMode value) {
   mSolverMode = value;
}
SchemeSolverMode SchemeSolver::GetSolverMode() const {
   return mSolverMode;
}

void SchemeSolver::SetCurrentTask(std::shared_ptr<SchemeTask> task) {
   mCurrentTask = task;
}
std::shared_ptr<SchemeTask> SchemeSolver::GetCurrentTask() const {
   return mCurrentTask;
}

void SchemeSolver::SetIterationCallback(SolverIterationCallback callback) {
   mIterationCallback = callback;
}
SolverIterationCallback SchemeSolver::GetIterationCallback() const {
   return mIterationCallback;
}

void SchemeSolver::SetIterationInfoUpdateStep(int callStep) {
   mIterationInfoUpdateStep = callStep;
}
int SchemeSolver::GetIterationInfoUpdateStep() const {
   return mIterationInfoUpdateStep;
}

void SchemeSolver::SetSaveLayerStep(int saveStep) {
   assert(saveStep > 0);
   mSaveLayerStep = saveStep;
}
int SchemeSolver::GetSaveLayerStep() const {
   return mSaveLayerStep;
}


SchemeSolverResult SchemeSolver::Solve() {
   SchemeTask task = mCurrentTask->Clone();
   CheckParameters(task);
   return SolveOverride(task);
}


void SchemeSolver::CheckParametersOverride(SchemeTask task) {
   SchemeLayer layerU1 = task.GetInitialLayerU1();
   SchemeLayer layerU2 = task.GetInitialLayerU2();
   if (!layerU1.CheckIsPositive() ||
       !layerU2.CheckIsPositive())
      throw std::runtime_error("Initial layer has negative value");
}

bool SchemeSolver::UpdateIterationInfo(SchemeSolverIterationInfo& info) {
   if (mIterationCallback)
      return mIterationCallback(info);

   return true;
}

void SchemeSolver::CheckParameters(SchemeTask task) {
   CheckParametersOverride(task);
}
