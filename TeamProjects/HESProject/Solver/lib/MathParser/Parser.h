

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include <string>
#include <iostream>
#include "eval_stack.h"

typedef std::wstring str;


#include "Scanner.h"
#include <exception>
#include <string>



class CocoException : public std::exception
{
private:
	std::wstring mStr;

public:
	CocoException(std::wstring& message);
	virtual ~CocoException();

	const wchar_t* wide_what();
};

class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_floatval=1,
		_intval=2,
		_ident=3
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

EvalQueue* mInternalQ = NULL;

	Parser(Scanner* scanner, EvalQueue* q);
	void eq_(str& s, EvalAtomType atomType);





	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void expr();
	void AddSubExp();
	void MltDivExp();
	void PowExp();
	void TermExpr();
	void BrExpr();
	void FuncExpr();
	void Term();

	void Parse();

}; // end Parser



#endif

