
#ifndef MathParser_H
#define MathParser_H

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
#define MP_ERR_MESSAGE_MAX_LENGTH 2048

#define MP_ERRNO_NONE         0
#define MP_ERRNO_ARGUMENT     1
#define MP_ERRNO_PARSING      2
#define MP_ERRNO_INTERNAL     3


//Structure describes error
typedef struct {
	int ErrCode;
	char Message[MP_ERR_MESSAGE_MAX_LENGTH];
} MPErrObj;

//Pointer to Math Parser Function
typedef void* MPFunction;

//One dimensional user defined function.
typedef double (*UserFunc)(double);


extern "C"
{

	//Parse math function and return pointer to function object.
	//If we can't parse input string returnes NULL.
	EXPORT_API MPFunction STD_CALL MP_Parse(
		const char* src, 
		MPErrObj* err);

	// Write to buffers variables identifiers.
	// Memory for buffers must be allocated before passing.
	// Size of allocated memory must be sizeof(char*)*Dimensions,
	// where Dimensions is count of variables (use MP_GetDimensions).
	// Size of one buffer must be maximum of variable identifier length
	// (use MP_GetMaxIdentifierSize).
	EXPORT_API void MP_GetVariables(
		const MPFunction func,
		char** buffers,
		MPErrObj* err);

	//Add maping variable name to index of arguments array.
	EXPORT_API void STD_CALL MP_SetVariable(
		const MPFunction func, 
		const char* varName, 
		int varIndex,
		MPErrObj* err);

	//Get variables count of parsed function.
	//If function constant returnes zero.
	//If error occurred returnes -1.
	EXPORT_API int STD_CALL MP_GetDimensions(
		const MPFunction func,
		MPErrObj* err);

	//Evaluate parsed function value at point. If error returns signaling NaN.
	EXPORT_API double STD_CALL MP_Calc(
		const MPFunction func, 
		const double* x,
		 MPErrObj* err);

	//Evaluate parsed function value at point without error reporting, and input data checking.
	EXPORT_API double STD_CALL MP_CalcUnsafe(
		const MPFunction func, 
		const double* x);

	//Release allocated function object.
	EXPORT_API void STD_CALL MP_Delete(
		MPFunction* func,
		MPErrObj* err);


	//Add new function.
	EXPORT_API void STD_CALL MP_AddFunc(
		const char* id,
		UserFunc f,
		MPErrObj* err);

	// Return maximum supported identifier size.
	EXPORT_API int STD_CALL MP_GetMaxIdentifierSize(
		MPErrObj* err);

};


#endif