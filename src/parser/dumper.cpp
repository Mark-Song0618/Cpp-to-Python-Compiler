#include "dumper.h"
#include <fstream>

Dumper::Dumper(std::string fileName)
	: _output(std::ofstream(fileName)), _level(0) {}

void Dumper::preAction(TreeNode* node)
{
	++_level;
	_output << std::endl;
	std::string tab(_level*2, ' ');
	_output << tab;
	switch (node->getClassId()) {
	case SYNTAX::Program:
		 prettyPrint<Program>(dynamic_cast<Program*>(node));
		 return;
	case SYNTAX::FuncDecl:
		 prettyPrint<FuncDecl>(dynamic_cast<FuncDecl*>(node));
		 return;
	case SYNTAX::ClassDecl:
		 prettyPrint<ClassDecl>(dynamic_cast<ClassDecl*>(node));
		 return;
	case SYNTAX::StructDecl:
		 prettyPrint<StructDecl>(dynamic_cast<StructDecl*>(node));
		 return;
	case SYNTAX::VarDef:
		 prettyPrint<VarDef>(dynamic_cast<VarDef*>(node));
		 return;
	case SYNTAX::FuncDef:
		 prettyPrint<FuncDef>(dynamic_cast<FuncDef*>(node));
		 return;
	case SYNTAX::ClassDef:
		 prettyPrint<ClassDef>(dynamic_cast<ClassDef*>(node));
		 return;
	case SYNTAX::StructDef:
		 prettyPrint<StructDef>(dynamic_cast<StructDef*>(node));
		 return;
	case SYNTAX::FuncCall:
		 prettyPrint<FuncCall>(dynamic_cast<FuncCall*>(node));
		 return;
	case SYNTAX::Assignment:
		 prettyPrint<Assignment>(dynamic_cast<Assignment*>(node));
		 return;
	case SYNTAX::Conditon:
		 prettyPrint<Conditon>(dynamic_cast<Conditon*>(node));
		 return;
	case SYNTAX::Loop:
		 prettyPrint<Loop>(dynamic_cast<Loop*>(node));
		 return;
	case SYNTAX::PlaceHolder:
		 prettyPrint<PlaceHolder>(dynamic_cast<PlaceHolder*>(node));
		 return;
	case SYNTAX::Expr0:
		 prettyPrint<Expr0>(dynamic_cast<Expr0*>(node));
		 return;
	case SYNTAX::Expr1:
		 prettyPrint<Expr1>(dynamic_cast<Expr1*>(node));
		 return;
	case SYNTAX::Expr2:
		 prettyPrint<Expr2>(dynamic_cast<Expr2*>(node));
		 return;
	case SYNTAX::BinOp:
		 prettyPrint<BinOp>(dynamic_cast<BinOp*>(node));
		 return;
	case SYNTAX::ParamList:
		 prettyPrint<ParamList>(dynamic_cast<ParamList*>(node));
		 return;
	case SYNTAX::DeclList:
		 prettyPrint<DeclList>(dynamic_cast<DeclList*>(node));
		 return;
	case SYNTAX::Type:
		 prettyPrint<Type>(dynamic_cast<Type*>(node));
		 return;
	case SYNTAX::BaseType:
		 prettyPrint<BaseType>(dynamic_cast<BaseType*>(node));
		 return;
	case SYNTAX::Factor:
		 prettyPrint<Factor>(dynamic_cast<Factor*>(node));
		 return;
	case SYNTAX::Identifier:
		 prettyPrint<Identifier>(dynamic_cast<Identifier*>(node));
		 return;
	case SYNTAX::Value:
		 prettyPrint<Value>(dynamic_cast<Value*>(node));
		 return;
	case SYNTAX::Return:
		 prettyPrint<Return>(dynamic_cast<Return*>(node));
		 return;
	default:
		return;
	}
}

template <>
void
Dumper::prettyPrint(Program* node)
{ 
	_output << "Program has " \
			<< node->getStmts().size() 
			<< " statements:" 
			<< std::endl;
}

template <>
void
Dumper::prettyPrint(FuncDecl* node)
{
	_output << "FuncDecl:" <<  std::endl;
}

template <>
void   
Dumper::prettyPrint(ClassDecl* node)
{ 
	_output << "ClassDecl:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(StructDecl* node)
{
	_output << "StructDecl:" << std::endl;
}

template <>
void    
Dumper::prettyPrint(VarDef* node)
{	
	_output << "VarDef:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(FuncDef* node)
{ 
	_output << "FuncDef:" << std::endl;
}

template <>
void  
Dumper::prettyPrint(ClassDef* node)
{ 
	_output << "ClassDef:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(StructDef* node)
{	
	_output << "StructDef:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(FuncCall* node)
{
	_output << "FuncCall:" << std::endl;
}

template <>
void  
Dumper::prettyPrint(Assignment* node)
{  
	_output << "Assignment:" << std::endl;
}

template <>
void  
Dumper::prettyPrint(Conditon* node)
{	
	_output << "Conditon:" << std::endl;
}

template <>
void  
Dumper::prettyPrint(Loop* node)
{ 
	_output << "Loop:" << std::endl;
}

template <>
void 
Dumper::prettyPrint(Expr0* node)
{ 
	_output << "Expr0:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(Expr1* node) 
{
	_output << "Expr1:" << std::endl;
}

template <>
void  
Dumper::prettyPrint(Expr2* node)
{
	_output << "Expr2:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(BinOp* node) 
{ 
	switch (node->getOperType()) {
	case BinOp::ADD:
		_output << "+" << std::endl;
		break;
	case BinOp::MINUS:
		_output << "-" << std::endl;
		break;
	case BinOp::MULTI:
		_output << "*" << std::endl;
		break;
	case BinOp::DIV:
		_output << "/" << std::endl;
		break;
	case BinOp::MEMBER:
		_output << "." << std::endl;
		break;
	case BinOp::LESS:
		_output << "<" << std::endl;
		break;
	case BinOp::LESSEQ:
		_output << "<=" << std::endl;
		break;
	case BinOp::GREATER:
		_output << ">" << std::endl;
		break;
	case BinOp::GREATEREQ:
		_output << ">=" << std::endl;
		break;
	case BinOp::EQUAL:
		_output << "==" << std::endl;
		break;
	case BinOp::INVALID:
		break;
	}
}

template <>
void   
Dumper::prettyPrint(ParamList* node)
{ 
	_output << "ParamList:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(DeclList* node)
{
	_output << "DeclList:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(Type* node) 
{
	_output << "Type:" << std::endl;
}

template <>
void  
Dumper::prettyPrint(BaseType* node)
{ 
	switch (node->getType()) {
	case SYNTAX::VAL_CHAR:
		_output << "char" << std::endl;
		return;
	case SYNTAX::VAL_SHORT:
		_output << "short" << std::endl;
		return;
	case SYNTAX::VAL_INT:
		_output << "int" << std::endl;
		return;
	case SYNTAX::VAL_LONG:
		_output << "long" << std::endl;
		return;
	case SYNTAX::VAL_FLOAT:
		_output << "float" << std::endl;
		return;
	case SYNTAX::VAL_DOUBLE:
		_output << "double" << std::endl;
		return;
	defualt:
		return;
	}
}

template <>
void  
Dumper::prettyPrint(Factor* node) 
{
	_output << "Factor:" << std::endl;
}

template <>
void   
Dumper::prettyPrint(Identifier* node)
{ 
	_output << node->getName() << std::endl;
}

template <>
void   
Dumper::prettyPrint(Value* node)
{ 
	switch (node->getType()) {
	case SYNTAX::VAL_CHAR:
		_output << node->getValue() << std::endl;
		return;
	case SYNTAX::VAL_SHORT:
		_output << node->getValue_s() << std::endl;
		return;
	case SYNTAX::VAL_INT:
		_output << node->getValue_i() << std::endl;
		return;
	case SYNTAX::VAL_LONG:
		_output << node->getValue_l() << std::endl;
		return;
	case SYNTAX::VAL_FLOAT:
		_output << node->getValue_f() << std::endl;
		return;
	case SYNTAX::VAL_DOUBLE:
		_output << node->getValue_d() << std::endl;
		return;
	default:
		return;
	}
}

template <>
void  
Dumper::prettyPrint(Return* node) 
{
	_output << "return" << std::endl;
}