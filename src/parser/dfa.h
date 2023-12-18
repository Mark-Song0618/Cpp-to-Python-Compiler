#pragma once
#include "syntax.h"
#include "../lexer/lexerPublic.h"
#include "treeNode.h"
#include <map>
#include <stack>
typedef	void(*productionHandler)(void);

class DFA {
public:
	bool run(Token* input);
	Program* getProgram() { return _program; }
private:
	productionHandler	getProduction();
	static Token*		lookAhead(size_t n);	// n : position after current
	static bool			isUdt();
	template <typename T>
	static void			production();

private:
	static Token*					_currToken;
	static Program*					_program;
	static std::list<TreeNode*>		_udt;	// user defined type
	static std::stack<TreeNode*>	_stack;
};

template <> void DFA::production<Program>();
template <> void DFA::production<FuncDecl>();
template <> void DFA::production<ClassDecl>();
template <> void DFA::production<StructDecl>();
template <> void DFA::production<VarDef>();
template <> void DFA::production<FuncDef>();
template <> void DFA::production<ClassDef>();
template <> void DFA::production<StructDef>();
template <> void DFA::production<FuncCall>();
template <> void DFA::production<Assignment>();
template <> void DFA::production<Conditon>();
template <> void DFA::production<Loop>();
template <> void DFA::production<PlaceHolder>();
template <> void DFA::production<Expr0>();
template <> void DFA::production<Expr1>();
template <> void DFA::production<Expr2>();
template <> void DFA::production<Factor>();
template <> void DFA::production<BaseType>();
template <> void DFA::production<Type>();
template <> void DFA::production<BinOp>();
template <> void DFA::production<DeclList>();
template <> void DFA::production<Identifier>();
template <> void DFA::production<ParamList>();
template <> void DFA::production<Value>();
template <> void DFA::production<Return>();
