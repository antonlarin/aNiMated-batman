#include <stdexcept>
#include <string>
#include <limits>
#include <cmath>

#include "math_parser.h"
#include "type_aliases.h"
#include "eval_table.h"
#include "InternalFunction.h"
#include "Parser.h"

using namespace std;

/* Helpers methods */

void StringToErrorBuffer(const char* str, char* buffer)
{
	int i = 0;
	for (; i < MP_ERR_MESSAGE_MAX_LENGTH; i++)
	{
		buffer[i] = str[i];
		if (str[i] == 0)
			break;
	}

	if (i == MP_ERR_MESSAGE_MAX_LENGTH - 1)
		buffer[i] = 0;
}

void SetMPErrObj(MPErrObj* err, int errcode, const char* mes)
{
	err->ErrCode = errcode;
	StringToErrorBuffer(mes, err->Message);
}

void SetMPErrObj(MPErrObj* err, int errcode, const wchar_t* mes)
{
	char* mesChars = coco_string_create_char(mes);
	SetMPErrObj(err, errcode, mesChars);
	coco_string_delete(mesChars);
}

inline double GetSignalingNaN()
{ return numeric_limits<double>::signaling_NaN(); }



/* Implementation of MathParser interface */

MPFunction STD_CALL MP_Parse(const char* src, MPErrObj* err) 
{
	err->ErrCode = MP_ERRNO_NONE;

	if (src == NULL) 
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Input string is null.");
		return NULL;
	}

	try
	{
		InternalFunction* f = new InternalFunction(src);
		return reinterpret_cast<MPFunction>(f);
	}
	catch (CocoException ex)
	{ SetMPErrObj(err, MP_ERRNO_PARSING, ex.wide_what()); }
	catch (runtime_error ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }

	return NULL;
}


void MP_GetVariables(const MPFunction func, char** buffers, MPErrObj* err)
{
	err->ErrCode = MP_ERRNO_NONE;

	if (func == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Function object is null.");
		return;
	}

	try
	{
		auto f = reinterpret_cast<InternalFunction*>(func);
		f->VariablesToBuffers(buffers);
	}
	catch (runtime_error ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
}


void STD_CALL MP_SetVariable(const MPFunction func, 
		const char* varName, int varIndex, MPErrObj* err) 
{
	err->ErrCode = MP_ERRNO_NONE;

	if (func == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Function object is null.");
		return;
	}

	try
	{
		auto f = reinterpret_cast<InternalFunction*>(func);
		f->MapVariableToIndex(varName, varIndex);
	}
	catch (CocoException ex)
	{ SetMPErrObj(err, MP_ERRNO_PARSING, ex.wide_what()); }
	catch (runtime_error ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
}


int STD_CALL MP_GetDimensions(const MPFunction func, MPErrObj* err)
{
	err->ErrCode = MP_ERRNO_NONE;

	if (func == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Function object is null.");
		return -1;
	}

	auto f = reinterpret_cast<InternalFunction*>(func);
	return f->GetVariablesCount();
}


double STD_CALL MP_Calc(const MPFunction func, const double* x, MPErrObj* err)
{
	err->ErrCode = MP_ERRNO_NONE;

	if (func == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Function object is null.");
		return GetSignalingNaN();
	}

	if (x == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Argument pointer is null.");
		return GetSignalingNaN();
	}

	try
	{
		auto f = reinterpret_cast<InternalFunction*>(func);
		return f->Calc(x);
	}
	catch (runtime_error ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }

	return GetSignalingNaN();
}


double STD_CALL MP_CalcUnsafe(const MPFunction func, const double* x)
{
	auto f = reinterpret_cast<InternalFunction*>(func);
	return f->Calc(x);
}

void STD_CALL MP_Delete(MPFunction* func, MPErrObj* err)
{
	err->ErrCode = MP_ERRNO_NONE;

	if (func == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Function object is null.");
		return;
	}

	auto f = reinterpret_cast<InternalFunction*>(*func);
	delete f;
	*func = NULL;
}



void STD_CALL MP_AddFunc(const char* id, UserFunc f, MPErrObj* err)
{
	err->ErrCode = MP_ERRNO_NONE;

	if (id == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Function identifier is null.");
		return;
	}

	if (f == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "User function pointer in null.");
		return;
	}

	try 
	{
		wchar_t* wId = coco_string_create(id);
		wstring idstr(wId);
		EvalTable::AddFunc(idstr, f);
		coco_string_delete(wId);
	}
	catch (CocoException ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.wide_what()); }
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
}


int STD_CALL MP_GetMaxIdentifierSize(MPErrObj* err)
{
	err->ErrCode = MP_ERRNO_NONE;
	return MP_MAX_IDENTIFIER_SIZE;
}