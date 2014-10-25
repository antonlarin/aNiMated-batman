#include "HE_Integrator.h"
#include <new>

void HE_Integrate(HE_Task* task, HES_Status* stat)
{
	double x_start = task->GetLeftBound();
	double x_end = task->GetRightBound();
	double timeLim = task->GetTimeLimit();
	int n = task->GetXDim();
	int m = task->GetYDim();
	try{
		task->PrepareGridMem();
	}
	catch (std::bad_alloc exp)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. Unable to allocate memory.");
		return;
	}
	double* const grid = task->GetGridPointer();
	double h = (x_end - x_start) / n;
	double k = timeLim / m;

	if ( k <= h*h / 2 )
	{
		for (int j = 0; j < m; j++)
		{
			double* curr_layer = grid + (n + 1)*(j + 1);
			double* prev_layer = grid + (n + 1)*j;
			for (int i = 1; i < n; i++)
				curr_layer[i] = k*((prev_layer[i - 1] - 2 * prev_layer[i] 
				+ prev_layer[i+1]) / (h * h) + task->RhsCalculate(x_start + i*h, j*k)) + prev_layer[i];
		}
		stat->ErrCode = HES_ERRNO_NONE;
	}
	else
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. Wrong grid dimensions.");
	}
}