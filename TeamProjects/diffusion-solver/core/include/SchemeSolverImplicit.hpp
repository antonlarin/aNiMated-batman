#ifndef SchemeSolverImplicit_H
#define SchemeSolverImplicit_H

#include "CoreGlobal.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverImplicit : public SchemeSolver {
   private:
      std::shared_ptr<double> u1GridPtr;
      std::shared_ptr<double> u2GridPtr;
   
   public:
      SchemeSolverImplicit();
      ~SchemeSolverImplicit();
   
   protected:
      virtual SchemeSolverResult SolveOverride(SchemeTask task);
      virtual void CheckParametersOverride(SchemeTask task);

   };
}

#endif