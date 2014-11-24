#include "D_TestTask.h"
D_TestTask::D_TestTask()
{}
D_TestTask::~D_TestTask()
{
	if (grid)
		delete[] grid;
}
void D_TestTask::SetGridDimentions(int n, int m, D_TaskStatus* stat)
{
	if (n > 2 && m > 2)
	{
		xDim = n;
		yDim = m;
		stat->errCode = DTS_ERRNO_NONE;
	}
	else
		stat->errCode = DTS_ERRNO_ERR;
}
void D_TestTask::PrepareGrid(D_TaskStatus* stat)
{
	if (grid)
		delete[] grid;
	grid = new double[(xDim + 1)*(yDim + 1)];
	for (int i = 0; i < (xDim + 1)*(yDim + 1); i++)
		grid[i] = 0;
	 
	double h = GetXGridStep();
	double k = GetYGridStep();

	for (int i = 0; i <= xDim; i++)
	{
		double x = x_left + i*h;

		grid[i] = 1 - x*x - y_left*y_left;
		grid[i + yDim*(xDim+1)] = 1 - x*x - y_right*y_right;
	}
	for (int j = 0; j <= yDim; j++)
	{
		double y = y_left + j*k;

		grid[j*(xDim + 1)] = 1 - x_left*x_left - y*y;
		grid[j*(xDim + 1) + xDim] = 1 - x_right*x_right - y*y;
	}
	stat->errCode = DTS_ERRNO_NONE;
}
double*  D_TestTask::GetGridPtr(D_TaskStatus* stat)
{
	if (grid)
		stat->errCode = DTS_ERRNO_NONE;
	else
		stat->errCode = DTS_ERRNO_ERR;
	return grid;
}
double D_TestTask::GetXGridStep()
{
	return (x_right-x_left) / xDim;
}
double D_TestTask::GetYGridStep()
{
	return (y_right - y_left) / yDim;
}
int D_TestTask::GetYGridDimention()
{
	return yDim;
}
int D_TestTask::GetXGridDimention()
{
	return xDim;
}