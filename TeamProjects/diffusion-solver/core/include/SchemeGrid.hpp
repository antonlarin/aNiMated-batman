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

      double mMinValue;
      double mMaxValue;

   public:
      SchemeGrid(int layersCount,
                 SchemeLayer& initialLayer,
                 SchemeSolverMode solverMode);
      ~SchemeGrid();

      std::shared_ptr<double> Source();
      SchemeSolution Solution(SchemeTask& task);

      double* GetPrevousLayer();
      double* GetCurrentLayer();

      void NextLayer();
      void UpdateMinMaxValues(double value);

   private:
      void InitializeGrid(SchemeLayer& initialLayer);
      void InitializeLayers();

   };
}

#endif