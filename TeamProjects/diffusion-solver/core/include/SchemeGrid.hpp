#ifndef SchemeGrid_H
#define SchemeGrid_H

#include <memory>
#include "SchemeTask.hpp"
#include "SchemeLayer.hpp"
#include "SchemeSolution.hpp"
#include "SchemeSolverMode.hpp"

namespace diffusioncore {
   class SchemeGrid final {
   private:
      std::shared_ptr<double> mGrid;
      SchemeSolverMode mSolverMode;
      int mPointsCount;
      int mLayersCount;

      double* mPrevLayer;
      double* mCurrLayer;

   public:
      SchemeGrid(int layersCount,
                 SchemeLayer& initialLayer,
                 SchemeSolverMode solverMode);
      ~SchemeGrid();

      std::shared_ptr<double> Source() const;
      double* GetPrevousLayer() const;
      double* GetCurrentLayer() const;

      void NextLayer();

   private:
      void InitializeGrid(SchemeLayer& initialLayer);
      void InitializeLayers();

   };
}

#endif