#include <algorithm>
#include <stdexcept>

#include "InternalFunction.h"
#include "Scanner.h"
#include "Parser.h"

using namespace std;


InternalFunction::InternalFunction(const char* src)
{
	mIndexedVariablesCount = 0;

	size_t strLen = strlen(src);
	auto usrc = reinterpret_cast<const byte*>(src);
	Scanner scanner(usrc, strLen);
	Parser parser(&scanner, &mQ);
	parser.Parse();

	//Determine subset of variable atoms.
	auto start = mQ.begin();
	auto end = mQ.end();
	for (auto s = start; s != end; s++)
	{
		if ((*s)->AtomType == EvalAtomType::Variable)
		{
			if (!IsVariableIdExists((*s)->Ident))
				mVariablesNames.push_back((*s)->Ident);

			mVariablesList.push_back((*s));
		}	
	}
}

InternalFunction::~InternalFunction() 
{
	for_each(mQ.begin(), mQ.end(), [&](EvalAtom* atom) 
	{ delete atom; });
}



int InternalFunction::GetVariablesCount()
{ return mVariablesNames.size(); }

void InternalFunction::MapVariableToIndex(
	const char* varName, int index)
{
	if (varName == NULL)
		throw runtime_error("Variable name is null.");

	if (index < 0)
		throw runtime_error("Invalid index.");

	string strVarName = string(varName);
	mIndexedVariablesCount = 0;

	auto start = mVariablesList.begin();
	auto end = mVariablesList.end();
	for (auto s = start; s != end; s++)
	{
		if ((*s)->Ident == strVarName)
			(*s)->Index = index;

		mIndexedVariablesCount += (int)((*s)->Index > -1);
	}	
}

void InternalFunction::VariablesToBuffers(char** buffers)
{
	auto start = mVariablesNames.begin();
	auto end = mVariablesNames.end();
	for (auto s = start; s != end; s++)
	{
		const char* cIdent = s->c_str();
		strcpy(*buffers, cIdent);
		buffers++; 
	}	
}

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


bool InternalFunction::IsAllVariablesIndexed()
{ return mIndexedVariablesCount == mVariablesList.size(); }

bool InternalFunction::IsVariableIdExists(string& id) 
{
	auto start = mVariablesList.begin();
	auto end = mVariablesList.end();
	for (auto s = start; s != end; s++)
	{
		if ((*s)->Ident == id)
			return true;
	}
	return false;
}