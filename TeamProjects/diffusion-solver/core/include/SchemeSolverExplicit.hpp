#ifndef SchemeSolverExplicit_H
#define SchemeSolverExplicit_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverExplicit : public SchemeSolver {
   private:
      double mK;
      double mC;
      double mMu;
      double mNu;
      double mRho;
      double mLambda1;
      double mLambda2;
      double mAccuracyU1;
      double mAccuracyU2;
      
      std::shared_ptr<double> u1GridPtr;
      std::shared_ptr<double> u2GridPtr;

   public:
      SchemeSolverExplicit();
      ~SchemeSolverExplicit();
   
   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual void CheckParametersOverride(SchemeTask task);
      
   private:
      void InitializeSchemeParameters(SchemeTask& task);
      void InitializeGrid(SchemeTask& task); 

   };
}

#endif