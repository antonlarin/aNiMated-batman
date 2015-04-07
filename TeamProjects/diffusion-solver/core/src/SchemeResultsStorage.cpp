#include <stdexcept>
#include "SchemeResultsStorage.hpp"

using namespace diffusioncore;

SchemeResultsStorage::SchemeResultsStorage() {
   mTotalLayerCount = 0;
}

SchemeResultsStorage::~SchemeResultsStorage() { }


void SchemeResultsStorage::AddResult(const SchemeSolverResult& result) {
   mResults.push_back(result);
   mTotalLayerCount += result.GetLayersCount();
}

void SchemeResultsStorage::Drop() {
   mResults.clear();
   mTotalLayerCount = 0;
}


SchemeLayer SchemeResultsStorage::GetLayerU1(int layerIndex) const {
   SchemeSolverResult res = GetResultFromLayerIndex(layerIndex);
   int indexInRes = GetLayerIndexForResult(res, layerIndex);
   return res.GetSolutionU1().GetLayer(indexInRes);
}

SchemeLayer SchemeResultsStorage::GetLayerU2(int layerIndex) const {
   SchemeSolverResult res = GetResultFromLayerIndex(layerIndex);
   int indexInRes = GetLayerIndexForResult(res, layerIndex);
   return res.GetSolutionU2().GetLayer(indexInRes);
}

double SchemeResultsStorage::TymeByIndex(int layerIndex) const {
   int layersCounter = 0;
   double resTime = 0.0;
   for (const SchemeSolverResult& res : mResults) {
      layersCounter += res.GetLayersCount();
      resTime += res.GetTotalTime();
      if (layerIndex < layersCounter) {
         int indexInCurrentResult = GetLayerIndexForResult(res, layerIndex);
         double currentStepTime = res.GetTask().GetStepTime();
         resTime += indexInCurrentResult * currentStepTime;
         return resTime - res.GetTotalTime();
      }
   }
   throw std::invalid_argument("Can't find layer with specified index");
}

int SchemeResultsStorage::GetLayersCount() const {
   return mTotalLayerCount;
}


SchemeSolverResult SchemeResultsStorage::GetResultFromLayerIndex(
   int layerIndex) const {
   int layersCounter = 0;
   for (const SchemeSolverResult& res : mResults) {
      layersCounter += res.GetLayersCount();
      if (layerIndex < layersCounter)
         return res;
   }
   throw std::invalid_argument("Can't find layer with specified index");
}

inline int SchemeResultsStorage::GetLayerIndexForResult(
   const SchemeSolverResult& res, int totalIndex) const {
   return totalIndex - res.GetLayersCount() + 1;
}