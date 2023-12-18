#include "parser.h"

bool
Parser::parse(Token* input, TreeNode*& root)
{
	bool rt = _dfa.run(input);
	root = _dfa.getProgram();
	return rt;
}

