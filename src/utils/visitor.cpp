#include "visitor.h"

void
Visitor::visit(PlaceHolder* node) {};

void Visitor::visit(Program* node)
{
	preAction(node);
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	postAction(node);
}

void Visitor::visit(FuncDecl* node)
{
	preAction(node);
	node->getType()->accept(this);
	node->getId()->accept(this);
	node->getDeclList()->accept(this);
	postAction(node);
}

void Visitor::visit(ClassDecl* node)
{
	preAction(node);
	node->getId()->accept(this);
	postAction(node);
}

void Visitor::visit(StructDecl* node)
{
	preAction(node);
	node->getId()->accept(this);
	postAction(node);
}

void Visitor::visit(VarDef* node)
{
	preAction(node);
	node->getType()->accept(this);
	node->getId()->accept(this);
	if (node->getExpr()) {
		node->getExpr()->accept(this);
	}
	postAction(node);
}

void Visitor::visit(FuncDef* node) 
{
	preAction(node);
	node->getType()->accept(this);
	node->getId()->accept(this);
	node->getDecls()->accept(this);
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	postAction(node);
}

void Visitor::visit(ClassDef* node)
{
	preAction(node);
	node->getId()->accept(this);
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	postAction(node);
}

void Visitor::visit(StructDef* node)
{
	preAction(node);
	node->getId()->accept(this);
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	postAction(node);
}

void Visitor::visit(FuncCall* node) 
{
	preAction(node);
	node->getId()->accept(this);
	node->getParams()->accept(this);
	postAction(node);
}

void Visitor::visit(Assignment* node)
{
	preAction(node);
	node->getId()->accept(this);
	node->getExpr()->accept(this);
	postAction(node);
}

void Visitor::visit(Conditon* node) 
{
	preAction(node);
	node->getExpr()->accept(this);
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	postAction(node);
}

void Visitor::visit(Loop* node)
{
	preAction(node);
	if (node->getInitDef()) {
		node->getInitDef()->accept(this);
	}
	else {
		node->getInitAssign()->accept(this);
	}
	node->getConditon()->accept(this);
	for (auto repeat : node->getRepeats()) {
		repeat->accept(this);
	}
	for (auto stmt: node->getBody()) {
		stmt->accept(this);
	}
	postAction(node);
}

void Visitor::visit(Expr0* node)
{
	preAction(node);
	if (node->getFuncCall()) {
		node->getFuncCall()->accept(this);
	}
	else {
		node->getFactor()->accept(this);
	}
	postAction(node);
}

void Visitor::visit(Expr1* node)
{
	preAction(node);
	node->getExpr()->accept(this);
	postAction(node);
}

void Visitor::visit(Expr2* node)
{
	preAction(node);
	node->getLeft()->accept(this);
	if (node->getOper()) {
		node->getOper()->accept(this);
		node->getRight()->accept(this);
	}
	postAction(node);
}

void Visitor::visit(BinOp* node) 
{
	preAction(node);
	postAction(node);
};

void Visitor::visit(ParamList* node)
{
	preAction(node);
	for (auto param : node->getParams()) {
		param->accept(this);
	}
	postAction(node);
}

void Visitor::visit(DeclList* node)
{
	preAction(node);
	for (auto varDef : node->getDecls()) {
		varDef->accept(this);
	}
	postAction(node);
}

void Visitor::visit(Type* node)
{
	preAction(node);
	node->getBaseType()->accept(this);
	postAction(node);
}

void Visitor::visit(BaseType* node)
{
	preAction(node);
	postAction(node);
};

void Visitor::visit(Factor* node)
{
	preAction(node);
	if (node->isId()) {
		visit(node->getId());
	}
	else {
		visit(node->getValue());
	}
	postAction(node);
};

void Visitor::visit(Identifier* node)
{
	preAction(node);
	postAction(node);
};

void Visitor::visit(Value* node)
{
	preAction(node);
	postAction(node);

};

void Visitor::visit(Return* node)
{
	preAction(node);
	if (node->getExpr()) {
		node->getExpr()->accept(this);
	}
	postAction(node);
}