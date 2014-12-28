#include <stdexcept>
#include <cmath>
#include "ImplicitSchemeSolver.hpp"
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


ImplicitSchemeSolver::ImplicitSchemeSolver(){}

ImplicitSchemeSolver::~ImplicitSchemeSolver(){}

double ImplicitSchemeSolver::GetLayersDifference(double* layer1, double* layer2, int layerSize)
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
SchemeResult ImplicitSchemeSolver::SolveOverride()
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
	double *alpha, *beta;

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
			u1Grid = new double[(n + 1) * 2];
			u1GridPtr = std::shared_ptr<double>(u1Grid, array_deleter<double>());

			u2Grid = new double[(n + 1) * 2];
			u2GridPtr = std::shared_ptr<double>(u2Grid, array_deleter<double>());
		}
		else
			throw std::runtime_error("Invalid solving mode");

		alpha = new double[n + 1];
		beta = new double[n + 1];
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
		////////////////solving zone
		double tmp1, tmp2;

		alpha[0] = 1;
		beta[0] = 0;
		for (int i = 1; i < n; i++)
		{
			tmp1 = 2 * mLambda1 + h*h / k - alpha[i - 1] * mLambda1;
			alpha[i] = mLambda1 / tmp1;
			tmp2 = h*h*(u1_prev_layer[i] / k + mRho - mMu*u1_prev_layer[i]
				+ mK*std::pow(u1_prev_layer[i], 2) / u2_prev_layer[i]);
			beta[i] = (tmp2 + beta[i - 1] * mLambda1) / tmp1;
		}
		u1_curr_layer[n] = beta[n - 1] / (-alpha[n - 1] + 1);
		for (int i = n; i > 0; i--)
			u1_curr_layer[i - 1] = alpha[i - 1] * u1_curr_layer[i] + beta[i - 1];

		alpha[0] = 1;
		beta[0] = 0;
		for (int i = 1; i < n; i++)
		{
			tmp1 = 2 * mLambda2 + h*h / k - alpha[i - 1] * mLambda2;
			alpha[i] = mLambda2 / tmp1;
			tmp2 = h*h*(u2_prev_layer[i] / k - mNu*u2_prev_layer[i]
				+ mC*std::pow(u1_prev_layer[i], 2));
			beta[i] = (tmp2 + beta[i - 1] * mLambda2) / tmp1;
		}
		u2_curr_layer[n] = beta[n - 1] / (-alpha[n - 1] + 1);
		for (int i = n; i > 0; i--)
			u2_curr_layer[i - 1] = alpha[i - 1] * u2_curr_layer[i] + beta[i - 1];

		////////////////////////////////
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

	delete[] alpha;
	delete[] beta;

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

void ImplicitSchemeSolver::CheckParametersOverride() {
}
