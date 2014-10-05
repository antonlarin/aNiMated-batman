#ifndef HE_TASK_H
#define HE_TASK_H
#include "math_parser.h"
#include <string.h>
#include "HES_Solver.h"
#define HES_ERRNO_ERR   1
class HE_Task
{
protected:
	double timeLimit = 0;
	double x_start = 0, x_end = 1;
	double* gridPoints = 0;
	int m = 0, n = 0;
	MPFunction initialConditions, leftBoundaryCondition, rightBoundaryCondition, rhsFunction;
	MPErrObj errObj;
public:
	HE_Task();
	void SetTimeLimit(double T, HES_Status* stat);
	void SetDimensions(int dimX, int dimY, HES_Status* stat);
	void SetInitialConditions(const char* iCFunc, HES_Status* stat);
	void SetRightBoundaryCondition(const char* iRBCFunc, HES_Status* stat);
	void SetLeftBoundaryCondition(const char* iLBCFunc, HES_Status* stat);
	void SetRHSFunction(const char* rhsF, HES_Status* stat);
	void GetLayer(int index, double* layer, HES_Status* stat);
	double RhsCalculate(double x, double t);
	void GetTable(double* table, HES_Status* stat);
	void PrepareGridMem();
	double* const GetGridPointer();
	int GetXDim();
	int GetYDim();
	double GetTimeLimit();
	double GetLeftBound();
	double GetRightBound();
	int CheckTaskConditions();
	~HE_Task();
};
#endif