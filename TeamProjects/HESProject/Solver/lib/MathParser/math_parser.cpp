
#include <algorithm>
#include <stdexcept>
#include <string>
#include <limits>
#include <cmath>

#include "type_aliases.h"
#include "math_parser.h"
#include "eval_stack.h"
#include "eval_table.h"
#include "Parser.h"

using namespace std;


void StringToErrorBuffer(const char* str, char* buffer)
{
	char* strPtr = const_cast<char*>(str);
	int i = 0;
	for (; i < MP_ERR_MESSAGE_MAX_LENGTH; i++)
	{
		*(buffer++) = *(strPtr++);
		if (*strPtr == 0)
			break;
	}

	if (i == MP_ERR_MESSAGE_MAX_LENGTH - 1)
		*buffer = 0;
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



class InternalFunction
{

private:

	EvalQueue mQ;
	EvalStack mS;
	EvalQueue mVariablesQ;

	int mIndexedVariablesCount;

public:

	InternalFunction(const char* src);
	~InternalFunction();

	int GetVariablesCount();
	void MapVariableToIndex(const char* varName, int index);
	double Calc(const double* x);

private:

	bool IsAllVariablesIndexed();

};


InternalFunction::InternalFunction(const char* src)
{
	mIndexedVariablesCount = 0;

	size_t strLen = strlen(src);
	auto usrc = reinterpret_cast<const byte*>(src);
	Scanner scanner(usrc, strLen);
	Parser parser(&scanner, &mQ);
	parser.Parse();

	//Determine subset of variable atoms.
	for_each(mQ.begin(), mQ.end(), [&](EvalAtom* atom)
	{
		if (atom->AtomType == EvalAtomType::Variable)
			mVariablesQ.push_back(atom);
	});
}

InternalFunction::~InternalFunction() 
{
	for_each(mQ.begin(), mQ.end(), [&](EvalAtom* atom) 
	{ delete atom; });
}



int InternalFunction::GetVariablesCount()
{ return mVariablesQ.size(); }


void InternalFunction::MapVariableToIndex(
	const char* varName, int index)
{
	if (varName == NULL)
		throw runtime_error("Variable name is null.");

	if (index < 0)
		throw runtime_error("Invalid index.");

	mIndexedVariablesCount = 0;
	auto mvq = mVariablesQ;
	for_each(mvq.begin(), mvq.end(), [&](EvalAtom* atom) 
	{ 
		if (atom->Ident == string(varName))
			atom->Index = index;

		mIndexedVariablesCount += (int)(atom->Index > -1);
	});
}


bool InternalFunction::IsAllVariablesIndexed()
{ return mIndexedVariablesCount == GetVariablesCount(); }


double InternalFunction::Calc(const double* x)
{
	if (!IsAllVariablesIndexed())
		throw runtime_error("Some variables not maped to index.");

	double left, right;
	for (auto s = mQ.begin(); s != mQ.end(); s++)
	{
		EvalAtom* atom = *s;
		switch (atom->AtomType)
		{
			case EvalAtomType::Operator:
				right = mS.top(); mS.pop();
				left  = mS.top(); mS.pop();
				mS.push(atom->Operator(left, right));
				break;

			case EvalAtomType::Function:
				left = mS.top(); mS.pop();
				mS.push(atom->Function(left));
				break;

			case EvalAtomType::Constant:
				mS.push(atom->Value);
				break;

			case EvalAtomType::Variable:
				mS.push(x[atom->Index]);
				break;
		}
	}

	double res = mS.top(); mS.pop();
	return res;
}




MPFunction STD_CALL MP_Parse(const char* src, MPErrObj* err) 
{
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
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }

	return NULL;
}


void STD_CALL MP_SetVariable(const MPFunction func, 
		const char* varName, int varIndex, MPErrObj* err) 
{
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
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }
}


int STD_CALL MP_GetDimensions(const MPFunction func, MPErrObj* err)
{
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
	catch (exception ex)
	{ SetMPErrObj(err, MP_ERRNO_INTERNAL, ex.what()); }

	return GetSignalingNaN();
}


double STD_CALL MP_CalcUnsafe(const MPFunction func, const double* x)
{
	auto f = reinterpret_cast<InternalFunction*>(func);
	return f->Calc(x);
}


void STD_CALL MP_AddFunc(const char* id, UserFunc f, MPErrObj* err)
{
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
}


void STD_CALL MP_Delete(MPFunction* func, MPErrObj* err)
{
	if (func == NULL)
	{
		SetMPErrObj(err, MP_ERRNO_ARGUMENT, "Function object is null.");
		return;
	}

	auto f = reinterpret_cast<InternalFunction*>(*func);
	delete f;
	*func = NULL;
}
