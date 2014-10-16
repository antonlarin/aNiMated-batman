
#include "Parser.h"
#include "eval_table.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>


#define O_LMBD(f) ( [](double x, double y) -> double { return f; } )
#define F_LMBD(f) ( [](double x) -> double { return f; } )


EvalTable* EvalTable::Instance = NULL;


EvalTable* EvalTable::GetInstance()
{
	if (Instance == NULL)
		Instance = new EvalTable();

	return Instance;
}

void EvalTable::Clean()
{
	if (Instance != NULL)
		delete Instance;

	Instance = NULL;
}



bool EvalTable::IsConstExists(std::wstring& id)
{
	EvalTable* tbl = GetInstance();
	auto end = tbl->mConsts.end();
	return tbl->mConsts.find(id) != end;
}

void EvalTable::CheckIdentSize(std::wstring& id)
{
	if (id.size() > MP_MAX_IDENTIFIER_SIZE)
		throw std::runtime_error("Identifier is too long.");
}


EvalOperator EvalTable::GetOper(std::wstring& id)
{
	EvalTable* tbl = GetInstance();
	try
	{ return tbl->mOpers.at(id); }
	catch (std::exception ex)
	{ 
		std::wstringstream ss;
		ss << L"Operator '";
		ss << id << L"' ";
		ss << L"undefined.";

		std::wstring exMes = ss.str();
		throw CocoException(exMes); 
	}
}


EvalFunction EvalTable::GetFunc(std::wstring& id)
{
	CheckIdentSize(id);

	EvalTable* tbl = GetInstance();
	try
	{ return tbl->mFuncs.at(id); }
	catch (std::exception ex)
	{ 
		std::wstringstream ss;
		ss << L"Function '";
		ss << id << L"' ";
		ss << L"undefined.";

		std::wstring exMes = ss.str();
		throw CocoException(exMes); 
	}
}


double EvalTable::GetConst(std::wstring& id)
{
	CheckIdentSize(id);

	EvalTable* tbl = GetInstance();
	try
	{ return tbl->mConsts.at(id); }
	catch (std::exception ex)
	{ 
		std::wstringstream ss;
		ss << L"Constant '";
		ss << id << L"' ";
		ss << L"undefined.";

		std::wstring exMes = ss.str();
		throw CocoException(exMes); 
	}
}


void EvalTable::AddFunc(std::wstring& id, EvalFunction f)
{
	CheckIdentSize(id);

	if (id.size() < 3)
	{
		std::wstring exMes = L"Function identifier is too short.";
		throw CocoException(exMes);	
	}

	EvalTable* tbl = GetInstance();
	if (tbl->mFuncs.find(id) == tbl->mFuncs.end())
	{ tbl->mFuncs.emplace(id, f); }
	else
	{
		std::wstring exMes = L"Function already exists.";
		throw CocoException(exMes);	
	}
}

EvalTable::EvalTable()
{
	InitOperators();
	InitFunctions();
	InitConstants();
}

EvalTable::~EvalTable() { }


void EvalTable::InitOperators()
{
	mOpers.emplace(L"+", O_LMBD(x + y));
	mOpers.emplace(L"-", O_LMBD(x - y));
	mOpers.emplace(L"*", O_LMBD(x * y));
	mOpers.emplace(L"/", O_LMBD(x / y));
	mOpers.emplace(L"^", O_LMBD(pow(x, y)));
}

void EvalTable::InitFunctions()
{
	//Inverse function
	mFuncs.emplace(L"-", F_LMBD(-x));


	//Trigonometric functions 
	mFuncs.emplace(L"sin",  F_LMBD( sin(x)     ));
	mFuncs.emplace(L"cos",  F_LMBD( cos(x)     ));
	mFuncs.emplace(L"tan",  F_LMBD( tan(x)     ));
	mFuncs.emplace(L"cot",  F_LMBD( 1 / tan(x) ));

	mFuncs.emplace(L"asin",  F_LMBD( asin(x)     ));
	mFuncs.emplace(L"acos",  F_LMBD( acos(x)     ));
	mFuncs.emplace(L"atan",  F_LMBD( atan(x)     ));
	mFuncs.emplace(L"acot",  F_LMBD( atan(1 / x) ));


	//Hyperbolic functions
	mFuncs.emplace(L"sinh", F_LMBD( sinh(x)     ));
	mFuncs.emplace(L"cosh", F_LMBD( cosh(x)     ));
	mFuncs.emplace(L"tanh", F_LMBD( tanh(x)     ));
	mFuncs.emplace(L"coth", F_LMBD( 1 / tanh(x) ));

	mFuncs.emplace(L"asinh", F_LMBD( asinh(x)     ));
	mFuncs.emplace(L"acosh", F_LMBD( acosh(x)     ));
	mFuncs.emplace(L"atanh", F_LMBD( atanh(x)     ));
	mFuncs.emplace(L"acoth", F_LMBD( atanh(1 / x) ));


	//Exponential and logarithmic functions
	mFuncs.emplace(L"exp",   F_LMBD( exp(x)   ));
	mFuncs.emplace(L"log",   F_LMBD( log(x)   ));
	mFuncs.emplace(L"log10", F_LMBD( log10(x) ));

	//Power functions
	mFuncs.emplace(L"sqrt", F_LMBD( sqrt(x) ));
	mFuncs.emplace(L"cbrt", F_LMBD( cbrt(x) ));

	//Other functions
	mFuncs.emplace(L"abs", F_LMBD( abs(x) ));
}

void EvalTable::InitConstants()
{
	mConsts.emplace(L"pi", M_PI);
	mConsts.emplace(L"e",  M_E);
}