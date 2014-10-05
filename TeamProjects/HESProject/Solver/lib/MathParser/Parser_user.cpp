
#include "Parser.h"

Parser::Parser(Scanner* scanner, EvalQueue* q)
	: Parser(scanner)
{ mInternalQ = q; }


void Parser::eq_(str& s, EvalAtomType atomType)
{
	if (mInternalQ == NULL)
		return;

	if (s.empty())
		return;

	EvalAtom* atom = new EvalAtom(s, atomType);
	mInternalQ->push_back(atom);
}