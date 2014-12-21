#ifndef ExplicitSchemeSolver_H
#define ExplicitSchemeSolver_H

#include <memory>
#include "CoreGlobal.hpp"
#include "SchemeSolver.hpp"
#include "SchemeResult.hpp"

namespace diffusioncore {
	class EXPORT_API ExplicitSchemeSolver : public SchemeSolver {
	public:
		ExplicitSchemeSolver();
		~ExplicitSchemeSolver();
		double EvaluateStableTimeStep(int xGridDim);
	private:
		std::shared_ptr<double> u1GridPtr;
		std::shared_ptr<double> u2GridPtr;

	protected:
		virtual void SolveOverride(SolverCallback callback);
	};
}

#endif