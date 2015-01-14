#include <limits>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include "CoreUtils.hpp"
#include "SchemeGrid.hpp"

using namespace diffusioncore;
using namespace diffusioncore::utils;

SchemeGrid::SchemeGrid() {
   mIsInitialized = false;
}

SchemeGrid::SchemeGrid(int layersCount,
                       SchemeLayer& initialLayer,
                       SchemeSolverMode solverMode) {
   assert(layersCount > 0);

   mSolverMode = solverMode;
   mLayersCount = layersCount;
   mPointsCount = initialLayer.GetLength();

   InitializeGrid(initialLayer);
   InitializeLayers();

   mIsInitialized = true;
}

SchemeGrid::~SchemeGrid() { }


std::shared_ptr<double> SchemeGrid::Source() const {
   CheckIsInitialized();
   return mGrid;
}

void SchemeGrid::NextLayer() {
   CheckIsInitialized();
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

double* SchemeGrid::GetPrevousLayer() const {
   CheckIsInitialized();
   return mPrevLayer;
}

double* SchemeGrid::GetCurrentLayer() const {
   CheckIsInitialized();
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

   for (int i = 0; i < n; i++) {
      grid[i] = initialLayer[i];
   }
}

void SchemeGrid::InitializeLayers() {
   mPrevLayer = mGrid.get();
   mCurrLayer = mGrid.get() + mPointsCount;
}

void SchemeGrid::CheckIsInitialized() const {
   if (!mIsInitialized)
      throw std::runtime_error("Object is not initialized");
}