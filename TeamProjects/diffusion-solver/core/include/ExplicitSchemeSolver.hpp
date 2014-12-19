#ifndef ExplicitSchemeSolver_H
#define ExplicitSchemeSolver_H

#include "CoreGlobal.hpp"
#include "SchemeSolver.hpp"

namespace diffusioncore {
	class EXPORT_API ExplicitSchemeSolver : public SchemeSolver {
	public:
		ExplicitSchemeSolver();
		~ExplicitSchemeSolver();
		double EvaluateStableTimeStep(int xGridDim);
	private:
		double *u1Grid, *u2Grid;
	protected:
		virtual void SolveOverride();
		virtual SchemeLayer GetSolutionU1Override(int index);
		virtual SchemeLayer GetSolutionU2Override(int index);
	};
}

#endif