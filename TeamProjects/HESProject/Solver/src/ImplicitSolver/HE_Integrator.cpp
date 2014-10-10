#include "HE_Integrator.h"

void HE_Integrate(HE_Task* task, HES_Status* stat)
{
	double x_start = task->GetLeftBound();
	double x_end = task->GetRightBound();
	double timeLim = task->GetTimeLimit();
	int n = task->GetXDim();
	int m = task->GetYDim();
	task->PrepareGridMem();
	double* const grid = task->GetGridPointer();

	if (!task->CheckTaskConditions())
	{
		double* alpha = new double[n + 1];
		double* beta = new double[n + 1];
		double h = (x_end - x_start) / n;
		double k = timeLim / m;
		double tmp1, tmp2;
		int p = n / 2;

		for (int j = 0; j < m; j++)
		{
			double* curr_layer = grid + (n+1)*(j + 1);
			double* prev_layer = grid + (n+1)*j;
			
			alpha[0] = 0;
			beta[0] = curr_layer[0];
			for (int i = 1; i <=n; i++)
			{
				tmp1 = 2 + h*h / k - alpha[i - 1];
				alpha[i] = 1 / tmp1;
				tmp2 = h*h*(prev_layer[i] / k + task->RhsCalculate(x_start + h*i, k*(j + 1)));
				beta[i] = (tmp2 + beta[i - 1]) / tmp1;
			}
			for (int i = n; i > 0; i--)
				curr_layer[i - 1] = alpha[i - 1] * curr_layer[i] + beta[i - 1];
		}
		delete[] alpha;
		delete[] beta;
		stat->ErrCode = HES_ERRNO_NONE;
	}
	else
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. Wrong task parameters.");
	}

}