#include "fsm.h"
#include "lexer.h"
#include <stdio.h>

static unsigned			parsedToken			= 0;
static BOOL				eof					= FALSE;
static BOOL				isSigned			= FALSE;
static STATUS			currStatus			= ST_START;
static Token*			result				= NULL;
static char				buf[MAX_TOKEN_LEN]	= { '\0' };
static unsigned			pos					= 0;

STATUS transit(STATUS status , char c)
{
	return _transit[status](c);
}

BOOL acceptChar(char c) {
	STATUS ret = transit(currStatus, c);
	if (ret != ST_UNRECOG) {
		buf[pos++] = c;
		currStatus = ret;
		return TRUE;
	}
	return FALSE;
}

BOOL acceptToken() {
	if (*buf == '\0') {
		return TRUE;	// nothing to parse
	}
	if (isComment(currStatus)) {
		goto RESET;	// comment need not to parse
	}
	else if (_typeTable[currStatus] == INVALID) {
		return FALSE;	// not a token
	}
	else {
		// store recognized token
		// todo, identify key word
		Token token;
		TokenType origType = _typeTable[currStatus];
		if (origType == ID) {
			TokenType kw = isKeyword(buf);
			if (kw != INVALID) {
				token.type = kw;
			}
			else {
				token.type = origType;
				strcpy(token.value, buf);
			}
		}
		else {
			if (isSigned == TRUE) {
				token.type = signedType(origType);
				strcpy(token.value, buf);
			}
			else {
				token.type = origType;
				strcpy(token.value, buf);
			}
		}
		result[parsedToken++] = token;
		
RESET:
		strset(buf, '\0');
		pos = 0;
		currStatus = ST_START;
		isSigned   = FALSE;
		return TRUE;
	}
}

BOOL fsm_run(FILE* srcFile, Token* tokens, unsigned maxToken)
{
	result = tokens;
// 当文件没有解析完，并且token没有填满，就持续地解析
	while (parsedToken != maxToken && !eof) {
		char c = fgetc(srcFile);
		BOOL ret = acceptChar(c);
		if (ret) {
			continue;
		}
		else {
			ret = acceptToken();
			if (!ret) {
				goto UNRECOGNIZE;
			}
			if (isSplitter(c)) {
				continue;
			}
			if (c == EOF) {
				eof = TRUE;
				break;
			}
			ret = acceptChar(c);
			if (!ret) {
				goto UNRECOGNIZE;
			}
		}
	}

	if (!eof) {
		printf("Buffer readched limit, token recognize stops\r\n");
	}
	printf("Token Recognized successfully\r\n");
	fclose(srcFile);

	return TRUE;

UNRECOGNIZE:
	printf("unrecognizable character after: %s\r\n", buf);
	fclose(srcFile);
	return FALSE;	
}