#pragma once

#include "treeNode.h"
#include "../lexer/lexerPublic.h"

template <typename T>
bool first(TokenType type);

template <>
bool first<BaseType>(TokenType type) {
	switch (type) {
	case CHAR:
	case SHORT:
	case INT:
	case LONG:
	case FLOAT:
	case DOUBLE:
		return true;
	default:
		return false;
	}
}

template <>
bool first<Type>(TokenType type) {
	return first<BaseType>(type);
}

template <>
bool first<FuncDecl>(TokenType type) {
	return first<BaseType>(type);
}

template <>
bool first<ClassDecl>(TokenType type) {
	return type == CLASS;
}

template <>
bool first<StructDecl>(TokenType type) {
	return type == STRUCT;
}

template <>
bool first<VarDef>(TokenType type) {
	return first<BaseType>(type);
}

template <>
bool first<FuncDef>(TokenType type) {
	return first<FuncDecl>(type);
}

template <>
bool first<ClassDef>(TokenType type) {
	return first<ClassDecl>(type);
}

template <>
bool first<StructDef>(TokenType type) {
	return first<StructDecl>(type);
}

template <>
bool first<FuncCall>(TokenType type) {
	return type == ID;
}

template <>
bool first<Assignment>(TokenType type) {
	return type == ID;
}

template <>
bool first<Conditon>(TokenType type) {
	return type == IF;
}

template <>
bool first<Loop>(TokenType type) {
	return type == FOR;
}

template <>
bool first<BinOp>(TokenType type) {
	switch (type) {
	case ADD:
	case MINUS:
	case MULTI:
	case DIV:
	case EQUAL:
	case GREATER:
	case LESS:
	case DOT:
		return true;
	default:
		return false;
	}
}

template <>
bool first<ParamList>(TokenType type) {
	return true;
}

template <>
bool first<DeclList>(TokenType type) {
	return true;
}

template <>
bool first<Factor>(TokenType type) {
	return true;
}

template <>
bool first<Expr0>(TokenType type) {
	return type == BIN ||
		type == OCT ||
		type == DEC ||
		type == HEX ||
		type == SIGNED_BIN ||
		type == SIGNED_OCT ||
		type == SIGNED_DEC ||
		type == SIGNED_HEX ||
		type == STR ||
		type == ID;
}

template <>
bool first<Expr1>(TokenType type) {
	if (type == ADD || type == MINUS) {
		return true;
	}
	return first<Expr0>(type);
}

template <>
bool first<Expr2>(TokenType type) {
	return first<Expr1>(type);
}

template <>
bool first<Identifier>(TokenType type) {
	return type == ID;
}
