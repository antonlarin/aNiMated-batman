#include <memory>
#include <functional>
#include <gtest/gtest.h>
#include <diffusioncore>
using namespace diffusioncore;

void SetSolverDefaultParameters(SchemeSolver& solver) {
   auto params = std::make_shared<SchemeTask>();
   params->SetC(0);
   params->SetK(0);
   params->SetMu(0);
   params->SetNu(0);
   params->SetRho(0);
   params->SetLambda1(1);
   params->SetLambda2(1);
   params->SetStepTime(0.00001);
   params->SetAccuracyU1(0.00001);
   params->SetAccuracyU2(0.00001);
   params->SetMaximumIterations(200000);

   SchemeLayer layer(std::vector<double>(200, 0));
   params->SetInitialLayers(layer, layer);

   solver.BindTask(params);
   solver.SetSolvingMode(SchemeSolvingMode::StableLayer);
}

TEST(ExplicitSchemeSolverClass, Initialization) {
   ExplicitSchemeSolver solver; 
   SetSolverDefaultParameters(solver);

   std::function<void(SchemeResult&)> callback = 
      [](SchemeResult& res) -> void {
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

TEST(ExplicitSchemeSolverClass, SolvingAbort) {
   ExplicitSchemeSolver solver; 
   SetSolverDefaultParameters(solver);

   std::function<void(SchemeResult&)> callback = 
      [](SchemeResult& res) -> void {
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

