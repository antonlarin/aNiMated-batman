#ifndef ImplicitSchemeSolver_H
#define ImplicitSchemeSolver_H

#include "CoreGlobal.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
   class EXPORT_API ImplicitSchemeSolver : public SchemeSolver {
   private:
      std::shared_ptr<double> u1GridPtr;
      std::shared_ptr<double> u2GridPtr;
   
   public:
      ImplicitSchemeSolver();
      ~ImplicitSchemeSolver();
   
   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual void CheckParametersOverride(SchemeTask task);

   };
}

#endif