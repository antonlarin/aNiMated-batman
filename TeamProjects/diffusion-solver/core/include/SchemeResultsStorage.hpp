#ifndef SCHEME_RESULTS_STORAGE_H
#define SCHEME_RESULTS_STORAGE_H

#include <list>
#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"
#include "SchemeSolverResult.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeResultsStorage final {
   private:
      int mTotalLayerCount;
      std::list<SchemeSolverResult> mResults;
      double mU1Minimum;
      double mU1Maximum;
      double mU2Minimum;
      double mU2Maximum;

   public:
      SchemeResultsStorage();
      ~SchemeResultsStorage();

      void AddResult(const SchemeSolverResult& result);
      void Drop();

      SchemeLayer GetLayerU1(int layerIndex) const;
      SchemeLayer GetLayerU2(int layerIndex) const;
      double GetU1Minimum() const;
      double GetU1Maximum() const;
      double GetU2Minimum() const;
      double GetU2Maximum() const;
      double TimeByIndex(int layerIndex) const;

      int GetLayersCount() const;

   private:
      SchemeSolverResult GetResultFromLayerIndex(int layer) const;
      int GetLayerIndexForResult(
         const SchemeSolverResult& res,
         int totalIndex) const;
      void ResetMinimaAndMaxima();

   };
}

#endif
