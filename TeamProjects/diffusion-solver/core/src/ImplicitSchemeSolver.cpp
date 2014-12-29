#include <cmath>
#include <stdexcept>
#include "CoreUtils.hpp"
#include "ImplicitSchemeSolver.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;

ImplicitSchemeSolver::ImplicitSchemeSolver() { }
ImplicitSchemeSolver::~ImplicitSchemeSolver() { }

SchemeSolverResult ImplicitSchemeSolver::SolveOverride(SchemeTask task) {
   int n = task.GetIntervalsCount();
   int m = task.GetMaximumIterations();
   double h = 1.0 / n;
   double k = task.GetStepTime();

   int mIterationsCount = task.GetMaximumIterations();
   SchemeSolvingMode solvingMode = GetSolvingMode();

   double mK = task.GetK();
   double mC = task.GetC();
   double mMu = task.GetMu();
   double mNu = task.GetNu();
   double mRho = task.GetRho();
   double mLambda1 = task.GetLambda1();
   double mLambda2 = task.GetLambda2();
   double mAccuracyU1 = task.GetAccuracyU1();
   double mAccuracyU2 = task.GetAccuracyU2();

   double* u1Grid;
   double* u2Grid;
   double* u1_curr_layer;
   double* u1_prev_layer;
   double* u2_curr_layer;
   double* u2_prev_layer;
   double *alpha, *beta;

   try{
      if (solvingMode == AllLayers)
      {
         u1Grid = new double[(n + 1)*(m + 1)];
         u1GridPtr = std::shared_ptr<double>(u1Grid, array_deleter<double>());

         u2Grid = new double[(n + 1)*(m + 1)];
         u2GridPtr = std::shared_ptr<double>(u2Grid, array_deleter<double>());
      }
      else if (solvingMode == StableLayer)
      {
         u1Grid = new double[(n + 1) * 2];
         u1GridPtr = std::shared_ptr<double>(u1Grid, array_deleter<double>());

         u2Grid = new double[(n + 1) * 2];
         u2GridPtr = std::shared_ptr<double>(u2Grid, array_deleter<double>());
      }
      else
         throw std::runtime_error("Invalid solving mode");

      alpha = new double[n + 1];
      beta = new double[n + 1];
   }
   catch (std::bad_alloc)
   {
      throw std::runtime_error("Not enought memory ");
   }

   auto initialLayerU1 = task.GetInitialLayerU1();
   auto initialLayerU2 = task.GetInitialLayerU2();
   for (int i = 0; i <= n; i++)
   {
      u1Grid[i] = initialLayerU1[i];
      u2Grid[i] = initialLayerU2[i];
   }

   u1_curr_layer = u1Grid;
   u1_prev_layer = u1Grid + n + 1;
   u2_curr_layer = u2Grid;
   u2_prev_layer = u2Grid + n + 1;

   int layersCount = 1;
   for (int j = 0; j < m; j++)
   {
      if (solvingMode == AllLayers)
      {
         u1_curr_layer = u1Grid + (n + 1)*(j + 1);
         u1_prev_layer = u1Grid + (n + 1)*j;
         u2_curr_layer = u2Grid + (n + 1)*(j + 1);
         u2_prev_layer = u2Grid + (n + 1)*j;
      }
      else
      {
         PointerSwap(&u1_curr_layer, &u1_prev_layer);
         PointerSwap(&u2_curr_layer, &u2_prev_layer);
      }
      ////////////////solving zone
      double tmp1, tmp2;

      alpha[0] = 1;
      beta[0] = 0;
      for (int i = 1; i < n; i++)
      {
         tmp1 = 2 * mLambda1 + h*h / k - alpha[i - 1] * mLambda1;
         alpha[i] = mLambda1 / tmp1;
         tmp2 = h*h*(u1_prev_layer[i] / k + mRho - mMu*u1_prev_layer[i]
            + mK*std::pow(u1_prev_layer[i], 2) / u2_prev_layer[i]);
         beta[i] = (tmp2 + beta[i - 1] * mLambda1) / tmp1;
      }
      u1_curr_layer[n] = beta[n - 1] / (-alpha[n - 1] + 1);
      for (int i = n; i > 0; i--)
         u1_curr_layer[i - 1] = alpha[i - 1] * u1_curr_layer[i] + beta[i - 1];

      alpha[0] = 1;
      beta[0] = 0;
      for (int i = 1; i < n; i++)
      {
         tmp1 = 2 * mLambda2 + h*h / k - alpha[i - 1] * mLambda2;
         alpha[i] = mLambda2 / tmp1;
         tmp2 = h*h*(u2_prev_layer[i] / k - mNu*u2_prev_layer[i]
            + mC*std::pow(u1_prev_layer[i], 2));
         beta[i] = (tmp2 + beta[i - 1] * mLambda2) / tmp1;
      }
      u2_curr_layer[n] = beta[n - 1] / (-alpha[n - 1] + 1);
      for (int i = n; i > 0; i--)
         u2_curr_layer[i - 1] = alpha[i - 1] * u2_curr_layer[i] + beta[i - 1];

      ////////////////////////////////
      layersCount++;

      if (solvingMode == StableLayer && (j + 1) % 1000 == 0)
      {
         if (MaxDifference(u1_curr_layer, u1_prev_layer, n + 1) < mAccuracyU1 &&
            MaxDifference(u2_curr_layer, u2_prev_layer, n + 1) < mAccuracyU2)
            break;
      }
      if (IsStoped())
         break;
   }

   delete[] alpha;
   delete[] beta;

   if (solvingMode == StableLayer)
   {
      mIterationsCount = layersCount - 1;
      layersCount = 1;
      if (u1_curr_layer != u1Grid)
         for (int i = 0; i <= n; i++)
         {
            u1Grid[i] = u1Grid[i + n + 1];
            u2Grid[i] = u2Grid[i + n + 1];
         }
   }

   double timeStep = k;
   int intervalsCount = n;
   SchemeSolverResult res(
      u1GridPtr,
      u2GridPtr,
      task,
      mIterationsCount,
      layersCount);

   return res;
}

void ImplicitSchemeSolver::CheckParametersOverride(SchemeTask task) {
}
