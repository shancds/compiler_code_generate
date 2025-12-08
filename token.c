#include "token.h"
#include "location.h"



void initializeTokenController()
{
    tokenOutFile = fopen("./output/tokens_01.txt", "w");
    if (tokenOutFile == NULL)
    {
        printf("Error opening file for writing!\n");
    }
    fprintf(tokenOutFile, "| %-20s + %-25s + %-6s + %-6s |\n", "--------------------","-------------------------", "------", "------");
    fprintf(tokenOutFile, "| %-20s | %-25s | %-6s | %-6s |\n", "Token","Lexeme", "Row", "Column");
    fprintf(tokenOutFile, "| %-20s + %-25s + %-6s + %-6s |\n", "--------------------","-------------------------", "------", "------");
}

void finalizeTokenController()
{ 
    fprintf(tokenOutFile, "| %-20s | %-25s | %-6s | %-6s |\n", "--------------------","-------------------------", "------", "------");

    if (tokenOutFile != NULL)
    {
        fclose(tokenOutFile);
    }
}

void getTokens(char *tokenName, char *lexeme)
{
    fprintf(tokenOutFile, "| %-20s | %-25s | %-6d | %-6d |\n", tokenName, lexeme, getLocationY(), getLocationX());
}