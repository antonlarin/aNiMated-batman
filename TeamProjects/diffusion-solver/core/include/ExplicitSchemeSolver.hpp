#ifndef ExplicitSchemeSolver_H
#define ExplicitSchemeSolver_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
   class EXPORT_API ExplicitSchemeSolver : public SchemeSolver {
   private:
      std::shared_ptr<double> u1GridPtr;
      std::shared_ptr<double> u2GridPtr;
   
   public:
      ExplicitSchemeSolver();
      ~ExplicitSchemeSolver();
   
      double EvaluateStableTimeStep(int xGridDim);

   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual void CheckParametersOverride(SchemeTask task);
      
   };
}

#endif