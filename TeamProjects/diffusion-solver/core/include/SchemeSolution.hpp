#ifndef SchemeSolution_H
#define SchemeSolution_H

#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolution
   {
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
      SchemeSolution(SharedVector solutionSource, 
                     int intervalsCount, 
                     int layersCount,
                     double timeStep,
                     double minValue,
                     double maxValue);
      ~SchemeSolution();
      
      // TODO: Add friend setters for min/max values 

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