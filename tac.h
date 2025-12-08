#ifndef TAC_
#define TAC_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

typedef struct TAC
{
    char *instruction;
    struct TAC *next;
} TAC;

TAC *create_tac(char *instruction);
void append_tac(TAC **tac_list, TAC *new_tac);
void print_tac(TAC** tac_list, FILE *file) ;
char *generate_tac_recursive(TAC **tac_list, ASTNode *node, int indent);
void generate_assembly(TAC **tac_list, FILE *output_file);
void assemble_and_run(const char *asm_file, const char *output_file);
#endif