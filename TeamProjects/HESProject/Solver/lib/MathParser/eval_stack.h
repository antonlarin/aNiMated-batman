
#ifndef EvalStack_H
#define EvalStack_H

#include <string>
#include <stack>
#include <list>

#include "type_aliases.h"
#include "eval_table.h"


enum EvalAtomType
{
	Operator,
	Function,
	Constant,
	Variable
};

class EvalAtom
{
public:

	EvalAtomType AtomType;

	double Value;
	int Index;
	std::string Ident;
	EvalFunction Function;
	EvalOperator Operator;


	EvalAtom(std::wstring& src, EvalAtomType atomType);
	~EvalAtom();

	void CreateOperator     (std::wstring& src);
	void CreateFunction     (std::wstring& src);
	void CreateNamedConstant(std::wstring& src);
	void CreateConstant     (std::wstring& src);
	void CreateVariable     (std::wstring& src);

};

typedef std::stack<double> EvalStack;
typedef std::list<EvalAtom*> EvalQueue;


#endif