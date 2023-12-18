#pragma once
#include <vector>
#include <map>
#include "../lexer/lexerPublic.h"

/* supported grammers :
 *	 
 * Program		:=	(Statement)*
 * 
 * Statement	:=	FuncDecl <;>	|
 *					ClassDecl <;>	|
 *					StructDecl <;>	|
 *					VarDef <;>		|
 *					FuncDef <;>		|
 *					ClassDef <;>	|
 *					StructDef <;>   |
 *					FuncCall <;>	|
 *					Assignment <;>	|
 *					Conditon <;>	|
 *					Loop <;>		|
 * 
 * FuncDecl		:=	Type <id> <(> DeclList <)>
 * 
 * ClassDecl	:=	<Class> <id>
 * 
 * StructDecl	:=	<Struct> <id>
 * 
 * VarDef		:=	Type <id> [ <=> Expr2 ]
 *
 * FuncDef		:=	Type <id> <(> DeclList <)> <{> (Statement)* <}>
 * 
 * ClassDef		:=	<Class> <id> <{>  (	FuncDecl | VarDef | ClassDecl | StructDecl)* <}>
 * 
 * StructDef	:=	<Struct> <id> <{> (	FuncDecl | VarDef | ClassDecl | StructDecl)* <}>
 * 
 * FuncCall		:=	<id> <(> ParamList <)>
 *
 * Assignment	:=	<id> <=> Expr2
 * 
 * Condition	:=	<if> <(> Expr2 <)> <{> (Statement)* <}>
 * 
 * Loop			:=	<for> <(> (VarDef | Assignment) <;> Expr2 <;> (Statement)* <)> <{> (Statement)* <}>
 * 
 * Expr0		:=	Factor | FuncCall
 *
 * Expr1		:=  <+><+> Expr0 |
 *					<-><-> Expr0 |
 *					Expr0 <+><+> |
 *					Expr0 <-><-> |
 *					Expr0
 * 
 * Expr2		:=	Expr1 BinOp Expr1 | Expr1
 * 
 * BinOp		:=	<+> | <-> | <*> | </> | < > > | < < > | <==> | < >= > | < <= > | <.>
 * 
 * ParamList	:=	NULL				|
 *					Factor (<,> Factor)*
 * 
 * DeclList		:=	NULL				|
 *					VarDef (<,> VarDef)*
 * 
 * Type			:=	BaseType (<*>)*
 * 
 * 
 * BaseType		:=	<int>  |
 *					<bool> |
 *					<char> |
 *					<id>	//class or struct id 
 * 
 * Factor		:=	<id> | <Value> 
 * 
 */
namespace SYNTAX {

	typedef enum {
		Program = 0,
		Statement,
		FuncDecl,
		ClassDecl,
		StructDecl,
		VarDef,
		FuncDef,
		ClassDef,
		StructDef,
		FuncCall,
		Return,
		Assignment,
		Conditon,
		Loop,
		Expr0,
		Expr1,
		Expr2,
		BinOp,
		ParamList,
		DeclList,
		Type,
		BaseType,
		Factor,
		Identifier,
		Value,
		PlaceHolder,
	} ClassId;

	typedef enum {
		KW_CHAR,
		KW_SHORT,
		KW_INT,
		KW_LONG,
		KW_FLOAT,
		KW_DOUBLE,
		KW_ADD,
		KW_MINUS,
		KW_MULTI,
		KW_DIV,
		KW_IF,
		KW_ELSE,
		KW_WHILE,
		KW_FOR,
		KW_PARENTHESES_LEFT,
		KW_PARENTHESES_RIGHT,
		KW_BRACKET_LEFT,
		KW_BRACKET_RIGHT,
		KW_BRACE_LEFT,
		KW_BRACE_RIGHT,
		KW_SEMICOLON,
		KW_COMMA,
		KW_CLASS,
		KW_STRUCT,
		KW_EQUAL,
		KW_RETURN,
		KW_CONST
	} KeyWordType;

	typedef enum {
		VAL_CHAR,
		VAL_SHORT,
		VAL_INT,
		VAL_LONG,
		VAL_FLOAT,
		VAL_DOUBLE,
		VAL_INVALID,
	} ValueType;
	
	extern std::map<TokenType, KeyWordType> _kwMap;
}
