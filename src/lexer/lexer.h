#pragma once
#include "lexerPublic.h"
#include <string.h>

// internal data and functions
typedef enum {
	ST_START = 0,
	ST_ID,
	ST_BIN_OCT_HEX_ZERO,
	ST_BIN_SIGN,
	ST_HEX_SIGN,
	ST_BIN,
	ST_OCT,
	ST_HEX,
	ST_DEC,
	ST_PARENTHESE_LEFT,
	ST_PARENTHESE_RIGHT,
	ST_BRACKET_LEFT,
	ST_BRACKET_RIGHT,
	ST_BRACE_LEFT,
	ST_BRACE_RIGHT,
	ST_ADD,
	ST_MINUS,
	ST_MULTI,
	ST_DIV,
	ST_EQU,
	ST_SEMICOLON,
	ST_UNCLOSE_STR,
	ST_STR,
	ST_SLASH_STAR,
	ST_SLASH_STAR_STAR,
	ST_MULTI_LINE_COMMENT,
	ST_SLASH_SLASH,
	ST_SINGLE_LINE_COMMENT,
	ST_COMMA,
	ST_LESS,
	ST_GREATER,
	ST_DOT,
	ST_UNRECOG,
	TOTAL_STATUS
} STATUS;

typedef STATUS	(*transitHandler)	(char);
extern transitHandler _transit[TOTAL_STATUS];
extern TokenType 	  _typeTable[TOTAL_STATUS];

STATUS transit_start(char);
STATUS transit_ID(char);
STATUS transit_BIN_OCT_HEX_ZERO(char);
STATUS transit_BIN_SIGN(char);
STATUS transit_HEX_SIGN(char);
STATUS transit_BIN(char);
STATUS transit_OCT(char);
STATUS transit_HEX(char);
STATUS transit_DEC(char);
STATUS transit_SINGLE_CHAR(char);
STATUS transit_UNCLOSE_STR(char);
STATUS transit_SLASH(char);
STATUS transit_SLASH_STAR(char);
STATUS transit_SLASH_STAR_STAR(char);
STATUS transit_SLASH_SLASH(char);

BOOL	isSplitter(char c);

BOOL	isAlpha(char);

BOOL	isUnderline(char);

BOOL	isBinary(char);

BOOL	isOctal(char);

BOOL	isDecimal(char);

BOOL	isHexadecimal(char);

BOOL	isKeyword(const char*);

TokenType signedType(TokenType);

BOOL	isComment(STATUS);
