#include <cmath>
#include <stdexcept>
#include "CoreUtils.hpp"
#include "SchemeStatistic.hpp"
#include "SchemeSolverImplicit.hpp"
#include <omp.h>

using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeSolverImplicit::SchemeSolverImplicit() { }
SchemeSolverImplicit::~SchemeSolverImplicit() { }

SchemeSolverResult SchemeSolverImplicit::SolveOverride(SchemeTask task) {
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

   double* alpha1 = new double[n + 1];
   double* beta1 = new double[n + 1];
   double* alpha2 = new double[n + 1];
   double* beta2 = new double[n + 1];

   double* u1Grid = u1GridPtr.get();
   double* u2Grid = u2GridPtr.get();
   double* u1_curr_layer = u1Grid;
   double* u2_curr_layer = u2Grid;
   double* u1_prev_layer = u1Grid + n + 1;
   double* u2_prev_layer = u2Grid + n + 1;

   SchemeSolverIterationInfo iterInfo;
   for (int j = 0; j < m; j++) {
      if (solvingMode == SchemeSolvingMode::AllLayers) {
         u1_curr_layer = u1Grid + (n + 1)* (j + 1);
         u1_prev_layer = u1Grid + (n + 1)* j;
         u2_curr_layer = u2Grid + (n + 1)* (j + 1);
         u2_prev_layer = u2Grid + (n + 1)* j;
      }
      else if (solvingMode == SchemeSolvingMode::StableLayer) {
         PointerSwap(&u1_curr_layer, &u1_prev_layer);
         PointerSwap(&u2_curr_layer, &u2_prev_layer);
      }
	  // danger zone
	  omp_set_num_threads(2);
#pragma omp parallel sections if (task.GetIntervalsCount() >= 100)
	  {
#pragma omp section
		  {
			  double tmp1, tmp2;
			  alpha1[0] = 1;
			  beta1[0] = 0;
			  for (int i = 1; i < n; i++) {
				  tmp1 = 2 * mLambda1 + h * h / k - alpha1[i - 1] * mLambda1;
				  alpha1[i] = mLambda1 / tmp1;
				  tmp2 = h * h * (u1_prev_layer[i] / k + mRho - mMu * u1_prev_layer[i] +
					  mK * std::pow(u1_prev_layer[i], 2) / u2_prev_layer[i]);
				  beta1[i] = (tmp2 + beta1[i - 1] * mLambda1) / tmp1;
			  }
			  u1_curr_layer[n] = beta1[n - 1] / (-alpha1[n - 1] + 1);
			  for (int i = n; i > 0; i--)
				  u1_curr_layer[i - 1] = alpha1[i - 1] * u1_curr_layer[i] + beta1[i - 1];
			  maxDiffU1 = MaxDifference(u1_curr_layer, u1_prev_layer, n + 1);
		  }
#pragma omp section
		  {
			  double tmp1, tmp2;
			  alpha2[0] = 1;
			  beta2[0] = 0;
			  for (int i = 1; i < n; i++) {
				  tmp1 = 2 * mLambda2 + h * h / k - alpha2[i - 1] * mLambda2;
				  alpha2[i] = mLambda2 / tmp1;
				  tmp2 = h * h *(u2_prev_layer[i] / k - mNu * u2_prev_layer[i] +
					  mC * std::pow(u1_prev_layer[i], 2));
				  beta2[i] = (tmp2 + beta2[i - 1] * mLambda2) / tmp1;
			  }
			  u2_curr_layer[n] = beta2[n - 1] / (-alpha2[n - 1] + 1);
			  for (int i = n; i > 0; i--)
				  u2_curr_layer[i - 1] = alpha2[i - 1] * u2_curr_layer[i] + beta2[i - 1];
			  maxDiffU2 = MaxDifference(u2_curr_layer, u2_prev_layer, n + 1);
		  }
	  }
	  // end danger zone
	  iterInfo = SchemeSolverIterationInfo(layersCount,
                                           iterationsCount, 
                                           maxDiffU1, 
                                           maxDiffU2);   
      layersCount++;
      if (!UpdateIterationInfo(iterInfo))
         break;

      if (solvingMode == SchemeSolvingMode::StableLayer) {
         if (maxDiffU1 < mAccuracyU1 && maxDiffU2 < mAccuracyU2)
            break;
      }
   }

   delete[] alpha1;
   delete[] beta1;
   delete[] alpha2;
   delete[] beta2;

   if (solvingMode == SchemeSolvingMode::StableLayer)  { 
      iterationsCount = layersCount - 1;
      layersCount = 1;
      if (u1_curr_layer != u1Grid) {
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

void SchemeSolverImplicit::CheckParametersOverride(SchemeTask task) { }


void SchemeSolverImplicit::InitializeSchemeParameters(SchemeTask& task) {
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

void SchemeSolverImplicit::InitializeGrid(SchemeTask& task) {
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

