#pragma once

#include "../parser/treeNode.h"

class Visitor {
public:
	virtual void		preAction(TreeNode*) {};
	virtual void		postAction(TreeNode*) {};
	virtual void        visit(PlaceHolder* node);
	virtual void        visit(Program* node);
	virtual void        visit(Statement* node) {};
	virtual void        visit(FuncDecl* node);
	virtual void        visit(ClassDecl* node);
	virtual void        visit(StructDecl* node);
	virtual void        visit(VarDef* node);
	virtual void        visit(FuncDef* node);
	virtual void        visit(ClassDef* node);
	virtual void        visit(StructDef* node);
	virtual void        visit(FuncCall* node);
	virtual void        visit(Assignment* node);
	virtual void        visit(Conditon* node);
	virtual void        visit(Loop* node);
	virtual void        visit(Expr0* node);
	virtual void        visit(Expr1* node);
	virtual void        visit(Expr2* node);
	virtual void        visit(BinOp* node);
	virtual void        visit(ParamList* node);
	virtual void        visit(DeclList* node);
	virtual void        visit(Type* node);
	virtual void        visit(BaseType* node);
	virtual void        visit(Factor* node);
	virtual void        visit(Identifier* node);
	virtual void        visit(Value* node);
	virtual void        visit(Return* node);
};