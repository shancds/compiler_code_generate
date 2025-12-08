%{
#include "ast.h"
#include <stdio.h>   
#include <string.h>
#include "parser.tab.h"
#include "location.h"
#include "token.h"
#include "error.h"


enum yytokentype createToken(enum yytokentype type);

%}

%x COMMENT
%option noyywrap
%%

"class"                                 { return createToken(CLASS); }
"implement"                             { return createToken(IMPLEMENT); }
"func"                                  { return createToken(FUNC); }
"isa"                                   { return createToken(ISA); }
"public"                                { return createToken(PUBLIC); }
"private"                               { return createToken(PRIVATE); }
"let"                                   { return createToken(LET); }
"integer"                               { return createToken(INTEGER); }
"float"                                 { return createToken(FLOAT); }
"void"                                  { return createToken(VOID); }
"if"                                    { return createToken(IF); }
"then"                                  { return createToken(THEN); }
"else"                                  { return createToken(ELSE); }
"while"                                 { return createToken(WHILE); }
"read"                                  { return createToken(READ); }
"write"                                 { return createToken(WRITE); }
"return"                                { return createToken(RETURN); }
"not"                                   { return createToken(NOT); }
"self"                                  { return createToken(SELF); }
"construct"                             { return createToken(CONSTRCT); }
"local"                                 { return createToken(LOCAL); }
"attribute"                             { return createToken(ATTRIBUTE); }


":="                                    { return createToken(ASSIGNOP); }
"=="                                    { return createToken(EQ); }
"!="                                    { return createToken(NEQ); }
"<"                                     { return createToken(LT); }
">"                                     { return createToken(GT); }
"<="                                    { return createToken(LEQ); }
">="                                    { return createToken(GEQ); }
"+"                                     { return createToken(ADDOP); }
"-"                                     { return createToken(MINUOP); }
"or"                                    { return createToken(OROP); }
"*"                                     { return createToken(MULTOP); }
"/"                                     { return createToken(DIVOP); }
"and"                                   { return createToken(ANDOP); }

                                                                 
"{"                                     { return createToken(LEFT_CURLY_BREACKET); }
"}"                                     { return createToken(RIGHT_CURLY_BREACKET); }
"("                                     { return createToken(LEFT_CIR_BREACKET); }
")"                                     { return createToken(RIGHT_CIR_BREACKET); }
"["                                     { return createToken(LEFT_RECT_BREACKET); }
"]"                                     { return createToken(RIGHT_RECT_BREACKET); }
"."                                     { return createToken(DOT); }
","                                     { return createToken(COMMA); }
";"                                     { return createToken(SEMICOL); }
":"                                     { return createToken(COLON); }
"->"                                    { return createToken(ARROW); }
 


\/\/.*                                  { updateLocationY(yytext);locationX = 1; }       

^[ \t]*"/*"                             { updateLocationY(yytext), locationX = 1; BEGIN COMMENT; /* enter comment eating state */ }
^[ \t]*"/*".*"*/"[ \t]*\n               { updateLocationY(yytext), locationX = 1; }
<COMMENT>"*/"[ \t]*\n                   { updateLocationY(yytext), locationX = 1; BEGIN 0; }
<COMMENT>"*/"                           { updateLocationY(yytext), locationX = 1; BEGIN 0; }
<COMMENT>\n                             { updateLocationY(yytext), locationX = 1; }
<COMMENT>.\n                            { updateLocationY(yytext), locationX = 1; }
^[ \t]*\n                               { updateLocationY(yytext), locationX = 1; }
.+"/*".*"*/".*\n                        { updateLocationY(yytext), locationX = 1; }
.*"/*".*"*/".+\n                        { updateLocationY(yytext), locationX = 1; }
.+"/*".*\n                              { updateLocationY(yytext), locationX = 1; BEGIN COMMENT; } 

[a-zA-Z][a-zA-Z0-9_]*                   { return createToken(ID); }   

[0-9]+                                  { return createToken(INTLIT); }
[0-9]+"."[0-9]+                         { return createToken(FLOATLIT); }

 
\n                                      { locationY++; locationX = 1; }
[ \t\r]+                                { locationX += yyleng; }

.                                       { printf("Invalid token '%s' at line:%d, col:%d.", yytext, locationY, locationX); }

<<EOF>>                                 { return 0; }

%%


char* enumTokenToString(enum yytokentype type) {
    switch (type) {
        case CLASS:
            return "CLASS";
        case IMPLEMENT:
            return "IMPLEMENT";
        case FUNC:
            return "FUNC";
        case ISA:
            return "ISA";
        case PUBLIC:
            return "PUBLIC";
        case PRIVATE:
            return "PRIVATE";
        case LET:
            return "LET";
        case INTEGER:
            return "INTEGER";
        case FLOAT:
            return "FLOAT";
        case VOID:
            return "VOID";
        case IF:
            return "IF";
        case THEN:
            return "THEN";
        case ELSE:
            return "ELSE";
        case WHILE:
            return "WHILE";
        case READ:
            return "READ";
        case WRITE:
            return "WRITE";
        case RETURN:
            return "RETURN";
        case NOT:
            return "NOT";
        case SELF:
            return "SELF";
        case CONSTRCT:
            return "CONSTRCT";
        case LOCAL:
            return "LOCAL";
        case ATTRIBUTE:
            return "ATTRIBUTE";
        case LEFT_CURLY_BREACKET:
            return "LEFT_CURLY_BREACKET";
        case RIGHT_CURLY_BREACKET:
            return "RIGHT_CURLY_BREACKET";
        case LEFT_CIR_BREACKET:
            return "LEFT_CIR_BREACKET";
        case RIGHT_CIR_BREACKET:
            return "RIGHT_CIR_BREACKET";
        case LEFT_RECT_BREACKET:
            return "LEFT_RECT_BREACKET";
        case RIGHT_RECT_BREACKET:
            return "RIGHT_RECT_BREACKET";
        case DOT:
            return "DOT";
        case COMMA:
            return "COMMA";
        case SEMICOL:
            return "SEMICOL";
        case COLON:
            return "COLON";
        case ARROW:
            return "ARROW";
        case ASSIGNOP:
            return "ASSIGNOP";
        case EQ:
            return "EQ";
        case NEQ:
            return "NEQ";
        case LT:
            return "LT";
        case GT:
            return "GT";
        case LEQ:
            return "LEQ";
        case GEQ:
            return "GEQ";
        case ADDOP:
            return "ADDOP";
        case MINUOP:
            return "MINUOP";
        case OROP:
            return "OROP";
        case MULTOP:
            return "MULTOP";
        case DIVOP:
            return "DIVOP";
        case ANDOP:
            return "ANDOP";
        case ID:
            return "ID";
        case INTLIT:
            return "INTLIT";
        case FLOATLIT:
            return "FLOATLIT";
        default:
            return "Invalid token";
    }
}



enum yytokentype createToken(enum yytokentype type){
    switch(type){
        case ID:        
            yylval.sval = strdup(yytext);
            break;
        case INTLIT:   
           yylval.ival = atoi(yytext);
            break;
        case FLOATLIT: 
           yylval.fval = atof(yytext); 
            break;
        default:
            break;
    }

    updateLocationX(yytext);

    getTokens(enumTokenToString(type), yytext); 



    return type;
}
