#include <cassert>
#include "CoreUtils.hpp"
#include "SchemeGrid.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeGrid::SchemeGrid(int layersCount,
                       SchemeLayer& initialLayer,
                       SchemeSolverMode solverMode) {
   assert(layersCount > 0);

   mSolverMode = solverMode;
   mLayersCount = layersCount;
   mPointsCount = initialLayer.GetLength();

   InitializeGrid(initialLayer);
   InitializeLayers();
}

SchemeGrid::~SchemeGrid() { }


std::shared_ptr<double> SchemeGrid::Source() {
   return mGrid;
}

SchemeSolution SchemeGrid::Solution(SchemeTask& task) {
   int layersCount;
   double k = task.GetStepTime();
   int n = task.GetIntervalsCount();
   std::shared_ptr<double> solution;
   switch (mSolverMode) {
      case SchemeSolverMode::AllLayers:
         layersCount = task.GetMaximumIterations() + 1;
         return SchemeSolution(mGrid, n, layersCount, k);

      case SchemeSolverMode::StableLayer:
         solution = CopyShared(mCurrLayer, n + 1);
         return SchemeSolution(solution, n, 1, k);

      default:
         throw std::runtime_error("Invalid solving mode");
   }   
}

void SchemeGrid::NextLayer() {
   switch (mSolverMode) {
      case SchemeSolverMode::AllLayers:
         mPrevLayer = mCurrLayer;
         mCurrLayer = mCurrLayer + mPointsCount;
         break;

      case SchemeSolverMode::StableLayer:
         PointerSwap(&mCurrLayer, &mPrevLayer);
         break;

      default:
         throw std::runtime_error("Invalid solving mode");
   }
}

double* SchemeGrid::GetPrevousLayer() {
   return mPrevLayer;
}

double* SchemeGrid::GetCurrentLayer() {
   return mCurrLayer;
}

void SchemeGrid::InitializeGrid(SchemeLayer& initialLayer) {
   int n = mPointsCount;
   int m = mLayersCount;

   size_t gridSize = 0;
   switch (mSolverMode) {
      case SchemeSolverMode::AllLayers:
         gridSize = n * m;
         break;

      case SchemeSolverMode::StableLayer:
         gridSize = 2 * n;
         break;

      default:
         throw std::runtime_error("Invalid solving mode");
   }

   double* grid = new double[gridSize];
   mGrid = std::shared_ptr<double>(grid, array_deleter<double>());

   for (int i = 0; i < n; i++)
      grid[i] = initialLayer[i];
}

void SchemeGrid::InitializeLayers() {
   mPrevLayer = mGrid.get();
   mCurrLayer = mGrid.get() + mPointsCount;
}
