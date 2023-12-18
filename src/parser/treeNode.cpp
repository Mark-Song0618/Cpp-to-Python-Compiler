#include "../utils/visitor.h"
#include "treeNode.h"

#define VISIT(T) \
	void T::accept(Visitor* v) {v->visit(this);}

VISIT(PlaceHolder)
VISIT(Statement)
VISIT(Program)
VISIT(FuncDecl)
VISIT(ClassDecl)
VISIT(StructDecl)
VISIT(VarDef)
VISIT(FuncDef)
VISIT(ClassDef)
VISIT(StructDef)
VISIT(FuncCall)
VISIT(Assignment)
VISIT(Conditon)
VISIT(Loop)
VISIT(Expr0)
VISIT(Expr1)
VISIT(Expr2)
VISIT(BinOp)
VISIT(ParamList)
VISIT(DeclList)
VISIT(Type)
VISIT(BaseType)
VISIT(Factor)
VISIT(Identifier)
VISIT(Value)
VISIT(Return)

TreeNode::TreeNode(): _open(false) {}

Type::Type() : _baseType(nullptr), _ptrCnt(0), _topConst(0), _innerConst(0){}

bool
Expr2::Evaluate(int& rt)
{
	if (!_right) {
		return false;
	}
	return _right->Evaluate(rt);
}

bool
Expr1::Evaluate(int& rt)
{
	if (!_expr) {
		return false;
	}
	return _expr->Evaluate(rt);
}

bool
Expr0::Evaluate(int& rt)
{
	if (_factor == nullptr || _factor->getValue() == nullptr) {
		return false;
	}
	else if (_factor->getValue()->getType() != SYNTAX::VAL_INT) {
		return false;
	}
	else {
		rt = _factor->getValue()->getValue_i();
		return true;
	}

}