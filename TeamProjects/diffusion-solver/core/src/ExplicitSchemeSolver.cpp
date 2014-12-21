#include "ExplicitSchemeSolver.hpp"
#include <cassert>
using namespace diffusioncore;

// helper class for shared_ptr
template<typename T>
struct array_deleter {
	void operator()(T const * p) { 
   	delete[] p; 
  	}
};


ExplicitSchemeSolver::ExplicitSchemeSolver()
{
	// We use shared_ptr instead c-style pointers
	// u1Grid = u2Grid = 0;
}
ExplicitSchemeSolver::~ExplicitSchemeSolver()
{
	// We don't need to release memory explicit 
	// because we use shared_ptr.

	// if (u1Grid)
	// 	delete[] u1Grid;
	// if (u2Grid)
	// 	delete[] u2Grid;
}
double ExplicitSchemeSolver::EvaluateStableTimeStep(int xGridDim)
{
	return 0.5 / (xGridDim*xGridDim);
}
void ExplicitSchemeSolver::SolveOverride(SolverCallback callback)
{
	int n = GetIntervalsCount();
	int m = GetMaximumIterations();
	mIterationsCount = GetMaximumIterations();
	double k = GetStepTime();
	double h = 1.0 / n;

	double mK= GetK();
	double mC = GetC();
	double mNu = GetNu();
	double mMu = GetMu();
	double mRho = GetRho();
	double mLambda1 = GetLambda1();
	double mLambda2 = GetLambda2();

	double* u1Grid;
	double* u2Grid;

	try{
		// use shared_pointer to hold allocated memory
		u1Grid = new double[(n + 1)*(m + 1)];
		u1GridPtr = std::shared_ptr<double>(u1Grid, array_deleter<double>());
		
		u2Grid = new double[(n + 1)*(m + 1)];
		u2GridPtr = std::shared_ptr<double>(u2Grid, array_deleter<double>());
	}
	catch (std::bad_alloc)
	{
		assert(0);
	}

	ISchemeInitialConditions* iConditions = GetInitialConditions();
	for (int i = 0; i <= n; i++)
	{
		u1Grid[i] = iConditions->GetValueU1(i*h);
		u2Grid[i] = iConditions->GetValueU2(i*h);
	}

	int layersCount = 1;
	if (k <= h*h / 2)
	{
		for (int j = 0; j < m; j++)
		{
			if (IsStoped())
				break;

			double* u1_curr_layer = u1Grid + (n + 1)*(j + 1);
			double* u1_prev_layer = u1Grid + (n + 1)*j;
			double* u2_curr_layer = u2Grid + (n + 1)*(j + 1);
			double* u2_prev_layer = u2Grid + (n + 1)*j;
			for (int i = 1; i < n; i++)
			{
				u1_curr_layer[i] = k*(mLambda1*(u1_prev_layer[i - 1] - 2 * u1_prev_layer[i]	+ u1_prev_layer[i + 1]) / (h * h)
					+ mK*std::pow(u1_prev_layer[i], 2)/u2_prev_layer[i] - mMu*u1_prev_layer[i] + mRho) + u1_prev_layer[i];

				u2_curr_layer[i] = k*(mLambda2*(u2_prev_layer[i - 1] - 2 * u2_prev_layer[i] + u2_prev_layer[i + 1]) / (h * h)
					+ mC*std::pow(u1_prev_layer[i], 2) - mNu*u2_prev_layer[i]) + u2_prev_layer[i];
			}
			u1_curr_layer[0] = (4 * u1_curr_layer[1] - u1_curr_layer[2]) / 3;
			u1_curr_layer[n] = (4 * u1_curr_layer[n-1] - u1_curr_layer[n-2]) / 3;

			u2_curr_layer[0] = (4 * u2_curr_layer[1] - u2_curr_layer[2]) / 3;
			u2_curr_layer[n] = (4 * u2_curr_layer[n - 1] - u2_curr_layer[n - 2]) / 3;
		
			layersCount++;
			if (IsStoped())
				break;
		}
	}
	else
	{
		assert(0);
	}

	double timeStep = k;
	int intervalsCount = n;
	SchemeResult res(
		u1GridPtr, 
		u2GridPtr, 
		intervalsCount, 
	   layersCount, 
	   timeStep);

	callback(res);	
}
