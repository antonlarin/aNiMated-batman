#ifndef SchemeSolution_H
#define SchemeSolution_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeLayer.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolution
   {
   private:
      std::shared_ptr<double> mSolution;
      size_t mSolutionSize;
      double mTimeStep;
      int mLayersCount;
      int mIntervalsCount;

   public:
      SchemeSolution();
      SchemeSolution(std::shared_ptr<double> solution, 
                     int intervalsCount, 
                     int layersCount,
                     double timeStep);
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