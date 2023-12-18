#pragma once
#include "../parser/parserPublic.h"
#include "../utils/visitor.h"
#include <iostream>
#include <fstream>
class PyGenerator : public Visitor {
public:
	PyGenerator();

	PyGenerator(std::string file);

	bool		generate(TreeNode* input);

	void		generateForLoop(Assignment*, Expr2*, std::list<Statement*>);

	void		generateForLoop(VarDef*, Expr2*, std::list<Statement*>);

	void        visit(FuncDecl* node) override ;

	void        visit(ClassDecl* node) override ;

	void        visit(StructDecl* node) override ;

	void        visit(VarDef* node) override ;

	void        visit(FuncDef* node) override ;

	void        visit(ClassDef* node) override ;

	void        visit(StructDef* node) override ;

	void        visit(FuncCall* node) override ;

	void        visit(Assignment* node) override ;

	void        visit(Conditon* node) override ;

	void        visit(Loop* node) override ;

	void        visit(Expr0* node) override ;

	void        visit(Expr1* node) override ;

	void        visit(BinOp* node) override ;

	void        visit(ParamList* node) override ;

	void        visit(DeclList* node) override ;

	void        visit(Identifier* node) override ;

	void        visit(Value* node) override ;

	void        visit(Return* node) override ;

	void        visit(Factor* node) override;

private:
	std::ostream&	_output;

	unsigned		_level;
	
	bool			_newLine;

	bool			_indent;
};