#pragma once

#include "../lexer/lexerPublic.h"
#include "dfa.h"
#include "treeNode.h"

class Parser {
public:
	bool		parse(Token* input, TreeNode*& root);
	Program*	getProgram() { return _dfa.getProgram(); }
private:
	DFA  _dfa;
};
