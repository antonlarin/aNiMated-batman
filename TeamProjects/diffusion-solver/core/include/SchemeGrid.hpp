#ifndef SchemeGrid_H
#define SchemeGrid_H

#include <vector>
#include "CoreUtils.hpp"
#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeLayer.hpp"
#include "SchemeSolver.hpp"
#include "SchemeSolution.hpp"
#include "SchemeWeakLayer.hpp"
#include "SchemeSolverMode.hpp"

namespace diffusioncore {
   class SchemeGrid final {
   private:
      std::unique_ptr<double, utils::array_deleter<double>> mPrevLayer;
      std::unique_ptr<double, utils::array_deleter<double>> mCurrLayer;
      std::vector<SchemeLayer> mLayers;

      SchemeSolverMode mSolverMode;
      int mPointsCount;
      int mLayersMaxCount;
      int mLayersCounter;
      int mSaveLayerStep;
      int mSwapCounter;
      size_t mRawGridSize;

      double mMaxValue;
      double mMinValue;

   public:
      SchemeGrid(const SchemeTask& task,
                 const SchemeLayer& initialLayer,
                 const SchemeSolver* solver);
      ~SchemeGrid();

      void UpdateMinMaxValues(double value);
      double* GetPrevousLayer() const;
      double* GetCurrentLayer() const;
      double GetMinValue() const;
      double GetMaxValue() const;

      void NextLayer();

      SchemeSolution BuildSolution();

   private:
      int CalculateLayersCount(const SchemeTask& task) const;
      void InitializeGrid(const SchemeLayer& initialLayer);
      void SaveCurrentLayer();

   };
}

#endif
