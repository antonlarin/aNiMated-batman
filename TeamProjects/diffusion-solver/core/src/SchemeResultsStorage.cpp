#include <stdexcept>
#include <limits>
#include <algorithm>
#include "SchemeResultsStorage.hpp"

using namespace diffusioncore;
typedef std::numeric_limits<double> doubleLimits;

SchemeResultsStorage::SchemeResultsStorage() :
   mTotalLayerCount(0) {
   ResetMinimaAndMaxima();
}

SchemeResultsStorage::~SchemeResultsStorage() { }


void SchemeResultsStorage::AddResult(const SchemeSolverResult& result) {
   mResults.push_back(result);
   mTotalLayerCount += result.GetLayersCount();

   mU1Minimum = std::min(mU1Minimum, result.GetSolutionU1().GetMinimum());
   mU1Maximum = std::max(mU1Maximum, result.GetSolutionU1().GetMaximum());
   mU2Minimum = std::min(mU2Minimum, result.GetSolutionU2().GetMinimum());
   mU2Maximum = std::max(mU2Maximum, result.GetSolutionU2().GetMaximum());
}

void SchemeResultsStorage::Drop() {
   mResults.clear();
   mTotalLayerCount = 0;
   ResetMinimaAndMaxima();
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

double SchemeResultsStorage::GetU1Minimum() const {
   return mU1Minimum;
}

double SchemeResultsStorage::GetU1Maximum() const {
   return mU1Maximum;
}

double SchemeResultsStorage::GetU2Minimum() const {
   return mU2Minimum;
}

double SchemeResultsStorage::GetU2Maximum() const {
   return mU2Maximum;
}


double SchemeResultsStorage::TimeByIndex(int layerIndex) const {
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
   int resultIndex = totalIndex;
   for (const SchemeSolverResult& res : mResults) {
      int currentLayerCount = res.GetLayersCount();
      if (resultIndex < currentLayerCount)
         return resultIndex;
      else
         resultIndex -= currentLayerCount;
   }
   return -1;
}

void SchemeResultsStorage::ResetMinimaAndMaxima() {
   mU1Minimum = doubleLimits::infinity();
   mU1Maximum = -doubleLimits::infinity();
   mU2Minimum = doubleLimits::infinity();
   mU2Maximum = -doubleLimits::infinity();
}
