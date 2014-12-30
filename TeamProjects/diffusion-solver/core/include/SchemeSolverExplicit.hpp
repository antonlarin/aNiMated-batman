#ifndef SchemeSolverExplicit_H
#define SchemeSolverExplicit_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverExplicit : public SchemeSolver {
   private:
      std::shared_ptr<double> u1GridPtr;
      std::shared_ptr<double> u2GridPtr;
   
   public:
      SchemeSolverExplicit();
      ~SchemeSolverExplicit();
   
      double EvaluateStableTimeStep(int xGridDim);

   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual void CheckParametersOverride(SchemeTask task);
      
   };
}

#endif