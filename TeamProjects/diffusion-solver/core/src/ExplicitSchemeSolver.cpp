#include "ExplicitSchemeSolver.hpp"
#include <cmath>
using namespace diffusioncore;

template<typename T>
void PointerSwap(T** a, T** b)
{
	T *c = *a;
	*a = *b;
	*b = c;
}
// helper class for shared_ptr
template<typename T>
struct array_deleter {
	void operator()(T const * p) {
		delete[] p;
	}
};


ExplicitSchemeSolver::ExplicitSchemeSolver(){}

ExplicitSchemeSolver::~ExplicitSchemeSolver(){}

double ExplicitSchemeSolver::GetLayersDifference(double* layer1, double* layer2, int layerSize)
{
	double maxDifference = 0, currentDifference;
	for (int i = 0; i < layerSize; i++)
	{
		currentDifference = std::abs(layer1[i] - layer2[i]);
		if (currentDifference > maxDifference)
			maxDifference = currentDifference;
	}
	return maxDifference;
}
double ExplicitSchemeSolver::EvaluateStableTimeStep(int xGridDim)
{
	return 0.5 / (xGridDim*xGridDim);
}
SchemeResult ExplicitSchemeSolver::SolveOverride()
{
	int n = GetIntervalsCount();
	int m = GetMaximumIterations();
	int solvingMode = GetSolvingMode();
	mIterationsCount = GetMaximumIterations();
	double k = GetStepTime();
	double h = 1.0 / n;

	double mK = GetK();
	double mC = GetC();
	double mNu = GetNu();
	double mMu = GetMu();
	double mRho = GetRho();
	double mLambda1 = GetLambda1();
	double mLambda2 = GetLambda2();

	double* u1Grid;
	double* u2Grid;
	double* u1_curr_layer;
	double* u1_prev_layer;
	double* u2_curr_layer;
	double* u2_prev_layer;

	try{
		if (solvingMode == AllLayers)
		{
			u1Grid = new double[(n + 1)*(m + 1)];
			u1GridPtr = std::shared_ptr<double>(u1Grid, array_deleter<double>());

			u2Grid = new double[(n + 1)*(m + 1)];
			u2GridPtr = std::shared_ptr<double>(u2Grid, array_deleter<double>());
		}
		else if (solvingMode == StableLayer)
		{
			u1Grid = new double[(n + 1) * 3];
			u1GridPtr = std::shared_ptr<double>(u1Grid, array_deleter<double>());

			u2Grid = new double[(n + 1) * 3];
			u2GridPtr = std::shared_ptr<double>(u2Grid, array_deleter<double>());
		}
		else
			throw std::runtime_error("Invalid solving mode");
	}
	catch (std::bad_alloc)
	{
		throw std::runtime_error("Not enought memory ");
	}

	auto iConditions = GetInitialConditions();
	for (int i = 0; i <= n; i++)
	{
		u1Grid[i] = iConditions->GetValueU1(i*h);
		u2Grid[i] = iConditions->GetValueU2(i*h);
	}
	u1_curr_layer = u1Grid;
	u1_prev_layer = u1Grid + n + 1;
	u2_curr_layer = u2Grid;
	u2_prev_layer = u2Grid + n + 1;

	int layersCount = 1;

	for (int j = 0; j < m; j++)
	{
		if (solvingMode == AllLayers)
		{
			u1_curr_layer = u1Grid + (n + 1)*(j + 1);
			u1_prev_layer = u1Grid + (n + 1)*j;
			u2_curr_layer = u2Grid + (n + 1)*(j + 1);
			u2_prev_layer = u2Grid + (n + 1)*j;
		}
		else
		{
			PointerSwap(&u1_curr_layer, &u1_prev_layer);
			PointerSwap(&u2_curr_layer, &u2_prev_layer);
		}
		for (int i = 1; i < n; i++)
		{
			u1_curr_layer[i] = k*(mLambda1*(u1_prev_layer[i - 1] - 2 * u1_prev_layer[i] + u1_prev_layer[i + 1]) / (h * h)
				+ mK*std::pow(u1_prev_layer[i], 2) / u2_prev_layer[i] - mMu*u1_prev_layer[i] + mRho) + u1_prev_layer[i];

			u2_curr_layer[i] = k*(mLambda2*(u2_prev_layer[i - 1] - 2 * u2_prev_layer[i] + u2_prev_layer[i + 1]) / (h * h)
				+ mC*std::pow(u1_prev_layer[i], 2) - mNu*u2_prev_layer[i]) + u2_prev_layer[i];
		}
		u1_curr_layer[0] = (4 * u1_curr_layer[1] - u1_curr_layer[2]) / 3;
		u1_curr_layer[n] = (4 * u1_curr_layer[n - 1] - u1_curr_layer[n - 2]) / 3;

		u2_curr_layer[0] = (4 * u2_curr_layer[1] - u2_curr_layer[2]) / 3;
		u2_curr_layer[n] = (4 * u2_curr_layer[n - 1] - u2_curr_layer[n - 2]) / 3;

		layersCount++;

		if (solvingMode == StableLayer && (j + 1) % 1000 == 0)
		{
			if (GetLayersDifference(u1_curr_layer, u1_prev_layer, n + 1) < GetAccuracy() &&
				GetLayersDifference(u2_curr_layer, u2_prev_layer, n + 1) < GetAccuracy())
				break;
		}
		if (IsStoped())
			break;
	}

	if (solvingMode == StableLayer)
	{
		mIterationsCount = layersCount - 1;
		layersCount = 1;
		if (u1_curr_layer != u1Grid)
			for (int i = 0; i <= n; i++)
			{
				u1Grid[i] = u1Grid[i + n + 1];
				u2Grid[i] = u2Grid[i + n + 1];
			}
	}

	double timeStep = k;
	int intervalsCount = n;
	SchemeResult res(
		u1GridPtr,
		u2GridPtr,
		intervalsCount,
		layersCount,
		timeStep);

	return res;
}

void ExplicitSchemeSolver::CheckParametersOverride() {
	double k = GetStepTime();
	int n = GetIntervalsCount();
	double h = 1.0 / n;

	if (k > h * h / 2) {
		throw std::runtime_error(
			"Incompatible intervlas "
			"count and time step");
	}
}