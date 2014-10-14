#include "HE_Task.h"
HE_Task::HE_Task()
{}
void HE_Task::SetTimeLimit(double T, HES_Status* stat)
{
	timeLimit = T;
	stat->ErrCode = HES_ERRNO_NONE;
	if (timeLimit <= 0)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Time limit must be positive.");
	}
}
void HE_Task::SetDimensions(int dimX, int dimY, HES_Status* stat)
{
	n = dimX;
	m = dimY;
	stat->ErrCode = HES_ERRNO_NONE;
	if (n < 2 || m < 2)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Grid dimentions must be greater than 2.");
	}
}
void HE_Task::SetInitialConditions(const char* iCFunc, HES_Status* stat)
{
	initialConditions = MP_Parse(iCFunc, &errObj);
	int dims = MP_GetDimensions(initialConditions, &errObj);
	if (errObj.ErrCode != MP_ERRNO_NONE || dims > 1)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
		return;
	}
	varBuffers[0] = new char[MP_GetMaxIdentifierSize(&errObj)];
	MP_GetVariables(initialConditions, varBuffers, &errObj);
	if (varBuffers[0][0] != 'x' && dims != 0)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "The variable must be called x.");
		return;
	}
	delete[] varBuffers[0];
	MP_SetVariable(initialConditions, "x", 0, &errObj);
	stat->ErrCode = HES_ERRNO_NONE;
	if (errObj.ErrCode != MP_ERRNO_NONE)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
	}
}
void HE_Task::SetRightBoundaryCondition(const char* iRBCFunc, HES_Status* stat)
{
	rightBoundaryCondition = MP_Parse(iRBCFunc, &errObj);
	int dims = MP_GetDimensions(rightBoundaryCondition, &errObj);
	if (errObj.ErrCode != MP_ERRNO_NONE || dims > 1)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
		return;
	}
	varBuffers[0] = new char[MP_GetMaxIdentifierSize(&errObj)];
	MP_GetVariables(rightBoundaryCondition, varBuffers, &errObj);
	if (varBuffers[0][0] != 't' && dims != 0)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "The variable must be called t.");
		return;
	}
	delete[] varBuffers[0];
	MP_SetVariable(rightBoundaryCondition, "t", 0, &errObj);
	stat->ErrCode = HES_ERRNO_NONE;
	if (errObj.ErrCode != MP_ERRNO_NONE)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
	}
}
void HE_Task::SetLeftBoundaryCondition(const char* iLBCFunc, HES_Status* stat)
{
	leftBoundaryCondition = MP_Parse(iLBCFunc, &errObj);
	int dims = MP_GetDimensions(leftBoundaryCondition, &errObj);
	if (errObj.ErrCode != MP_ERRNO_NONE || dims > 1)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
		return;
	}
	varBuffers[0] = new char[MP_GetMaxIdentifierSize(&errObj)];
	MP_GetVariables(leftBoundaryCondition, varBuffers, &errObj);
	if (varBuffers[0][0] != 't' && dims != 0)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "The variable must be called t.");
		return;
	}
	delete[] varBuffers[0];
	MP_SetVariable(leftBoundaryCondition, "t", 0, &errObj);
	stat->ErrCode = HES_ERRNO_NONE;
	if (errObj.ErrCode != MP_ERRNO_NONE)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
	}
}
void HE_Task::SetRHSFunction(const char* rhsF, HES_Status* stat)
{
	rhsFunction = MP_Parse(rhsF, &errObj);
	int dims = MP_GetDimensions(rhsFunction, &errObj);
	if (errObj.ErrCode != MP_ERRNO_NONE || dims>2)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
		return;
	}
	varBuffers[0] = new char[MP_GetMaxIdentifierSize(&errObj)];
	varBuffers[1] = new char[MP_GetMaxIdentifierSize(&errObj)];
	MP_GetVariables(rhsFunction, varBuffers, &errObj);
	if ((varBuffers[1][0] != 't' && varBuffers[1][0] != 'x' || varBuffers[0][0] != 't' && varBuffers[0][0] != 'x') && dims == 2)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "The variables must be called x and t.");
		return;
	}
	else if (varBuffers[0][0] != 't' && varBuffers[0][0] != 'x' && dims != 0)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "The variables must be called x and t.");
		return;
	}
	delete[] varBuffers[0];
	delete[] varBuffers[1];
	MP_SetVariable(rhsFunction, "x", 0, &errObj);
	MP_SetVariable(rhsFunction, "t", 1, &errObj);
	stat->ErrCode = HES_ERRNO_NONE;
	if (errObj.ErrCode != MP_ERRNO_NONE)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, errObj.Message);
	}
}
void HE_Task::GetLayer(int index, double* layer, HES_Status* stat)
{
	if (gridPoints && index <= m)
	{
		for (int i = 0; i <= n; i++)
			layer[i] = gridPoints[(n + 1)*index + i];
		stat->ErrCode = HES_ERRNO_NONE;
	}
	else if (!gridPoints)
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. The layer is not filled yet.");
	}
	else
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. Index must be less than M+1.");
	}
}
double HE_Task::RhsCalculate(double x, double t)
{
	double args[] = { x, t };
	return MP_CalcUnsafe(rhsFunction, args);
}
void HE_Task::GetTable(double* table, HES_Status* stat)
{
	stat->ErrCode = HES_ERRNO_NONE;
	if (gridPoints)
		for (int i = 0; i < (m + 1)*(n + 1); i++)
			table[i] = gridPoints[i];
	else
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. The table is not filled yet.");
	}
}
void HE_Task::PrepareGridMem()
{
	double t, x;
	if (gridPoints)
		delete[] gridPoints;
	gridPoints = new double[(m + 1)*(n + 1)];
	for (int i = 0; i <= n; i++)
	{
		x = x_start + i*(x_end - x_start) / n;
		gridPoints[i] = MP_CalcUnsafe(initialConditions, &x);
	}
	for (int i = 1; i <= m; i++)
	{
		t = i * timeLimit / m;
		gridPoints[i*(n + 1)] = MP_CalcUnsafe(leftBoundaryCondition, &t);
		gridPoints[i*(n + 1) + n] = MP_CalcUnsafe(rightBoundaryCondition, &t);
	}
}
double* const HE_Task::GetGridPointer()
{
	return gridPoints;
}
int HE_Task::GetXDim()
{
	return n;
}
int HE_Task::GetYDim()
{
	return m;
}
double HE_Task::GetTimeLimit()
{
	return timeLimit;
}
double HE_Task::GetLeftBound()
{
	return x_start;
}
double HE_Task::GetRightBound()
{
	return x_end;
}
double HE_Task::GetSolution(int layer, int index, HES_Status* stat)
{
	if (gridPoints)
		return gridPoints[(n + 1)*layer + index];
	else
	{
		stat->ErrCode = HES_ERRNO_ERR;
		strcpy(stat->Message, "Error. The task is not decided yet.");
		return 0;
	}
}
int HE_Task::CheckTaskConditions()
{
	int check_result = 0;
	return check_result;
}
HE_Task::~HE_Task()
{
	if (gridPoints)
		delete[] gridPoints;
	if (rhsFunction)
		MP_Delete(&rhsFunction, &errObj);
	if (leftBoundaryCondition)
		MP_Delete(&leftBoundaryCondition, &errObj);
	if (rightBoundaryCondition)
		MP_Delete(&rightBoundaryCondition, &errObj);
}