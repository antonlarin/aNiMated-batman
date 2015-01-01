#include <cmath>
#include <stdexcept>
#include "CoreUtils.hpp"
#include "SchemeStatistic.hpp"
#include "SchemeSolverExplicit.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolverExplicit::SchemeSolverExplicit() { }
SchemeSolverExplicit::~SchemeSolverExplicit() { }

SchemeSolverResult SchemeSolverExplicit::SolveOverride(SchemeTask task) {
   InitializeSchemeParameters(task);
   InitializeGrid(task);

   int n = task.GetIntervalsCount();
   int m = task.GetMaximumIterations();
   int iterationsCount = task.GetMaximumIterations();
   int layersCount = 1;
   double h = 1.0 / n;
   double k = task.GetStepTime();
   SchemeSolvingMode solvingMode = GetSolvingMode();

   double maxDiffU1 = mAccuracyU1;
   double maxDiffU2 = mAccuracyU2;

   double* u1Grid = u1GridPtr.get();
   double* u2Grid = u2GridPtr.get();
   double* u1_curr_layer = u1Grid;
   double* u2_curr_layer = u2Grid;
   double* u1_prev_layer = u1Grid + n + 1;
   double* u2_prev_layer = u2Grid + n + 1;

   SchemeSolverIterationInfo iterInfo;
   for (int j = 0; j < m; j++) {
      if (solvingMode == AllLayers) {
         u1_curr_layer = u1Grid + (n + 1) * (j + 1);
         u1_prev_layer = u1Grid + (n + 1) * j;
         u2_curr_layer = u2Grid + (n + 1) * (j + 1);
         u2_prev_layer = u2Grid + (n + 1) * j;
      }
      else {
         PointerSwap(&u1_curr_layer, &u1_prev_layer);
         PointerSwap(&u2_curr_layer, &u2_prev_layer);
      }

      for (int i = 1; i < n; i++) {
         u1_curr_layer[i] = k * (mLambda1 * (u1_prev_layer[i - 1] - 
                            2 * u1_prev_layer[i] + u1_prev_layer[i + 1]) / 
                            (h * h) + mK * std::pow(u1_prev_layer[i], 2) / 
                            u2_prev_layer[i] - mMu * u1_prev_layer[i] + mRho) + 
                            u1_prev_layer[i];

         u2_curr_layer[i] = k * (mLambda2 * (u2_prev_layer[i - 1] - 
                            2 * u2_prev_layer[i] + u2_prev_layer[i + 1]) / 
                            (h * h) + mC * std::pow(u1_prev_layer[i], 2) - 
                            mNu * u2_prev_layer[i]) + u2_prev_layer[i];
      }

      u1_curr_layer[0] = (4 * u1_curr_layer[1] - u1_curr_layer[2]) / 3;
      u1_curr_layer[n] = (4 * u1_curr_layer[n - 1] - u1_curr_layer[n - 2]) / 3;

      u2_curr_layer[0] = (4 * u2_curr_layer[1] - u2_curr_layer[2]) / 3;
      u2_curr_layer[n] = (4 * u2_curr_layer[n - 1] - u2_curr_layer[n - 2]) / 3;

      maxDiffU1 = MaxDifference(u1_curr_layer, u1_prev_layer, n + 1);
      maxDiffU2 = MaxDifference(u2_curr_layer, u2_prev_layer, n + 1);
      iterInfo = SchemeSolverIterationInfo(layersCount, 
                                           iterationsCount, 
                                           maxDiffU1, 
                                           maxDiffU2);      
      layersCount++;
      
      if (!UpdateIterationInfo(iterInfo))
         break;

      if (solvingMode == StableLayer) {
         if (maxDiffU1 < mAccuracyU1 && maxDiffU2 < mAccuracyU2)
            break;
      }
   }

   if (solvingMode == StableLayer) {
      iterationsCount = layersCount - 1;
      layersCount = 1;
      if (u1_curr_layer != u1Grid)  {
         MoveMemory(u1Grid, 0, n, n + 1);
         MoveMemory(u2Grid, 0, n, n + 1);
      }
   }

   SchemeSolution solutionU1(u1GridPtr, n, layersCount, k);
   SchemeSolution solutionU2(u2GridPtr, n, layersCount, k);
   SchemeStatistic statistic(iterationsCount, maxDiffU1, maxDiffU2);
   SchemeSolverResult result(solutionU1, solutionU2, statistic, task);
   return result;
}

void SchemeSolverExplicit::CheckParametersOverride(SchemeTask task) {
   double k = task.GetStepTime();
   int n = task.GetIntervalsCount();
   double h = 1.0 / n;

   if (k > h * h / 2)
      throw std::runtime_error("Incompatible intervlas count and time step");
}


void SchemeSolverExplicit::InitializeSchemeParameters(SchemeTask& task) {
   mK = task.GetK();
   mC = task.GetC();
   mMu = task.GetMu();
   mNu = task.GetNu();
   mRho = task.GetRho();
   mLambda1 = task.GetLambda1();
   mLambda2 = task.GetLambda2();
   mAccuracyU1 = task.GetAccuracyU1();
   mAccuracyU2 = task.GetAccuracyU2();
}

void SchemeSolverExplicit::InitializeGrid(SchemeTask& task) {
   int n = task.GetIntervalsCount();
   int m = task.GetMaximumIterations();
   SchemeSolvingMode solvingMode = GetSolvingMode();

   size_t gridSize = 0;
   switch (solvingMode) {
      case SchemeSolvingMode::AllLayers:
         gridSize = (n + 1) * (m + 1);
         break;

      case SchemeSolvingMode::StableLayer:
         gridSize = 2 * (n + 1);
         break;

      default:
         throw std::runtime_error("Invalid solving mode");
   }

   double* u1Grid;
   double* u2Grid;
   try {
      u1Grid = new double[gridSize];
      u1GridPtr = std::shared_ptr<double>(u1Grid, array_deleter<double>());
      
      u2Grid = new double[gridSize];
      u2GridPtr = std::shared_ptr<double>(u2Grid, array_deleter<double>());
   }
   catch (std::bad_alloc) {
      throw std::runtime_error("Not enought memory");
   }

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto initialLayerU2 = task.GetInitialLayerU2();
   for (int i = 0; i <= n; i++) {
      u1Grid[i] = initialLayerU1[i];
      u2Grid[i] = initialLayerU2[i];
   }
}

