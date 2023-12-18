#pragma once
#include "lexerPublic.h"
#include <stdio.h>

BOOL fsm_run(FILE* srcFile, Token* tokens, unsigned maxToken);