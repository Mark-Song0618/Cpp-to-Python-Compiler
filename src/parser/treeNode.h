#pragma once

#include "syntax.h"
#include <list>
#include <string>

class PlaceHolder;
class Program;
class Statement;
class FuncDecl;
class ClassDecl;
class StructDecl;
class VarDef;
class FuncDef;
class ClassDef;
class StructDef;
class FuncCall;
class Assignment;
class Conditon;
class Loop;
class Expr0;
class Expr1;
class Expr2;
class BinOp;
class ParamList;
class DeclList;
class Type;
class BaseType;
class Factor;
class Identifier;
class Value;

class Visitor;

// abstract class
class TreeNode {
public:
	TreeNode();

	virtual SYNTAX::ClassId		getClassId() = 0;

	virtual void				accept(Visitor* v) = 0;

	bool						isOpen() { return _open; }

	void						Open() { _open = true; }

protected:
	bool						_open : 1;
};

class PlaceHolder : public TreeNode {
	friend class DFA;
public:
	PlaceHolder(SYNTAX::KeyWordType type) : _type(type) {}
	
	void				accept(Visitor* v);
	
	SYNTAX::ClassId		getClassId() { return SYNTAX::PlaceHolder; }

	SYNTAX::KeyWordType getType() { return _type; }
private:
	SYNTAX::KeyWordType _type;
};

class Value : public TreeNode {
	friend class DFA;
public:
	SYNTAX::ClassId		getClassId() { return SYNTAX::Value; }

	void				accept(Visitor* v);

	Value() : _type(SYNTAX::VAL_INVALID){}

	SYNTAX::ValueType	getType() { return _type; }

	short				getValue_s() { return _v._s; }
	int					getValue_i() { return _v._i; }
	long				getValue_l() { return _v._l; }
	float				getValue_f() { return _v._f; }
	double				getValue_d() { return _v._d; }
	std::string			getValue()   { return _c; }
private:
	SYNTAX::ValueType	_type;
	union {
		short	_s;
		int		_i;
		long    _l;
		float   _f;
		double  _d;
	} _v;
	std::string _c;
};

class Identifier : public TreeNode {
	friend class DFA;
public:
	SYNTAX::ClassId		getClassId() { return SYNTAX::Identifier;  }

	void				accept(Visitor* v);

	std::string			getName() { return _name; }
private:
	std::string _name;
};

class Program : public TreeNode
{
public:
	SYNTAX::ClassId		getClassId() { return SYNTAX::Program; }

	void				accept(Visitor* v);

	void				addStatement(Statement* stmt) { _stmts.emplace_back(stmt); }

	const std::list<Statement*>&
						getStmts() { return _stmts; }

private:
	std::list<Statement*>	_stmts;
};

// abstract class
class Statement : public TreeNode {
public:
	void			accept(Visitor* v);
};

class Return : public Statement
{
	friend class DFA;
public:
	SYNTAX::ClassId getClassId() { return SYNTAX::Return; }

	void			accept(Visitor* v);

	Expr2*			getExpr() { return _expr; }

private:
	Expr2* _expr;
};

class FuncDecl : public Statement
{
	friend class DFA;
public:
	FuncDecl() : _type(nullptr), _id(nullptr), _decls(nullptr) {}

	SYNTAX::ClassId		getClassId()	{ return SYNTAX::FuncDecl; }

	void				accept(Visitor* v);

	Type*				getType() { return _type; };
	
	Identifier*			getId() { return _id; }
	
	DeclList*			getDeclList() { return _decls; }

private:
	Type*			_type;

	Identifier*		_id;
	
	DeclList*		_decls;
};

class ClassDecl : public Statement
{
	friend class DFA;
public:
	ClassDecl() : _id(nullptr) {}

	SYNTAX::ClassId		getClassId() { return SYNTAX::ClassDecl; }

	void				accept(Visitor* v);

	Identifier*			getId() { return _id; }

private:
	Identifier* _id;
};

class StructDecl : public Statement
{
	friend class DFA;
public:
	StructDecl() : _id(nullptr) {}

	SYNTAX::ClassId		getClassId() { return SYNTAX::StructDecl; }

	void				accept(Visitor* v);

	Identifier*			getId() { return _id; }

private:
	Identifier* _id;
};

class VarDef : public Statement
{
	friend class DFA;
public:
	VarDef() : _type(nullptr), _identifier(nullptr), _expr(nullptr) {}

	SYNTAX::ClassId getClassId() { return SYNTAX::VarDef; }

	void			accept(Visitor* v);
	
	Type*			getType() {return _type; }
	
	Identifier*		getId() { return _identifier; }
	
	Expr2*			getExpr() { return _expr; }

private:
	Type*			_type;

	Identifier*		_identifier;
	
	Expr2*			_expr;
};

class FuncDef : public Statement
{
	friend class DFA;
public:
	FuncDef():_type(nullptr), _id(nullptr), _decls(nullptr) {};

	SYNTAX::ClassId getClassId() { return SYNTAX::FuncDef; }

	void			accept(Visitor* v);
	
	Type*			getType() { return _type; }

	Identifier*		getId() { return _id; }

	DeclList*		getDecls() { return _decls; }

	std::list<Statement*>&
		getStmts() {return _stmts;}

private:
	Type* _type;
	Identifier* _id;
	DeclList* _decls;
	std::list<Statement*>	_stmts;
};

class ClassDef : public Statement
{
	friend class DFA;
public:
	ClassDef() : _id(nullptr) {}

	SYNTAX::ClassId		getClassId() { return SYNTAX::ClassDef; }

	void				accept(Visitor* v);

	Identifier*			getId() { return _id; }

	std::list<Statement*>&
						getStmts() { return _stmts; }

private:
	Identifier* _id;

	std::list<Statement*>	_stmts;
};

class StructDef : public Statement
{
	friend class DFA;
public:
	StructDef() : _id(nullptr) {}

	SYNTAX::ClassId		getClassId() { return SYNTAX::StructDef; }

	void				accept(Visitor* v);

	Identifier*			getId() { return _id; }

	std::list<Statement*>&
						getStmts() { return _stmts; }

private:
	Identifier* _id;

	std::list<Statement*>	_stmts;
};

class FuncCall : public Statement
{
	friend class DFA;
public:
	FuncCall() : _id(nullptr), _params(nullptr) {}

	SYNTAX::ClassId		getClassId() { return SYNTAX::FuncCall; }

	void				accept(Visitor* v);

	Identifier*			getId() { return _id; }

	ParamList*			getParams() { return _params; }

private:
	Identifier* _id;

	ParamList*	_params;
};

class Assignment : public Statement
{
	friend class DFA;
public:
	Assignment() : _id(nullptr), _expr(nullptr) {}

	SYNTAX::ClassId	getClassId() { return SYNTAX::Assignment; }

	void			accept(Visitor* v);

	Identifier*		getId() { return _id; }

	Expr2*			getExpr() { return _expr; }

private:
	Identifier* _id;

	Expr2* _expr;
};

class Conditon : public Statement
{
	friend class DFA;
public:
	Conditon():_expr(nullptr) {}

	SYNTAX::ClassId		getClassId() { return SYNTAX::Conditon; }

	void				accept(Visitor* v);
	
	Expr2*				getExpr() { return _expr; }

	std::list<Statement*>&
						getStmts() { return _stmts; }

private:
	Expr2* _expr;
	std::list<Statement*>	_stmts;
};

class Loop : public Statement
{
	friend class DFA;
public:
	SYNTAX::ClassId	getClassId() { return SYNTAX::Loop; }

	void			accept(Visitor* v);

	VarDef*			getInitDef() { return _init._def; }

	Assignment*		getInitAssign() { return _init._assign; }

	Expr2*			getConditon() { return _condition; }

	bool			IsDef() { return _isDef; }

	std::list<Statement*>&
		getRepeats() { return _repeat; }
	
	std::list<Statement*>&
		getBody()	 { return _body; }

private:
	union {
		VarDef* _def;
		Assignment* _assign;
	} _init;
	bool _isDef = false;
	Expr2* _condition = nullptr;
	std::list<Statement*> _repeat;
	std::list<Statement*> _body;
	unsigned _stage = 0;
};

class BinOp : public TreeNode
{
	friend class DFA;
public:
	SYNTAX::ClassId	getClassId() { return SYNTAX::BinOp; }

	void				accept(Visitor* v);
	enum BinOperator {
		ADD,
		MINUS,
		MULTI,
		DIV,
		MEMBER,
		LESS,
		LESSEQ,
		GREATER,
		GREATEREQ,
		EQUAL,
		INVALID,
	};
	
	BinOperator		getOperType() { return _type; }

private:
	BinOperator _type = INVALID;
};

class Expr2 : public TreeNode
{
	friend class DFA;
public:
	SYNTAX::ClassId	getClassId() { return SYNTAX::Expr2; }

	void				accept(Visitor* v);

	Expr1*				getLeft() { return _left; }

	BinOp*				getOper() { return _op; }

	Expr1*				getRight() { return _right; }

	bool				Evaluate(int& rt);

private:
	Expr1* _left = nullptr;
	BinOp* _op = nullptr;
	Expr1* _right = nullptr;
};

class Expr1 : public TreeNode 
{
	friend class DFA;
public:
	SYNTAX::ClassId		getClassId() { return SYNTAX::Expr1; }

	void				accept(Visitor* v);

	Expr0*				getExpr() { return _expr; }

	bool				Evaluate(int& rt);

	enum uniOperator {
		NONE,
		PREFIX_ADD,
		PREFIX_MINUS,
		SUFFIX_ADD,
		SUFFIX_MINUS,
	};

	uniOperator			getType() { return _type; }

private:
	Expr0*		_expr = nullptr;
	uniOperator _type = NONE;
};

class Expr0 : public TreeNode {
	friend class DFA;
public:
	SYNTAX::ClassId	getClassId() { return SYNTAX::Expr0; }

	void			accept(Visitor* v);

	FuncCall*		getFuncCall() { return _func; }

	Factor*			getFactor() { return _factor; }

	bool			Evaluate(int& rt);

private:
	FuncCall* _func = nullptr;

	Factor* _factor = nullptr;
};

class ParamList : public TreeNode 
{
	friend class DFA;
public:

	SYNTAX::ClassId		getClassId() { return SYNTAX::ParamList; }

	void				accept(Visitor* v);

	std::list<Expr2*>&
						getParams() { return _params; }

private:
	std::list<Expr2*> _params;
};

class DeclList : public TreeNode
{
	friend class DFA;
public:
	SYNTAX::ClassId	getClassId() { return SYNTAX::DeclList; }

	void			accept(Visitor* v);

	std::list<VarDef*>&
					getDecls() { return _decls; }

private:
	std::list<VarDef*>	_decls;
};

class Type : public TreeNode
{
	friend class DFA;
public:
	Type::Type();
	
	SYNTAX::ClassId		getClassId() { return SYNTAX::Type; }

	void				accept(Visitor* v);

	BaseType*			getBaseType() { return _baseType; }

private:
	BaseType* _baseType = nullptr;
	unsigned  _ptrCnt   = 0;
	bool	  _topConst : 1;
	bool	  _innerConst : 1;
};

class BaseType : public Statement
{
	friend class DFA;
public:
	SYNTAX::ClassId		getClassId() { return SYNTAX::BaseType; }

	void				accept(Visitor* v);

	SYNTAX::ValueType   getType() { return _type; }

private:
	SYNTAX::ValueType	_type = SYNTAX::VAL_INVALID;

	TreeNode*			_udt = nullptr;
};

class Factor : public Statement
{
	friend class DFA;
public:
	SYNTAX::ClassId		getClassId() { return SYNTAX::Factor; }

	void				accept(Visitor* v);

	bool				isId() { return _isIdentifier; }

	Value*				getValue() { return _val._value; }

	Identifier*			getId() { return _val._id; }

private:
	union 
	{
		Identifier* _id;
		Value* _value;
	}  _val;

	bool _isIdentifier = true;
};
