#include "ExplicitSchemeSolver.hpp"
#include <cassert>
using namespace diffusioncore;

ExplicitSchemeSolver::ExplicitSchemeSolver()
{
	u1Grid = u2Grid = 0;
}
ExplicitSchemeSolver::~ExplicitSchemeSolver()
{
	if (u1Grid)
		delete[] u1Grid;
	if (u2Grid)
		delete u2Grid;
}
void ExplicitSchemeSolver::SolveOverride()
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

	try{
		u1Grid = new double[(n + 1)*(m + 1)];
		u2Grid = new double[(n + 1)*(m + 1)];
	}
	catch (std::bad_alloc)
	{
		assert(0);
	}

	SchemeInitialConditions iConditions = GetInitialConditions();
	for (int i = 0; i <= n; i++)
	{
		u1Grid[i] = std::sin(i*h)/2 + 1;//iConditions.GetValueU1(i*h);
		u2Grid[i] = std::cos(i*h)/2 + 1;//iConditions.GetValueU2(i*h);
	}

	if (k <= h*h / 2)
	{
		for (int j = 0; j < m; j++)
		{
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
			u1_curr_layer[0] = (4 * u1_curr_layer[1] + u1_curr_layer[2]) / 3;
			u1_curr_layer[n] = (4 * u1_curr_layer[n-1] + u1_curr_layer[n-2]) / 3;

			u2_curr_layer[0] = (4 * u2_curr_layer[1] + u2_curr_layer[2]) / 3;
			u2_curr_layer[n] = (4 * u2_curr_layer[n - 1] + u2_curr_layer[n - 2]) / 3;
		}
	}
	else
	{
		assert(0);
	}
}

SchemeLayer ExplicitSchemeSolver::GetSolutionU1Override(int index)
{

	return SchemeLayer(u1Grid + (GetIntervalsCount() + 1)*index, GetIntervalsCount() + 1);
}

SchemeLayer ExplicitSchemeSolver::GetSolutionU2Override(int index)
{
	return SchemeLayer(u1Grid + (GetIntervalsCount() + 1)*index, GetIntervalsCount() + 1);
}