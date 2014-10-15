#ifndef InternalFunction_H
#define InternalFunction_H

#include <string>
#include <list>
#include "eval_stack.h";

class InternalFunction
{

private:

	EvalQueue mQ;
	EvalStack mS;
	std::list<EvalAtom*> mVariablesList;
	std::list<std::string> mVariablesNames; 

	int mIndexedVariablesCount;
	int mVariablesCount;

public:

	InternalFunction(const char* src);
	~InternalFunction();

	int GetVariablesCount();
	void MapVariableToIndex(const char* varName, int index);
	void VariablesToBuffers(char** buffers);
	double Calc(const double* x);

private:

	bool IsAllVariablesIndexed();
	bool IsVariableIdExists(std::string& id);

};

#endif