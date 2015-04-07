#ifndef SCHEME_RESULTS_STORAGE_H
#define SCHEME_RESULTS_STORAGE_H

namespace diffusioncore {
   class EXPORT_API SchemeResultsStorage final {
   private:

   public:
      SchemeResultsStorage();
      ~SchemeResultsStorage();

      void AddResult(const SchemeSolverResult& result);
      void Drop();

      SchemeLayer GetLayerU1(int layerIndex) const;
      SchemeLayer GetLayerU2(int layerIndex) const;
      double TymeByIndex(int layerIndex) const;

      int GetLayersCount() const;
   };
}

#endif   