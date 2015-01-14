#ifndef SchemeGrid_H
#define SchemeGrid_H

#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeLayer.hpp"
#include "SchemeSolution.hpp"
#include "SchemeSolverMode.hpp"

namespace diffusioncore {
   class SchemeGrid final {
   private:
      SharedVector mGrid;
      SchemeSolverMode mSolverMode;
      int mPointsCount;
      int mLayersCount;

      double* mPrevLayer;
      double* mCurrLayer;

      bool mIsInitialized;

   public:
      SchemeGrid();
      SchemeGrid(int layersCount,
                 SchemeLayer& initialLayer,
                 SchemeSolverMode solverMode);
      ~SchemeGrid();

      SharedVector Source() const;
      double* GetPrevousLayer() const;
      double* GetCurrentLayer() const;

      void NextLayer();

   private:
      void InitializeGrid(SchemeLayer& initialLayer);
      void InitializeLayers();
      void CheckIsInitialized() const;

   };
}

#endif