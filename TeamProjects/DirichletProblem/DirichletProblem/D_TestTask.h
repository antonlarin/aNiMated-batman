#ifndef D_TESTTASK_H
#define D_TESTTASK_H
#define DTS_ERRNO_NONE   0
#define DTS_ERRNO_ERR   1
typedef struct
{
	int errCode;
} D_TaskStatus;
class D_TestTask
{
protected:
	double x_left = -1, x_right = 1, y_left = -1, y_right = 1;
	int xDim, yDim;
	double *grid=0;

public:
	D_TestTask();
	~D_TestTask();
	void SetGridDimentions(int n, int m, D_TaskStatus* stat);
	void PrepareGrid(D_TaskStatus* stat);
	double* GetGridPtr(D_TaskStatus* stat);
	double GetXGridStep();
	double GetYGridStep();
	int GetYGridDimention();
	int GetXGridDimention();

};
#endif