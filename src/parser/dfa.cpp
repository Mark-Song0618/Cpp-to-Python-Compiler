#include "dfa.h"
#include "first.h"

#define PLACE_HOLDER(type) \
	_stack.push(new PlaceHolder(SYNTAX::type))

Token*				  DFA::_currToken;
Program*			  DFA::_program;
std::stack<TreeNode*> DFA::_stack;
std::list<TreeNode*>  DFA::_udt;

bool
DFA::run(Token* input)
{
	_currToken = input;
	_program = new Program();
	_stack.push(_program);
	while (true) {
		if (_stack.empty()) {
			// parse tree finished
			return _currToken->type == INVALID;
		}
		if (_currToken->type == INVALID) {
			// input finished or invalid
			if (_stack.top()->getClassId() == SYNTAX::Program) {
				return true;
			}
			else {
				return false;
			}
		}
		productionHandler hdl = getProduction();
		if (!hdl) {
			return false;
		}
		hdl();
	}
}

productionHandler
DFA::getProduction()
{
	switch (_stack.top()->getClassId()) {
	case SYNTAX::Program:
		return production<Program>;
	case SYNTAX::FuncDecl:
		return production<FuncDecl>;
	case SYNTAX::ClassDecl:
		return production<ClassDecl>;
	case SYNTAX::StructDecl:
		return production<StructDecl>;
	case SYNTAX::VarDef:
		return production<VarDef>;
	case SYNTAX::FuncDef:
		return production<FuncDef>;
	case SYNTAX::ClassDef:
		return production<ClassDef>;
	case SYNTAX::StructDef:
		return production<StructDef>;
	case SYNTAX::FuncCall:
		return production<FuncCall>;
	case SYNTAX::Assignment:
		return production<Assignment>;
	case SYNTAX::Conditon:
		return production<Conditon>;
	case SYNTAX::Loop:
		return production<Loop>;
	case SYNTAX::PlaceHolder:
		return production<PlaceHolder>;
	case SYNTAX::Expr0:
		return production<Expr0>;
	case SYNTAX:: Expr1:
		return production<Expr1>;
	case SYNTAX:: Expr2:
		return production<Expr2>;
	case SYNTAX:: BinOp:
		return production<BinOp>;
	case SYNTAX:: ParamList:
		return production<ParamList>;
	case SYNTAX:: DeclList:
		return production<DeclList>;
	case SYNTAX:: Type:
		return production<Type>;
	case SYNTAX:: BaseType:
		return production<BaseType>;
	case SYNTAX:: Factor:
		return production<Factor>;
	case SYNTAX:: Identifier:
		return production<Identifier>;
	case SYNTAX:: Value:
		return production<Value>;
	case SYNTAX::Return:
		return production<Return>;
	default:
		return nullptr;
	}
}
	
Token*
DFA::lookAhead(size_t n)
{
	Token* rt = _currToken;
	for (int i = 0; i < n && rt != nullptr; ++i, ++rt);
	return rt;
}

bool
DFA::isUdt()
{
	if (_currToken->type != ID) {
		return false;
	}
	for (auto it = _udt.begin(); it != _udt.end(); ++it) {
		switch ((*it)->getClassId()) {
		case SYNTAX::StructDecl:
			if (dynamic_cast<StructDecl*>(*it)->_id->_name == _currToken->value) {
				return true;
			}
		case SYNTAX::StructDef:
			if (dynamic_cast<StructDef*>(*it)->_id->_name == _currToken->value) {
				return true;
			}
		case SYNTAX::ClassDecl:
			if (dynamic_cast<ClassDecl*>(*it)->_id->_name == _currToken->value) {
				return true;
			}
		case SYNTAX::ClassDef:
			if (dynamic_cast<ClassDef*>(*it)->_id->_name == _currToken->value) {
				return true;
			}
		}
	}
	return false;
}

template <>
void DFA::production<Program>()
{
	Program* prog = dynamic_cast<Program*>(_stack.top());
	if (_currToken && _currToken->type != INVALID) {
		Statement* stmt = nullptr;
		if (first<Loop>(_currToken->type)) {
			stmt = new Loop;
		}
		else if (first<Conditon>(_currToken->type)) {
			stmt = new Conditon;
		}
		else if (first<Assignment>(_currToken->type) && lookAhead(1)->type == EQUAL) {
			stmt = new Assignment;
		}
		else if (first<StructDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new StructDef;
			}
			else {
				stmt = new StructDecl;
			}
		}
		else if (first<ClassDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new ClassDef;
			}
			else {
				stmt = new ClassDecl;
			}
		}
		else if (first<FuncDef>(_currToken->type) && lookAhead(1)->type == ID && lookAhead(2)->type == PARENTHESES_LEFT) {
			bool bDef = false;
			unsigned i = 1;
			while (lookAhead(i++)->type != SEMICOLON) {
				if (lookAhead(i)->type == BRACE_LEFT) {
					bDef = true;
					break;
				}
			}
			if (bDef) {
				stmt = new FuncDef;
			}
			else {
				stmt = new FuncDecl;
			}
		}
		else if (first<FuncCall>(_currToken->type) && lookAhead(1)->type == PARENTHESES_LEFT) {
			stmt = new FuncCall;
		}
		else if (first<VarDef>(_currToken->type) && lookAhead(1)->type == ID) {
			stmt = new VarDef;
		}
		else {
			// error out;
		}
		prog->addStatement(stmt);
		PLACE_HOLDER(KW_SEMICOLON);
		_stack.push(stmt);
	}
	else {
		_stack.pop();
	}
}

// int add (int a, int b)
template <> void DFA::production<FuncDecl>()
{
	FuncDecl* funcDecl = dynamic_cast<FuncDecl*>(_stack.top());
	funcDecl->_type = new Type();
	funcDecl->_id = new Identifier();
	funcDecl->_decls = new DeclList();
	
	_stack.pop();
	PLACE_HOLDER(KW_PARENTHESES_RIGHT);
	_stack.push(funcDecl->_decls);
	PLACE_HOLDER(KW_PARENTHESES_LEFT);
	_stack.push(funcDecl->_id);
	_stack.push(funcDecl->_type);
}

// class Myclass
template <> 
void DFA::production<ClassDecl>()
{
	ClassDecl* classDecl = dynamic_cast<ClassDecl*>(_stack.top());
	_udt.push_back(classDecl);
	classDecl->_id = new Identifier;
	_stack.pop();
	_stack.push(classDecl->_id);
	PLACE_HOLDER(KW_CLASS);
}

// struct MyStruct
template <> 
void DFA::production<StructDecl>()
{
	StructDecl* structDecl = dynamic_cast<StructDecl*>(_stack.top());
	_udt.push_back(structDecl);
	structDecl->_id = new Identifier;
	_stack.pop();
	_stack.push(structDecl->_id);
	PLACE_HOLDER(KW_STRUCT);
}


// int a [ = expr2]
template <> 
void DFA::production<VarDef>() 
{
	VarDef* varDef = dynamic_cast<VarDef*>(_stack.top());
	varDef->_type = new Type;
	varDef->_identifier = new Identifier;
	_stack.pop();

	unsigned i = 0;
	while (lookAhead(++i)->type != SEMICOLON) {
		if (lookAhead(i)->type == EQUAL) {
			varDef->_expr = new Expr2;
			_stack.push(varDef->_expr);
			PLACE_HOLDER(KW_EQUAL);
			break;
		}
	}
	_stack.push(varDef->_identifier);
	_stack.push(varDef->_type);
}

// int add (delcs) { stmts }
template <> 
void DFA::production<FuncDef>()
{
	FuncDef* funcDef = dynamic_cast<FuncDef*>(_stack.top());
	if (!funcDef->isOpen()) {
		funcDef->_type = new Type;
		funcDef->_id = new Identifier;
		funcDef->_decls = new DeclList;
		PLACE_HOLDER(KW_BRACE_LEFT);
		PLACE_HOLDER(KW_PARENTHESES_RIGHT);
		_stack.push(funcDef->_decls);
		PLACE_HOLDER(KW_PARENTHESES_LEFT);
		_stack.push(funcDef->_id);
		_stack.push(funcDef->_type);
		funcDef->Open();
	}
	else if (_currToken->type == BRACE_RIGHT) {
		_stack.pop();
		++_currToken;
	}
	else {
		Statement* stmt = nullptr;
		if (first<Loop>(_currToken->type)) {
			stmt = new Loop;
		}
		else if (first<Conditon>(_currToken->type)) {
			stmt = new Conditon;
		}
		else if (first<Assignment>(_currToken->type) && lookAhead(1)->type == EQUAL) {
			stmt = new Assignment;
		}
		else if (first<StructDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new StructDef;
			}
			else {
				stmt = new StructDecl;
			}
		}
		else if (first<ClassDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new ClassDef;
			}
			else {
				stmt = new ClassDecl;
			}
		}
		else if ((isUdt() || first<FuncDef>(_currToken->type)) && lookAhead(1)->type == ID && lookAhead(2)->type == PARENTHESES_LEFT) {
			bool bDef = false;
			unsigned i = 1;
			while (lookAhead(i)->type != SEMICOLON) {
				if (lookAhead(i)->type == BRACE_LEFT) {
					bDef = true;
					break;
				}
			}
			if (bDef) {
				stmt = new FuncDef;
			}
			else {
				stmt = new FuncDecl;
			}
		}
		else if (first<FuncCall>(_currToken->type) && lookAhead(1)->type == PARENTHESES_LEFT) {
			stmt = new FuncCall;
		}
		else if ((isUdt() || first<VarDef>(_currToken->type)) && lookAhead(1)->type == ID) {
			stmt = new VarDef;
		}
		else if (_currToken->type == RETURN) {
			stmt = new Return;
		}

		if (stmt) {
			PLACE_HOLDER(KW_SEMICOLON);
			funcDef->_stmts.push_back(stmt);
			_stack.push(stmt);
		}
		else {
			// error out
		}
	}
}

// return [Expr2]
template <>
void DFA::production<Return>()
{
	Return* ret = dynamic_cast<Return*>(_stack.top());
	_stack.pop();
	if (lookAhead(1)->type != SEMICOLON) {
		ret->_expr = new Expr2;
		_stack.push(ret->_expr);
	}
	PLACE_HOLDER(KW_RETURN);
}

// class MyClass {decls & defs}
template <>
void DFA::production<ClassDef>() {
	ClassDef* classDef= dynamic_cast<ClassDef*>(_stack.top());
	_udt.push_back(classDef);
	if (!classDef->_open) {
		classDef->_id = new Identifier;
		PLACE_HOLDER(KW_BRACE_LEFT);
		_stack.push(classDef->_id);
		PLACE_HOLDER(KW_CLASS);
		classDef->_open = true;
	}
	else if (SYNTAX::_kwMap[_currToken->type] == SYNTAX::KW_BRACE_RIGHT) {
		_stack.pop();
		++_currToken;
	}
	else {
		//class items are declares or defs
		Statement* stmt = nullptr;
		if (first<StructDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new StructDef;
			}
			else {
				stmt = new StructDecl;
			}
		}
		else if (first<ClassDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new ClassDef;
			}
			else {
				stmt = new ClassDecl;
			}
		}
		else if (isUdt() || (first<FuncDef>(_currToken->type)) && lookAhead(1)->type == ID && lookAhead(2)->type == PARENTHESES_LEFT) {
			bool bDef = false;
			unsigned i = 1;
			while (lookAhead(i)->type != SEMICOLON) {
				if (lookAhead(i)->type == BRACE_LEFT) {
					bDef = true;
					break;
				}
			}
			if (bDef) {
				stmt = new FuncDef;
			}
			else {
				stmt = new FuncDecl;
			}
		}
		else if (isUdt() || (first<VarDef>(_currToken->type)) && lookAhead(1)->type == ID) {
			stmt = new VarDef;
		}
		if (stmt) {
			PLACE_HOLDER(KW_SEMICOLON);
			classDef->_stmts.push_back(stmt);
			_stack.push(stmt);
		}
		else {
			//error out
		}
	}
}

// struct MyStruct {decls & defs}
template <> 
void DFA::production<StructDef>()
{
	StructDef* structDef = dynamic_cast<StructDef*>(_stack.top());
	_udt.push_back(structDef);
	if (!structDef->_open) {
		structDef->_id = new Identifier;
		PLACE_HOLDER(KW_BRACE_LEFT);
		_stack.push(structDef->_id);
		PLACE_HOLDER(KW_STRUCT);
		structDef->_open = true;
	}
	else if (SYNTAX::_kwMap[_currToken->type] == SYNTAX::KW_BRACE_RIGHT) {
		_stack.pop();
		++_currToken;
	}
	else {
		//class items are declares or defs
		Statement* stmt = nullptr;
		if (first<StructDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new StructDef;
			}
			else {
				stmt = new StructDecl;
			}
		}
		else if (first<ClassDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new ClassDef;
			}
			else {
				stmt = new ClassDecl;
			}
		}
		else if (isUdt() || (first<FuncDef>(_currToken->type)) && lookAhead(1)->type == ID && lookAhead(2)->type == PARENTHESES_LEFT) {
			bool bDef = false;
			unsigned i = 1;
			while (lookAhead(i)->type != SEMICOLON) {
				if (lookAhead(i)->type == BRACE_LEFT) {
					bDef = true;
					break;
				}
			}
			if (bDef) {
				stmt = new FuncDef;
			}
			else {
				stmt = new FuncDecl;
			}
		}
		else if (isUdt() || (first<VarDef>(_currToken->type)) && lookAhead(1)->type == ID) {
			stmt = new VarDef;
		}

		if (stmt) {
			structDef->_stmts.push_back(stmt);
			PLACE_HOLDER(KW_SEMICOLON);
			_stack.push(stmt);
		}
		else {
			// error out
		}
	}
}

// add(paramList)
template <> 
void DFA::production<FuncCall>()
{
	FuncCall* funcCall = dynamic_cast<FuncCall*>(_stack.top());
	funcCall->_id = new Identifier;
	funcCall->_params = new ParamList;

	_stack.pop();
	_stack.push(funcCall->_params);
	PLACE_HOLDER(KW_PARENTHESES_LEFT);
	_stack.push(funcCall->_id);
}

// <id> <=> Expr2
template <> 
void DFA::production<Assignment>()
{
	Assignment* assign = dynamic_cast<Assignment*>(_stack.top());
	assign->_id = new Identifier;
	_stack.pop();

	unsigned i = 0;
	while (lookAhead(++i)->type != SEMICOLON) {
		if (lookAhead(i)->type == EQUAL) {
			assign->_expr = new Expr2;
			_stack.push(assign->_expr);
			PLACE_HOLDER(KW_EQUAL);
			break;
		}
	}
	_stack.push(assign->_id);
}

// <if> <(> Expr2 <)> <{> (Statement)* <}>
template <>
void DFA::production<Conditon>()
{
	Conditon* cond = dynamic_cast<Conditon*>(_stack.top());
	if (!cond->isOpen()) {
		cond->_expr = new Expr2;
		PLACE_HOLDER(KW_BRACE_LEFT);
		PLACE_HOLDER(KW_PARENTHESES_RIGHT);
		_stack.push(cond->_expr);
		PLACE_HOLDER(KW_PARENTHESES_LEFT);
		PLACE_HOLDER(KW_IF);
	}
	else if (_currToken->type == BRACE_RIGHT){
		_stack.pop();
		++_currToken;
	}
	else {
		Statement* stmt = nullptr;
		if (first<Loop>(_currToken->type)) {
			stmt = new Loop;
		}
		else if (first<Conditon>(_currToken->type)) {
			stmt = new Conditon;
		}
		else if (first<Assignment>(_currToken->type) && lookAhead(1)->type == EQUAL) {
			stmt = new Assignment;
		}
		else if (first<StructDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new StructDef;
			}
			else {
				stmt = new StructDecl;
			}
		}
		else if (first<ClassDef>(_currToken->type) && lookAhead(1)->type == ID) {
			if (lookAhead(2)->type == BRACE_LEFT) {
				stmt = new ClassDef;
			}
			else {
				stmt = new ClassDecl;
			}
		}
		else if (isUdt() || (first<FuncDef>(_currToken->type)) && lookAhead(1)->type == ID) {
			bool bDef = false;
			unsigned i = 1;
			while (lookAhead(i)->type != SEMICOLON) {
				if (lookAhead(i)->type == BRACE_LEFT) {
					bDef = true;
					break;
				}
			}
			if (bDef) {
				stmt = new FuncDef;
			}
			else {
				stmt = new FuncDecl;
			}
		}
		else if (first<FuncCall>(_currToken->type) && lookAhead(1)->type == PARENTHESES_LEFT) {
			stmt = new FuncCall;
		}
		else if (isUdt() || (first<VarDef>(_currToken->type)) && lookAhead(1)->type == ID) {
			stmt = new VarDef;
		}
		if (stmt) {
			PLACE_HOLDER(KW_SEMICOLON);
			cond->_stmts.push_back(stmt);
			_stack.push(stmt);
		}
		else {
			// error out
		}
	}
}

// <for> <(> (VarDef | Assignment) <;> Expr2 <;> (Statement)* <)> <{> (Statement)* <}>
// todo
template <>
void DFA::production<Loop>()
{
	Loop* loop = dynamic_cast<Loop*>(_stack.top());
	if (loop->_stage == 0) {
		++(loop->_stage);
		loop->_condition = new Expr2;
		PLACE_HOLDER(KW_SEMICOLON);
		_stack.push(loop->_condition);
		PLACE_HOLDER(KW_SEMICOLON);
		if (lookAhead(2)->type == ID) {
			loop->_isDef = false;
			loop->_init._assign = new Assignment;
			_stack.push(loop->_init._assign);
		}
		else {
			loop->_isDef = true;
			loop->_init._def = new VarDef;
			_stack.push(loop->_init._def);
		}
		PLACE_HOLDER(KW_PARENTHESES_LEFT);
		PLACE_HOLDER(KW_FOR);
	}
	else if (loop->_stage == 1) {
		if (_currToken->type == PARENTHESES_RIGHT) {
			++_currToken;
			++(loop->_stage);
			PLACE_HOLDER(KW_BRACE_LEFT);
		}
		else {
			Statement* stmt = nullptr;
			if (first<Loop>(_currToken->type)) {
				stmt = new Loop;
			}
			else if (first<Conditon>(_currToken->type)) {
				stmt = new Conditon;
			}
			else if (first<Assignment>(_currToken->type) && lookAhead(1)->type == EQUAL) {
				stmt = new Assignment;
			}
			else if (first<StructDef>(_currToken->type) && lookAhead(1)->type == ID) {
				if (lookAhead(2)->type == BRACE_LEFT) {
					stmt = new StructDef;
				}
				else {
					stmt = new StructDecl;
				}
			}
			else if (first<ClassDef>(_currToken->type) && lookAhead(1)->type == ID) {
				if (lookAhead(2)->type == BRACE_LEFT) {
					stmt = new ClassDef;
				}
				else {
					stmt = new ClassDecl;
				}
			}
			else if (isUdt() || (first<FuncDef>(_currToken->type)) && lookAhead(1)->type == ID) {
				bool bDef = false;
				unsigned i = 1;
				while (lookAhead(i)->type != SEMICOLON) {
					if (lookAhead(i)->type == BRACE_LEFT) {
						bDef = true;
						break;
					}
				}
				if (bDef) {
					stmt = new FuncDef;
				}
				else {
					stmt = new FuncDecl;
				}
			}
			else if (first<FuncCall>(_currToken->type) && lookAhead(1)->type == PARENTHESES_LEFT) {
				stmt = new FuncCall;
			}
			else if (isUdt() || (first<VarDef>(_currToken->type)) && lookAhead(1)->type == ID) {
				stmt = new VarDef;
			}
			loop->_repeat.push_back(stmt);
			_stack.push(stmt);
			// todo, place_holde "," for multi stmts;
		}
	}
	else if (loop->_stage == 2) {
		if (_currToken->type == BRACE_RIGHT) {
			++_currToken;
			_stack.pop();
		}
		else {
			Statement* stmt = nullptr;
			if (first<Loop>(_currToken->type)) {
				stmt = new Loop;
			}
			else if (first<Conditon>(_currToken->type)) {
				stmt = new Conditon;
			}
			else if (first<Assignment>(_currToken->type) && lookAhead(1)->type == EQUAL) {
				stmt = new Assignment;
			}
			else if (first<StructDef>(_currToken->type) && lookAhead(1)->type == ID) {
				if (lookAhead(2)->type == BRACE_LEFT) {
					stmt = new StructDef;
				}
				else {
					stmt = new StructDecl;
				}
			}
			else if (first<ClassDef>(_currToken->type) && lookAhead(1)->type == ID) {
				if (lookAhead(2)->type == BRACE_LEFT) {
					stmt = new ClassDef;
				}
				else {
					stmt = new ClassDecl;
				}
			}
			else if (isUdt() || (first<FuncDef>(_currToken->type)) && lookAhead(1)->type == ID) {
				bool bDef = false;
				unsigned i = 1;
				while (lookAhead(i)->type != SEMICOLON) {
					if (lookAhead(i)->type == BRACE_LEFT) {
						bDef = true;
						break;
					}
				}
				if (bDef) {
					stmt = new FuncDef;
				}
				else {
					stmt = new FuncDecl;
				}
			}
			else if (first<FuncCall>(_currToken->type) && lookAhead(1)->type == PARENTHESES_LEFT) {
				stmt = new FuncCall;
			}
			else if (isUdt() || (first<VarDef>(_currToken->type)) && lookAhead(1)->type == ID) {
				stmt = new VarDef;
			}
			loop->_body.push_back(stmt);
			PLACE_HOLDER(KW_SEMICOLON);
			_stack.push(stmt);
		}
	}
}


// Factor | FuncCall
template <>
void 
DFA::production<Expr0>()
{
	Expr0* expr0 = dynamic_cast<Expr0*>(_stack.top());
	_stack.pop();
	if (lookAhead(1)->type == PARENTHESES_LEFT) {
		expr0->_func = new FuncCall;
		_stack.push(expr0->_func);
	}
	else {
		expr0->_factor = new Factor;
		_stack.push(expr0->_factor);
	}
}

/*	<+><+> Expr0 |
*	<-><-> Expr0 |
*	Expr0 <+><+> |
*	Expr0 <-><-> |
*	Expr0
*/
template <>
void 
DFA::production<Expr1>()
{
	Expr1* expr1 = dynamic_cast<Expr1*>(_stack.top());
	_stack.pop();
	expr1->_expr = new Expr0;

	if (_currToken->type == SYNTAX::KW_ADD && lookAhead(1)->type == ADD) {
		expr1->_type = Expr1::PREFIX_ADD;
		_stack.push(expr1->_expr);
		PLACE_HOLDER(KW_ADD);
		PLACE_HOLDER(KW_ADD);
	}
	else if (_currToken->type == MINUS && lookAhead(1)->type == MINUS) {
		expr1->_type = Expr1::PREFIX_MINUS;
		_stack.push(expr1->_expr);
		PLACE_HOLDER(KW_MINUS);
		PLACE_HOLDER(KW_MINUS);
	}
	else {
		bool suffix = false;
		unsigned i = 0;
		while (lookAhead(++i)->type != SEMICOLON) {
			if (lookAhead(i)->type == ADD && lookAhead(i+1)->type == ADD) {
				expr1->_type = Expr1::SUFFIX_ADD;
				PLACE_HOLDER(KW_ADD);
				PLACE_HOLDER(KW_ADD);
				_stack.push(expr1->_expr);
				suffix = true;
				break;
			}
			else if (lookAhead(i)->type == MINUS && lookAhead(i+1)->type == MINUS) {
				expr1->_type = Expr1::SUFFIX_MINUS;
				PLACE_HOLDER(KW_MINUS);
				PLACE_HOLDER(KW_MINUS);
				_stack.push(expr1->_expr);
				suffix = true;
				break;
			}
		}
		if (!suffix) {
			_stack.push(expr1->_expr);
		}
	}
}

// Expr1 BinOp Expr1 | Expr1
template <>
void
DFA::production<Expr2>()
{
	Expr2* expr2 = dynamic_cast<Expr2*>(_stack.top());
	expr2->_left = new Expr1;
	_stack.pop();
	unsigned i = 0;
	while (lookAhead(++i)->type != SEMICOLON && lookAhead(i)->type != PARENTHESES_LEFT) {
		if (first<BinOp>(lookAhead(i)->type)) {
			if (first<BinOp>(lookAhead(i + 1)->type)) {
				break;
			}
			expr2->_op = new BinOp;	// todo : set binOp type
			expr2->_right = new Expr1;
			_stack.push(expr2->_right);
			_stack.push(expr2->_op);
			break;
		}
	}
	_stack.push(expr2->_left);
}

template <>
void
DFA::production<Factor>()
{
	Factor* factor = dynamic_cast<Factor*>(_stack.top());
	_stack.pop();
	if (first<Identifier>(_currToken->type)) {
		factor->_isIdentifier = true;
		factor->_val._id = new Identifier;
		_stack.push(factor->_val._id);
	}
	else {
		factor->_isIdentifier = false;
		factor->_val._value = new Value;
		_stack.push(factor->_val._value);
	}
}

template <>
void DFA::production<PlaceHolder>()
{
	PlaceHolder* placeHolder = dynamic_cast<PlaceHolder*>(_stack.top());
	if (SYNTAX::_kwMap[_currToken->type] == placeHolder->_type) {
		delete _stack.top();
		_stack.pop();
		++_currToken;
	}
	else {
		// error out
	}
}

template <>
void DFA::production<BaseType>()
{
	BaseType* baseType = dynamic_cast<BaseType*>(_stack.top());
	_stack.pop();
	switch (_currToken->type) {
	case CHAR:
		baseType->_type = SYNTAX::VAL_CHAR;
		break;
	case SHORT:
		baseType->_type = SYNTAX::VAL_SHORT;
		break;
	case INT:
		baseType->_type = SYNTAX::VAL_INT;
		break;
	case LONG:
		baseType->_type = SYNTAX::VAL_LONG;
		break;
	case FLOAT:
		baseType->_type = SYNTAX::VAL_FLOAT;
		break;
	case DOUBLE:
		baseType->_type = SYNTAX::VAL_DOUBLE;
		break;
	case ID:
		for (auto it = _udt.begin(); it != _udt.end(); ++it) {
			switch ((*it)->getClassId())
			{
			case SYNTAX::StructDecl:
				if (dynamic_cast<StructDecl*>(*it)->_id->_name == _currToken->value) {
					baseType->_udt = (*it);
				}
				break;
			case SYNTAX::StructDef:
				if (dynamic_cast<StructDef*>(*it)->_id->_name == _currToken->value) {
					baseType->_udt = (*it);
				}
				break;
			case SYNTAX::ClassDecl:
				if (dynamic_cast<ClassDecl*>(*it)->_id->_name == _currToken->value) {
					baseType->_udt = (*it);
				}
				break;
			case SYNTAX::ClassDef:
				if (dynamic_cast<ClassDef*>(*it)->_id->_name == _currToken->value) {
					baseType->_udt = (*it);
				}
				break;
			default:
				// error out
				break;
			}
		}
	}
	++_currToken;
}

template <>
void DFA::production<Type>()
{
	Type* type = dynamic_cast<Type*>(_stack.top());
	if (type->_baseType == nullptr) {
		if (_currToken->type == CONST) {
			type->_topConst = true;
			++_currToken;
		}
		type->_baseType = new BaseType;
		_stack.push(type->_baseType);
	}
	else {
		if (_currToken->type == MULTI) {
			++(type->_ptrCnt);
			++_currToken;
		}
		else {
			if (_currToken->type == CONST) {
				type->_innerConst = true;
				++_currToken;

			}
			_stack.pop();
		}
	}
}

template <>
void DFA::production<BinOp>()
{
	BinOp* binOp = dynamic_cast<BinOp*>(_stack.top());
	_stack.pop();
	switch (_currToken->type) {
	case ADD:
		binOp->_type = BinOp::ADD;
		break;
	case MINUS:
		binOp->_type = BinOp::MINUS;
		break;
	case MULTI:
		binOp->_type = BinOp::MULTI;
		break;
	case DIV:
		binOp->_type = BinOp::DIV;
		break;
	case DOT:
		binOp->_type = BinOp::MEMBER;
		break;
	case LESS:
		if (lookAhead(1)->type == EQUAL) {
			binOp->_type = BinOp::LESSEQ;
			_currToken = ++_currToken;
		}
		else {
			binOp->_type = BinOp::LESS;
		}
		break;
	case GREATER:
		if (lookAhead(1)->type == EQUAL) {
			binOp->_type = BinOp::GREATEREQ;
			_currToken = ++_currToken;
		}
		else {
			binOp->_type = BinOp::GREATER;
		}
		break;
	case EQUAL:
		binOp->_type = BinOp::EQUAL;
		break;
	}
	++_currToken;
}

template <>
void DFA::production<DeclList>()
{
	DeclList* declList = dynamic_cast<DeclList*>(_stack.top());
	if (!declList->_open) {
		declList->_open = true;
		VarDef* varDef = new VarDef;
		_stack.push(varDef);
		declList->_decls.push_back(varDef);
	}
	else {
		if (_currToken->type == PARENTHESES_RIGHT) {
			_stack.pop();
		}
		else {
			VarDef* varDef = new VarDef;
			_stack.push(varDef);
			declList->_decls.push_back(varDef);
			PLACE_HOLDER(KW_COMMA);
		}
	}
}

template <>
void DFA::production<ParamList>()
{
	ParamList* paramList = dynamic_cast<ParamList*>(_stack.top());
	if (!paramList->_open) {
		paramList->_open = true;
		Expr2* expr = new Expr2;
		_stack.push(expr);
		paramList->_params.push_back(expr);
	}
	else {
		if (_currToken->type == PARENTHESES_RIGHT) {
			_stack.pop();
			++_currToken;
		}
		else {
			Expr2* expr = new Expr2;
			_stack.push(expr);
			paramList->_params.push_back(expr);
			PLACE_HOLDER(KW_COMMA);
		}
	}
}

template <>
void DFA::production<Identifier>()
{
	Identifier* id = dynamic_cast<Identifier*>(_stack.top());
	_stack.pop();
	id->_name = (_currToken++)->value;
}

template <>
void DFA::production<Value>()
{
	Value* value = dynamic_cast<Value*>(_stack.top());
	_stack.pop();
	std::string str(_currToken->value);
	switch (_currToken->type) {
	case STR:
		value->_type = SYNTAX::VAL_CHAR;
		value->_c = str;
		break;
	case DEC:
		value->_type = SYNTAX::VAL_INT;
		value->_v._i = atof(str.c_str());	// todo: not correct
		break;
	case BIN:
	case OCT:
	case HEX:
	defualt:
		// not supported
		// error out
		break;
	}
	++_currToken;
}
