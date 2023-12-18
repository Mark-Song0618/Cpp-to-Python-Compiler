#include "generator.h"

static std::stack<bool> _indentStack;
static std::stack<bool>	_newLineStack;

#define SET(flag, b) \
	flag##Stack.push(flag); flag = b;

#define RESET(flag) \
	flag = flag##Stack.top(); flag##Stack.pop();

#define INDENT \
	if (_indent) { _output << std::string(_level * 2, ' ');}

#define NEWLINE \
	if (_newLine) {_output << std::endl;}


PyGenerator::PyGenerator()
	:_level(0), _indent(1), _newLine(1), _output(std::cout){}

PyGenerator::PyGenerator(std::string file)
	:_level(0), _indent(1), _newLine(1), _output(std::ofstream(file)) {}

bool
PyGenerator::generate(TreeNode* input)
{
	input->accept(this);
	return true;
}

void
PyGenerator::generateForLoop(VarDef* init, Expr2* cond, std::list<Statement*> repeats)
{
	std::string _loopVarName;
	int			_start = 0;
	int			_step = 0;
	int			_stop = 0;

	// var name
	_loopVarName = init->getId()->getName();

	// init
	if (init->getExpr()) {
		init->getExpr()->Evaluate(_start);
	}

	// stop
	if (!cond->getLeft()->Evaluate(_stop)) {
		cond->getRight()->Evaluate(_stop);
	}

	// step
	// todo: only support one repeat, and it is an assignment
	// i = i+1
	auto assign = dynamic_cast<Assignment*>(repeats.front());
	auto right = assign->getExpr();
	if (!right->getLeft()->Evaluate(_step)) {
		right->getRight()->Evaluate(_step);
	}
	if (right->getOper()->getOperType() == BinOp::MINUS) {
		_step *= -1;
	}

	_output << _loopVarName
		<< " in Range("
		<< _start
		<< ", "
		<< _stop
		<< ", "
		<< _step
		<< ")";
}

void
PyGenerator::generateForLoop(Assignment* init , Expr2* cond, std::list<Statement*> repeats)
{
	std::string _loopVarName;
	int			_start = 0;
	int			_step = 0;
	int			_stop = 0;

	// var name
	_loopVarName = init->getId()->getName();

	// init
	if (init->getExpr()) {
		init->getExpr()->Evaluate(_start);
	}

	// stop
	if (!cond->getLeft()->Evaluate(_stop)) {
		cond->getRight()->Evaluate(_stop);
	}

	// step
	// todo: only support one repeat, and it is an assignment
	// i = i+1
	auto assign = dynamic_cast<Assignment*>(repeats.front());
	auto right = assign->getExpr();
	if (!right->getLeft()->Evaluate(_step)) {
		right->getRight()->Evaluate(_step);
	}
	if (right->getOper()->getOperType() == BinOp::MINUS) {
		_step *= -1;
	}

	_output << _loopVarName
			<< " in Range("
			<< _start
			<< ", "
			<< _stop
			<< ", "
			<< _step
			<< ")";
}

void PyGenerator::visit(FuncDecl* node)
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)
	_output << "def ";
	node->getId()->accept(this);
	_output << "(";
	visit(node->getDeclList());
	_output << ")";
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(ClassDecl* node)
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)
	_output << "class ";
	visit(node->getId());
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(StructDecl* node)
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)
	_output << "class ";
	visit(node->getId());
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(VarDef* node) 
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)
	visit(node->getId());
	_output << " = ";
	if (node->getExpr()) {
		node->getExpr()->accept(this);
	}
	else {
		_output << "0";
	}
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(FuncDef* node) 
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)
	_output << "def ";
	visit(node->getId());
	_output << "(";
	visit(node->getDecls());
	_output << "):" << std::endl;
	SET(_indent, true)
	SET(_newLine, true)
	++_level;
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	--_level;
	RESET(_indent)
	RESET(_newLine)
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(ClassDef* node) 
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)
	_output << "class ";
	visit(node->getId());
	_output << ":" << std::endl;
	SET(_indent, true)
	SET(_newLine, true)
	++_level;
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	--_level;
	RESET(_indent)
	RESET(_newLine)
	
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(StructDef* node)
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)
	_output << "class ";
	visit(node->getId());
	SET(_indent, true)
	SET(_newLine, true)
	_output << ":" << std::endl;
	++_level;
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	--_level;
	RESET(_indent)
	RESET(_newLine)

	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(FuncCall* node) 
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)

	visit(node->getId());
	_output << "(";
	visit(node->getParams());
	_output << ")";
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(Assignment* node) 
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)

	visit(node->getId());
	_output << " = ";
	Visitor::visit(node->getExpr());
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(Conditon* node)
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)

	_output << "if ";
	Visitor::visit(node->getExpr());
	_output << ":" << std::endl;
	++_level;
	for (auto stmt : node->getStmts()) {
		stmt->accept(this);
	}
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(Loop* node)
{
	INDENT
	SET(_indent, false)
	SET(_newLine, false)

	_output << "for ";
	SET(_indent, false)
	SET(_newLine, false)
	if (node->IsDef()) {
		generateForLoop(node->getInitDef(), node->getConditon(), node->getRepeats());
	}
	else {
		generateForLoop(node->getInitAssign(), node->getConditon(), node->getRepeats());
	}
	_output << ":" << std::endl;
	RESET(_indent)
	RESET(_newLine)
	++_level;
	SET(_indent, true)
	SET(_newLine, true)
	for (auto stmt : node->getBody()) {
		stmt->accept(this);
	}
	--_level;
	RESET(_indent)
	RESET(_newLine)
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void PyGenerator::visit(Expr0* node)
{
	if (node->getFactor()) {
		node->getFactor()->accept(this);
	}
	else if (node->getFuncCall()) {
		visit(node->getFuncCall());
	}
}

void PyGenerator::visit(Expr1* node)
{
	switch (node->getType()) {
	case Expr1::PREFIX_ADD:
		_output << "++";
		visit(node->getExpr());
		break;
	case Expr1::PREFIX_MINUS:
		_output << "--";
		visit(node->getExpr());
		break;
	case Expr1::SUFFIX_ADD:
		visit(node->getExpr());
		_output << "++";
		break;
	case Expr1::SUFFIX_MINUS:
		visit(node->getExpr());
		_output << "--";
		break;
	default:
		visit(node->getExpr());
		break;
	}
}

void PyGenerator::visit(BinOp* node)
{
	switch (node->getOperType()) {
	case BinOp::ADD:
		_output << "+";
		break;
	case BinOp::MINUS:
		_output << "-" ;
		break;
	case BinOp::MULTI:
		_output << "*" ;
		break;
	case BinOp::DIV:
		_output << "/" ;
		break;
	case BinOp::MEMBER:
		_output << "." ;
		break;
	case BinOp::LESS:
		_output << "<" ;
		break;
	case BinOp::LESSEQ:
		_output << "<=" ;
		break;
	case BinOp::GREATER:
		_output << ">" ;
		break;
	case BinOp::GREATEREQ:
		_output << ">=" ;
		break;
	case BinOp::EQUAL:
		_output << "==" ;
		break;
	case BinOp::INVALID:
		break;
	}
}

void PyGenerator::visit(ParamList* node)
{
	for (auto iter = node->getParams().begin(); iter != node->getParams().end();) {
		Visitor::visit(*(iter++));
		if (iter != node->getParams().end()) {
			_output << ",";
		}
	}
}

void PyGenerator::visit(DeclList* node)
{
	for (auto iter = node->getDecls().begin(); iter != node->getDecls().end();) {
		visit((*(iter++))->getId());
		if (iter != node->getDecls().end()) {
			_output << ",";
		}
	}
}

void PyGenerator::visit(Identifier* node)
{
	_output << node->getName();
}

void PyGenerator::visit(Value* node)
{
	switch (node->getType()) {
	case SYNTAX::VAL_CHAR:
		_output << node->getValue();
		return;
	case SYNTAX::VAL_SHORT:
		_output << node->getValue_s() ;
		return;
	case SYNTAX::VAL_INT:
		_output << node->getValue_i() ;
		return;
	case SYNTAX::VAL_LONG:
		_output << node->getValue_l() ;
		return;
	case SYNTAX::VAL_FLOAT:
		_output << node->getValue_f() ;
		return;
	case SYNTAX::VAL_DOUBLE:
		_output << node->getValue_d() ;
		return;
	default:
		return;
	}
}

void PyGenerator::visit(Return* node)
{
	INDENT
	_output << "return";
	SET(_indent, false)
	SET(_newLine, false)
	if (node->getExpr()) {
		_output << " ";
		node->getExpr()->accept(this);
	}
	RESET(_indent)
	RESET(_newLine)
	NEWLINE
}

void  
PyGenerator::visit(Factor* node)
{
	if (node->isId()) {
		node->getId()->accept(this);
	}
	else {
		node->getValue()->accept(this);
	}
}
