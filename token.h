#ifndef TOKEN_CONTROLLER
#define TOKEN_CONTROLLER
#include <stdio.h>

extern FILE *tokenOutFile;
void getTokens(char *tokenName, char *lexeme);
void initializeTokenController();
void finalizeTokenController();

#endif