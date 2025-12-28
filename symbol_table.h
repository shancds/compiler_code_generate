

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


typedef enum {
    SYMBOL_VAR,
    SYMBOL_FUNC,
    SYMBOL_PARAM,
    SYMBOL_CLASS,
    SYMBOL_TYPEDEF
} SymbolKind;


typedef struct SymbolTable SymbolTable;


typedef struct Symbol {
    char *name;                
    char *type;                
    SymbolKind kind;           
    int scope_level;           
    struct Symbol *params;     
    struct Symbol *next;       
    SymbolTable *function_scope; 
    SymbolTable *class_scope; 
} Symbol;


struct SymbolTable {
    int scope_level;           
    Symbol *symbols;           
    SymbolTable *parent;       
};




SymbolTable* create_symbol_table(SymbolTable *parent);


void free_symbol_tables(SymbolTable *table);


Symbol* create_symbol(const char *name, const char *type, SymbolKind kind, int scope_level);


void insert_symbol(SymbolTable *table, Symbol *symbol);


Symbol* lookup_symbol(SymbolTable *table, const char *name);


void print_symbol_table(SymbolTable *table, int indent, FILE *file);


const char* get_symbol_kind_name(SymbolKind kind);


void build_symbol_table(ASTNode *node, SymbolTable *current_table, Symbol *current_function);


const char *get_expression_type(ASTNode *node, SymbolTable *current_table);


int are_types_compatible(const char *type1, const char *type2);


const char* get_expression_type(ASTNode *node, SymbolTable *current_table);


const char* get_expression_type(ASTNode *node, SymbolTable *current_table);
 

#endif 
