#include "HE_Integrator.h"
#include <new>
void HE_Integrate(HE_Task* task, HES_Status* stat)
{
	double x_start = task->GetLeftBound();
	double x_end = task->GetRightBound();
	double timeLim = task->GetTimeLimit();
	int n = task->GetXDim();
	int m = task->GetYDim();
	double* alpha, *beta;
	try{
		task->PrepareGridMem();
		alpha = new double[n + 1];
		beta = new double[n + 1];
	}
	catch (std::bad_alloc exp)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. Unable to allocate memory.");
	}

	double* const grid = task->GetGridPointer();
	double h = (x_end - x_start) / n;
	double k = timeLim / m;
	double tmp1, tmp2;

	for (int j = 0; j < m; j++)
	{
		double* curr_layer = grid + (n + 1)*(j + 1);
		double* prev_layer = grid + (n + 1)*j;

		alpha[0] = 0;
		beta[0] = curr_layer[0];
		for (int i = 1; i <= n; i++)
		{
			tmp1 = 2 + h*h / k - alpha[i - 1];
			alpha[i] = 1 / tmp1;
			tmp2 = h*h*(prev_layer[i] / k + task->RhsCalculate(x_start + h*i, k*(j + 1)));
			beta[i] = (tmp2 + beta[i - 1]) / tmp1;
		}
		for (int i = n; i > 1; i--)
			curr_layer[i - 1] = alpha[i - 1] * curr_layer[i] + beta[i - 1];
	}
	delete[] alpha;
	delete[] beta;
	stat->ErrCode = HES_ERRNO_NONE;
}