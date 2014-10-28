#include "HES_Solver.h"
#include "HE_Task.h"
#include "HE_Integrator.h"

EXPORT_API HES_Task STD_CALL HES_CreateTask(HES_Status* stat)
{
	stat->ErrCode = HES_ERRNO_NONE;
	return new HE_Task;
}
EXPORT_API void STD_CALL HES_SetTimeLimit(HES_Task task, double limit, HES_Status* stat)
{
	((HE_Task*)task)->SetTimeLimit(limit, stat);
}
EXPORT_API void STD_CALL HES_SetDimensions(HES_Task task, int n, int m, HES_Status* stat)
{
	((HE_Task*)task)->SetDimensions(n, m, stat);
}
EXPORT_API void STD_CALL HES_SetInitialConditions(HES_Task task, HES_Str funcStr, HES_Status* stat)
{
	((HE_Task*)task)->SetInitialConditions(funcStr, stat);
}
EXPORT_API void STD_CALL HES_LeftBoundaryCondition(HES_Task task, HES_Str funcStr, HES_Status* stat)
{
	((HE_Task*)task)->SetLeftBoundaryCondition(funcStr, stat);
}
EXPORT_API void STD_CALL HES_RightBoundaryCondition(HES_Task task, HES_Str funcStr, HES_Status* stat)
{
	((HE_Task*)task)->SetRightBoundaryCondition(funcStr, stat);
}
EXPORT_API void STD_CALL HES_SetRHSFunction(HES_Task task, HES_Str funcStr, HES_Status* stat)
{
	((HE_Task*)task)->SetRHSFunction(funcStr, stat);
}
EXPORT_API void STD_CALL HES_GetLayer(HES_Task task, int index, double* layer, HES_Status* stat)
{
	((HE_Task*)task)->GetLayer(index, layer, stat);
}
EXPORT_API void STD_CALL HES_GetTable(HES_Task task, double* table, HES_Status* stat)
{
	((HE_Task*)task)->GetTable(table, stat);
}
EXPORT_API void STD_CALL HES_Solve(HES_Task task, HES_Status* stat)
{
	HE_Integrate(((HE_Task*)task), stat);
}
EXPORT_API void STD_CALL HES_FreeTask(HES_Task task, HES_Status* stat)
{
	delete ((HE_Task*)task);
}
EXPORT_API double STD_CALL HES_GetSolution(HES_Task task, int layer, int index, HES_Status* stat)
{
	return ((HE_Task*)task)->GetSolution(layer, index, stat);
}
EXPORT_API double STD_CALL HES_GetMaxFormTable(HES_Task task, HES_Status* stat)
{
	return ((HE_Task*)task)->GetMaxFromTable(stat);
}
EXPORT_API double STD_CALL HES_GetMinFormTable(HES_Task task, HES_Status* stat)
{
	return ((HE_Task*)task)->GetMinFromTable(stat);
}
EXPORT_API double STD_CALL HES_GetErrorNorm(HES_Task task, HES_Str funcStr, HES_Status* stat)
{
	return ((HE_Task*)task)->GetErrorNorm(funcStr, stat);
}