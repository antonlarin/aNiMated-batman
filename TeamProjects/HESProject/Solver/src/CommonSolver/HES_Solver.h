#ifndef Solver_H
#define Solver_H

//Export directives
#if defined(_MSC_VER)
//  Microsoft 
#define EXPORT_API __declspec(dllexport)
#define STD_CALL __stdcall
#elif defined(__GNUG__)
//  GCC
#define EXPORT_API __attribute__((visibility("default")))
#define STD_CALL
#else
//  Unknown
#define EXPORT_API
#define STD_CALL
#pragma warning Unknown dynamic link import/export semantics.
#endif


//Error definitions
#define HES_ERR_MESSAGE_MAX_LENGTH 2048

#define HES_ERRNO_NONE   0
#define HES_ERRNO_ERR    1

//Structure describes error
typedef struct {
	int ErrCode;
	char Message[HES_ERR_MESSAGE_MAX_LENGTH];
} HES_Status;


//Pointer to task.
typedef void* HES_Task;

//Alias for C-style string. 
typedef const char* HES_Str; 


extern "C"
{

	/**
	 * All functions in this case return error occurred in method 
	 * as last parameter. If method works correctly status ErrCode 
	 * must be HES_ERRNO_NONE.
	 */

	//Create new task.
	EXPORT_API HES_Task STD_CALL HES_CreateTask(HES_Status*);

	/**
	 * Set time limit for specified task (T in segment [0, T]).
	 * @param limit time limit (T).
	 */ 
	EXPORT_API void STD_CALL HES_SetTimeLimit(HES_Task, double limit, HES_Status*);
	
	/**
	 * Set dimensions of specified task.
	 * @param n grid dimensions by 'x' coordinate.
	 * @param m grid dimensions by 't' coordinate.
	 */
	EXPORT_API void STD_CALL HES_SetDimensions(HES_Task, int n, int m, HES_Status*);
	
	/**
	 * Set initial condition for specified task.
	 * @param funcStr string representation of task initial condition.
	 * Function may have 'x' as free variable.
	 */
	EXPORT_API void STD_CALL HES_SetInitialConditions(HES_Task, HES_Str funcStr, HES_Status*);
	
	/**
	 * Set left boundary condition for specified task.
	 * @param funcStr string representation of task left boundary condition.
	 * Function may have 't' as free variable.
	 */
	EXPORT_API void STD_CALL HES_LeftBoundaryCondition(HES_Task, HES_Str funcStr, HES_Status*);
	
	/**
	 * Set right boundary condition for specified task.
	 * @param funcStr string representation of task right boundary condition.
	 * Function may have 't' as free variable.
	 */
	EXPORT_API void STD_CALL HES_RightBoundaryCondition(HES_Task, HES_Str funcStr, HES_Status*);
	
	/**
	 * Set right hand side function of specified task.
	 * @param funcStr string representation of task right hand side function.
	 * Function may have 'x' and 't' as free variables.
	 */
	EXPORT_API void STD_CALL HES_SetRHSFunction(HES_Task, HES_Str funcStr, HES_Status*);

	/**
	 * Get specified layer of solved task. Task must be solved before calling this method.
	 * @param index index of specified layer. Indexing started from 0 to M, 
	 * where M grid dimensions by 't' variable.
	 * @param layer vector of computed task values. Memory must be allocated before passing
	 * pointer to function. Size of memory block must be greater or equal sizeof(double)*(N + 1),
	 * where N grid dimensions by 'x' variable. 
	 */
	EXPORT_API void STD_CALL HES_GetLayer(HES_Task, int index, double* layer, HES_Status*);
	
	/**
	 * Get all task solution. Task must be solved before calling this method.
	 * @param table matrix of computed task values. Matrix stored by layers. First layer 
	 * must be first row of matrix. Memory must be allocated before passing
	 * pointer to function. Size of memory block must be greater or equal 
	 * sizeof(double)*(N + 1)*(M + 1), where (N, M) grid dimensions. 
	 */
	EXPORT_API void STD_CALL HES_GetTable(HES_Task, double* table, HES_Status*);
	
	/**
	 * Return value stored in specified position of table. Task must be solved before
	 * calling this method.
	 * @param layer index of table layer.
	 * @param index index of layer column.
	 */
	EXPORT_API double STD_CALL HES_GetSolution(HES_Task, int layer, int index, HES_Status*);

	//Return maximal value in solution. Task must be solved before calling this method.
	EXPORT_API double STD_CALL HES_GetMaxFormTable(HES_Task, HES_Status*);

	//Return minimal value in solution. Task must be solved before calling this method.
	EXPORT_API double STD_CALL HES_GetMinFormTable(HES_Task, HES_Status*);

	/*
	* Return norm of difference between reference function and solution.
	* @param string contains reference function.
	* Function may have 'x' and 't' as free variables.
	* Task must be solved before calling this method.
	*/
	EXPORT_API double STD_CALL HES_GetErrorNorm(HES_Task, HES_Str, HES_Status*);

	//Solve specified task.
	EXPORT_API void STD_CALL HES_Solve(HES_Task, HES_Status*);

	//Release all resources used by task.
	EXPORT_API void STD_CALL HES_FreeTask(HES_Task, HES_Status*);

};

#endif