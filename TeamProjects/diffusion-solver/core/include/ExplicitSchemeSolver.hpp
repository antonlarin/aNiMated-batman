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
		virtual SchemeResult SolveOverride();
		virtual void CheckParametersOverride();
		
		double GetLayersDifference(double* layer1, double* layer2, int layerSize);
	};
}

#endif