#include <cassert>
#include <stdexcept>
#include "SchemeSolver.hpp"
using namespace diffusioncore;

SchemeSolver::SchemeSolver() {
   mSolvingMode = SchemeSolvingMode::StableLayer;
   mTask = std::make_shared<SchemeTask>();
}

SchemeSolver::~SchemeSolver() { }


void SchemeSolver::SetSolvingMode(SchemeSolvingMode value) {
   mSolvingMode = value;
}
SchemeSolvingMode SchemeSolver::GetSolvingMode() const {
   return mSolvingMode;
}


void SchemeSolver::RegisterTask(std::shared_ptr<SchemeTask> task) {
   mTask = task;
}

void SchemeSolver::RegisterIterationCallback(
   SolverIterationCallback callback) {
   mIterationCallback = callback;
}

SchemeSolverResult SchemeSolver::Solve() {  
   if (!mTask)
      throw std::runtime_error("Task is not registred");

   auto task = mTask->Clone();
   CheckParameters(task);
   return SolveOverride(task);
}


void SchemeSolver::CheckParametersOverride(SchemeTask task) { }

bool SchemeSolver::UpdateIterationInfo(SchemeSolverIterationInfo& info) {
   if (mIterationCallback)
      return mIterationCallback(info);

   return true;
}

void SchemeSolver::CheckParameters(SchemeTask task) {
   CheckParametersOverride(task);
}
