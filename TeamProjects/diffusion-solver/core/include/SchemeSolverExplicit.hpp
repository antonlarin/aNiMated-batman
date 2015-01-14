#ifndef SchemeSolverExplicit_H
#define SchemeSolverExplicit_H

#include "CoreGlobal.hpp"
#include "SchemeSolverCommon.hpp"

namespace diffusioncore {
   class EXPORT_API SchemeSolverExplicit : public SchemeSolverCommon {
   public:
      SchemeSolverExplicit();
      virtual ~SchemeSolverExplicit();
   
   protected:
      virtual void CheckParametersOverride(const SchemeTask& task);
      virtual void DoSolverIteration();

   };
}

#endif