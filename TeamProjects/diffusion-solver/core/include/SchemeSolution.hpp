#ifndef SchemeSolution_H
#define SchemeSolution_H

#include "CoreGlobal.hpp"
#include "SchemeTask.hpp"
#include "SchemeLayer.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolution {

   private:
      SharedVector mSolution;
      size_t mSolutionSize;
      double mTimeStep;
      int mLayersCount;
      int mIntervalsCount;

      double mMinValue;
      double mMaxValue;

   public:
      SchemeSolution();
      SchemeSolution(const SchemeTask& task, 
                     SharedVector solutionSource, 
                     int layersCount,
                     double minValue, 
                     double maxValue);
      ~SchemeSolution();

      SchemeLayer GetLayer(int index) const;
      SchemeLayer GetLayerByTime(double time) const;
      SchemeLayer GetLastLayer() const;

      SchemeLayer operator[] (int index) const;

      double GetMaximum() const;
      double GetMinimum() const;
      int GetLayersCount() const;
      int GetIntervalsCount() const;

   private:
      int TimeToIndex(double t) const;
      void CheckIsInitialized() const;

   };

}

#endif