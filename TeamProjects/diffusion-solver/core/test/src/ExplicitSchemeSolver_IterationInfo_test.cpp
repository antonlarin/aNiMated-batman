#include <memory>
#include <functional>
#include <gtest/gtest.h>
#include <diffusioncore>
using namespace diffusioncore;

void SetSolverDefaultParameters(SchemeSolver& solver, int maxIter) {
   auto task = std::make_shared<SchemeTask>();
   task->SetC(0);
   task->SetK(0);
   task->SetMu(0);
   task->SetNu(0);
   task->SetRho(0);
   task->SetLambda1(1);
   task->SetLambda2(1);
   task->SetStepTime(0.00001);
   task->SetAccuracyU1(0.00001);
   task->SetAccuracyU2(0.00001);
   task->SetMaximumIterations(maxIter);

   SchemeLayer layer(std::vector<double>(200, 0));
   task->SetInitialLayers(layer, layer);

   solver.RegisterTask(task);
   solver.SetSolverMode(SchemeSolverMode::AllLayers);
}

TEST(SchemeSolverExplicit_IterationInfo, IterationsCounting) {
   SchemeSolverExplicit solver;

   int targetItersCount = 1000; 
   SetSolverDefaultParameters(solver, targetItersCount);

   int iterCounter = 0;
   std::function<bool(SchemeSolverIterationInfo&)> iterCallback =
      [&](SchemeSolverIterationInfo& info) -> bool {
         iterCounter++;
         return true;
      };

   solver.RegisterIterationCallback(iterCallback);
   solver.Solve();

   ASSERT_EQ(targetItersCount, iterCounter);
}
