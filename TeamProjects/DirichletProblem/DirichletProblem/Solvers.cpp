#include "Solvers.h"
#include <cmath>
void swap(double** a, double** b)
{
	double *c = *a;
	*a = *b;
	*b = c;
}
void SolveWithZeidel(D_TestTask* task, int Nmax, double eps, double& reachedEps, int &stepNum, D_TaskStatus& stat)
{
	int xDim = task->GetXGridDimention();
	int yDim = task->GetYGridDimention();
	double h = task->GetXGridStep();
	double k = task->GetYGridStep();
	task->PrepareGrid(&stat);
	double* const grid = task->GetGridPtr(&stat);
	double h2 = 1 / (h*h), k2 = 1 / (k*k);
	double a2 = -2 * (h2 + k2);
	double f = 4, eps_max, eps_cur, v_old, v_new;
	bool stop = false;
	stepNum = 0;

	while (!stop)
	{
		eps_max = 0;
		for (int j = 1; j < yDim; j++)
			for (int i = 1; i < xDim; i++)
			{
				v_old = grid[i + j*(xDim + 1)];
				v_new = h2*(grid[i + 1 + j*(xDim + 1)] + grid[i - 1 + j*(xDim + 1)])
					+ k2*(grid[i + (j + 1)*(xDim + 1)] + grid[i + (j - 1)*(xDim + 1)]);
				v_new += f;
				v_new /= -a2;
				eps_cur = abs(v_new - v_old);
				if (eps_cur > eps_max)
					eps_max = eps_cur;
				grid[i + j*(xDim + 1)] = v_new;
			}
		stepNum++;
		if (eps_max < eps || stepNum >= Nmax)
			stop = true;
	}
	reachedEps = eps_max;
}
void SolveWithJacoby(D_TestTask* task, int Nmax, double eps, double& reachedEps, int &stepNum, D_TaskStatus& stat)
{
	int xDim = task->GetXGridDimention();
	int yDim = task->GetYGridDimention();
	double h = task->GetXGridStep();
	double k = task->GetYGridStep();
	task->PrepareGrid(&stat);
	double* const grid = task->GetGridPtr(&stat);
	double h2 = 1 / (h*h), k2 = 1 / (k*k);
	double a2 = -2 * (h2 + k2);
	double f = 4, eps_max, eps_cur, v_old, v_new;
	bool stop = false;
	stepNum = 0;

	double *tmpGrid2 = new double[(xDim + 1)*(yDim + 1)];
	double *tmpGrid1 = grid, *mem = tmpGrid2;

	for (int i = 0; i <= xDim; i++)
	{
		tmpGrid2[i] = grid[i];
		tmpGrid2[i + yDim*(xDim + 1)] = grid[i + yDim*(xDim + 1)];
	}
	for (int j = 0; j <= yDim; j++)
	{
		tmpGrid2[j*(xDim + 1)] = grid[j*(xDim + 1)];
		tmpGrid2[j*(xDim + 1) + xDim] = grid[j*(xDim + 1) + xDim];
	}
	
	while (!stop)
	{
		eps_max = 0;
		for (int j = 1; j < yDim; j++)
			for (int i = 1; i < xDim; i++)
			{
				v_old = tmpGrid1[i + j*(xDim + 1)];
				v_new = h2*(tmpGrid1[i + 1 + j*(xDim + 1)] + tmpGrid1[i - 1 + j*(xDim + 1)])
					+ k2*(tmpGrid1[i + (j + 1)*(xDim + 1)] + tmpGrid1[i + (j - 1)*(xDim + 1)]);
				v_new += f;
				v_new /= -a2;
				eps_cur = abs(v_new - v_old);
				if (eps_cur > eps_max)
					eps_max = eps_cur;
				tmpGrid2[i + j*(xDim + 1)] = v_new;
			}
		stepNum++;

		swap(&tmpGrid1, &tmpGrid2);

		if (eps_max < eps || stepNum >= Nmax)
			stop = true;
	}

	if (tmpGrid1 != grid)
		for (int i = 0; i < (xDim+1)*(yDim+1); i++)
			grid[i] = tmpGrid1[i];
	reachedEps = eps_max;
	delete[] mem;
}
void SolveWithURelax(D_TestTask* task, int Nmax, double eps, double& reachedEps, int &stepNum, double omega, D_TaskStatus& stat)
{
	int xDim = task->GetXGridDimention();
	int yDim = task->GetYGridDimention();
	double h = task->GetXGridStep();
	double k = task->GetYGridStep();
	task->PrepareGrid(&stat);
	double* const grid = task->GetGridPtr(&stat);
	double h2 = 1 / (h*h), k2 = 1 / (k*k);
	double a2 = -2*(h2 + k2);
	double f = 4, eps_max, eps_cur, v_old, v_new;
	bool stop = false;
	stepNum = 0;

	while (!stop)
	{
		eps_max = 0;
		for (int j = 1; j < yDim; j++)
			for (int i = 1; i < xDim; i++)
			{
				v_old = grid[i + j*(xDim + 1)];
				v_new = omega*(h2*(grid[i + 1 + j*(xDim + 1)] + grid[i - 1 + j*(xDim + 1)]) 
					+ k2*(grid[i + (j + 1)*(xDim + 1)] + grid[i + (j-1)*(xDim + 1)]));
				v_new += (1 - omega)*(-a2)*grid[i + j*(xDim + 1)] + omega*f;
				v_new /= -a2;
				eps_cur = abs(v_new - v_old);
				if (eps_cur > eps_max)
					eps_max = eps_cur;
				grid[i + j*(xDim + 1)] = v_new;
			}
		stepNum++;
		if (eps_max < eps || stepNum >= Nmax)
			stop = true;
	}
	reachedEps = eps_max;
}