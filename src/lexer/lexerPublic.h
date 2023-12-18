#pragma once

#define MAX_TOKEN_LEN 128

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	FALSE = 0,
	TRUE = 1
} BOOL;

typedef enum {
	INVALID,
	// key words
	CHAR,				// char		1
	SHORT,				// short	2
	INT,				// int		3
	LONG,				// long		4
	FLOAT,				// float	5
	DOUBLE,				// double	6
	CLASS,				// class    7
	STRUCT,				// struct   8
	ADD,				// +		9
	MINUS,				// -		10
	MULTI,				// *		11
	DIV,				// /		12
	EQUAL,				// =		13
	GREATER,			// >		14
	LESS,				// <		15
	IF,					// if		16
	ELSE,				// else		17
	WHILE,				// while	18
	FOR,				// for		19
	PARENTHESES_LEFT,	// (		20
	PARENTHESES_RIGHT,	// )		21
	BRACKET_LEFT,		// [		22
	BRACKET_RIGHT,		// ]		23
	BRACE_LEFT,			// {		24
	BRACE_RIGHT,		// }		25
	
	// id
	ID,					// user defined identifier
	
	// value
	BIN,				// 0b...	27
	OCT,				// 0...		28
	DEC,				// ...		29
	HEX,				// 0x...	30
	SIGNED_BIN,			// -0b...	31
	SIGNED_OCT,			// -0...	32
	SIGNED_DEC,			// -...		33
	SIGNED_HEX,			// -0x...	34
	STR,				// "..."	35
	SEMICOLON,			// ;		36
	COMMENT,			// /**/ or //
	COMMA,				// ,		37
	DOT,				// .		38
	RETURN,				// return   39
	CONST				// const    40
}TokenType;

typedef struct {
	TokenType			type;
	char				value[MAX_TOKEN_LEN];
}Token;

// BOOL :		successfully parsed file or not
// fileName:	file to be parsed
// tokens:		memory to store parsed tokens
// maxToken:	capacity of memory to store
BOOL ParseFile(const char* fileName, Token* tokens, unsigned maxToken);

BOOL isKeywordType(TokenType);

#ifdef __cplusplus
	}
#endif