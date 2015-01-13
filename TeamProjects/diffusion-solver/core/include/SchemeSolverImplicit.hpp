#ifndef SchemeSolverImplicit_H
#define SchemeSolverImplicit_H

#include "CoreGlobal.hpp"
#include "SchemeSolverCommon.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverImplicit : public SchemeSolverCommon {
   private:
      double* mAlpha;
      double* mBeta;

   public:
      SchemeSolverImplicit();
      virtual ~SchemeSolverImplicit();
   
   protected:
      virtual void PrepareSolverOverride();
      virtual void CleanupSolver(); 
      virtual void DoSolverIteration();

   };
}

#endif