#pragma once
#include "../utils/visitor.h"
#include <iostream>

class Dumper : public Visitor {
public:
	Dumper() : _output(std::cout), _level(0) {}

	Dumper(std::string fileName);
	
	void		dump(TreeNode* root) { root->accept(this); }
	
	void		preAction(TreeNode*);
	
	void		postAction(TreeNode*) {--_level;}

	template <typename T>
	void        prettyPrint(T* node) {};
private:
	unsigned		_level;

	std::ostream&	_output;
};
template <>
void Dumper::prettyPrint(Program* node);
template <>
void Dumper::prettyPrint(FuncDecl* node);
template <>
void Dumper::prettyPrint(ClassDecl* node);
template <>
void Dumper::prettyPrint(StructDecl* node);
template <>
void Dumper::prettyPrint(VarDef* node);
template <>
void Dumper::prettyPrint(FuncDef* node);
template <>
void Dumper::prettyPrint(ClassDef* node);
template <>
void Dumper::prettyPrint(StructDef* node);
template <>
void Dumper::prettyPrint(FuncCall* node);
template <>
void Dumper::prettyPrint(Assignment* node);
template <>
void Dumper::prettyPrint(Conditon* node);
template <>
void Dumper::prettyPrint(Loop* node);
template <>
void Dumper::prettyPrint(Expr0* node);
template <>
void Dumper::prettyPrint(Expr1* node);
template <>
void Dumper::prettyPrint(Expr2* node);
template <>
void Dumper::prettyPrint(BinOp* node);
template <>
void Dumper::prettyPrint(ParamList* node);
template <>
void Dumper::prettyPrint(DeclList* node);
template <>
void Dumper::prettyPrint(Type* node);
template <>
void Dumper::prettyPrint(BaseType* node);
template <>
void Dumper::prettyPrint(Factor* node);
template <>
void Dumper::prettyPrint(Identifier* node);
template <>
void Dumper::prettyPrint(Value* node);
template <>
void Dumper::prettyPrint(Return* node);