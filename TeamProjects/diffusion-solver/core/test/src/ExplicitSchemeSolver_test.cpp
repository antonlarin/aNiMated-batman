#include <memory>
#include <functional>
#include <gtest/gtest.h>
#include <diffusioncore>
using namespace diffusioncore;

void SetSolverDefaultParameters(SchemeSolver& solver) {
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
   task->SetMaximumIterations(200000);

   SchemeLayer layer(std::vector<double>(200, 0));
   task->SetInitialLayers(layer, layer);

   solver.BindTask(task);
   solver.SetSolvingMode(SchemeSolvingMode::StableLayer);
}

TEST(SchemeSolverExplicit, Initialization) {
   SchemeSolverExplicit solver; 
   SetSolverDefaultParameters(solver);

   std::function<void(SchemeSolverResult&)> callback = 
      [](SchemeSolverResult& res) -> void {
         SUCCEED();
      };
   std::function<void(std::exception&)> exCallback = 
      [](std::exception& ex) -> void {
         FAIL();
      };

   solver.SolveAsync(callback, exCallback);
   solver.SolveWait();

   solver.SolveAsync(callback, exCallback);
   solver.SolveWait();
}

TEST(SchemeSolverExplicit, SolvingAbort) {
   SchemeSolverExplicit solver; 
   SetSolverDefaultParameters(solver);

   std::function<void(SchemeSolverResult&)> callback = 
      [](SchemeSolverResult& res) -> void {
         SUCCEED();
      };
   std::function<void(std::exception&)> exCallback = 
      [](std::exception& ex) -> void {
         FAIL();
      };

   solver.SolveAsync(callback, exCallback);
   solver.SolveCancel();
   solver.SolveCancel();

   solver.SolveAsync(callback, exCallback);
   solver.SolveWait();

   solver.SolveAsync(callback, exCallback);
   solver.SolveCancel();
}

