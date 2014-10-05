
#ifndef EvalTable_H
#define EvalTable_H

#include <string>
#include <map>

typedef double (*EvalOperator)(double, double);
typedef double (*EvalFunction)(double);

class EvalTable sealed 
{

private:

	static EvalTable* Instance;

	std::map<std::wstring, EvalOperator> mOpers;
	std::map<std::wstring, EvalFunction> mFuncs;
	std::map<std::wstring, double> mConsts;

	static EvalTable* GetInstance();

	EvalTable();
	~EvalTable();

	void InitOperators();
	void InitFunctions();
	void InitConstants();

public:

	static bool IsConstExists(std::wstring& id);

	static EvalOperator GetOper(std::wstring& id);
	static EvalFunction GetFunc(std::wstring& id);
	static double GetConst(std::wstring& id);

	static void Clean();
	static void AddFunc(std::wstring& id, EvalFunction f);
};

#endif