#include <thread>
#include <iostream>
#include <functional>
#include <gtest/gtest.h>
#include <diffusioncore>
using namespace diffusioncore;

void SetSolverDefaultParameters(SchemeSolver& solver) {
   solver.SetC(0);
   solver.SetK(0);
   solver.SetMu(0);
   solver.SetNu(0);
   solver.SetRho(0);
   solver.SetLambda1(1);
   solver.SetLambda2(1);
   solver.SetAccuracy(0.00001);
   solver.SetStepTime(0.00001);
   solver.SetIntervalsCount(200);
   solver.SetMaximumIterations(200000);
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

   solver.BeginSolve(callback, exCallback);
   solver.WaitSolve();

   solver.BeginSolve(callback, exCallback);
   solver.WaitSolve();
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

   solver.BeginSolve(callback, exCallback);
   solver.StopSolving();
   solver.StopSolving();

   solver.BeginSolve(callback, exCallback);
   solver.WaitSolve();

   solver.BeginSolve(callback, exCallback);
   solver.StopSolving();
}

