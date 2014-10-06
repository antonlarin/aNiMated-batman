
#include "eval_stack.h"
#include <iostream>
#include <locale>

EvalAtom::EvalAtom(std::wstring& src, 
						 EvalAtomType atomType)
{
	AtomType = atomType;
	Value    = 0.0;
	Index    = -1;
	Function = NULL;
	Operator = NULL;

	switch (atomType)
	{
		case EvalAtomType::Operator:
			CreateOperator(src);
			break;

		case EvalAtomType::Function:
			CreateFunction(src);
			break;

		case EvalAtomType::Variable:
			/**
			 * If identifier of variable will be found in 
			 * internal constants table we will store this
			 * constant, not variable. 
			 */
			if (EvalTable::IsConstExists(src))
			{
				AtomType = EvalAtomType::Constant;
				CreateNamedConstant(src);
			}
			else
			{ CreateVariable(src); }
			break;

		case EvalAtomType::Constant:
			CreateConstant(src);
			break;
	}
}

EvalAtom::~EvalAtom() { }


void EvalAtom::CreateOperator(std::wstring& src)
{ Operator = EvalTable::GetOper(src); }

void EvalAtom::CreateFunction(std::wstring& src)
{ Function = EvalTable::GetFunc(src); }

void EvalAtom::CreateNamedConstant(std::wstring& src)
{ Value = EvalTable::GetConst(src); }

void EvalAtom::CreateConstant(std::wstring& src)
{ Value = stod(src); }

void EvalAtom::CreateVariable(std::wstring& src)
{
	/**
	 * Convert wstring representation of identifier to
	 * string representation. No reason to support non
	 * latin characters in math formula.
	 */
	Ident = std::string(src.begin(), src.end());
}