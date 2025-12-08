/* symbol_table.h */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Enumeration for symbol kinds */
typedef enum {
    SYMBOL_VAR,
    SYMBOL_FUNC,
    SYMBOL_PARAM,
    SYMBOL_CLASS,
    SYMBOL_TYPEDEF
} SymbolKind;

/* Forward declaration of SymbolTable */
typedef struct SymbolTable SymbolTable;

/* Structure for a symbol (identifier) */
typedef struct Symbol {
    char *name;                /* Name of the symbol */
    char *type;                /* Data type (e.g., "int", "float") */
    SymbolKind kind;           /* Kind of symbol */
    int scope_level;           /* Scope level */
    struct Symbol *params;     /* Parameters (for functions) */
    struct Symbol *next;       /* Next symbol in the list */
    SymbolTable *function_scope; /* For function symbols */
    SymbolTable *class_scope; /* For struct symbols */
} Symbol;

/* Structure for the symbol table */
struct SymbolTable {
    int scope_level;           /* Current scope level */
    Symbol *symbols;           /* Linked list of symbols in this scope */
    SymbolTable *parent;       /* Parent symbol table (outer scope) */
};

/* Function prototypes */

/* Create a new symbol table */
SymbolTable* create_symbol_table(SymbolTable *parent);

/* Free all symbol tables */
void free_symbol_tables(SymbolTable *table);

/* Create a new symbol */
Symbol* create_symbol(const char *name, const char *type, SymbolKind kind, int scope_level);

/* Insert a symbol into the symbol table */
void insert_symbol(SymbolTable *table, Symbol *symbol);

/* Lookup a symbol in the symbol table */
Symbol* lookup_symbol(SymbolTable *table, const char *name);

/* Print the symbol table (for debugging) */
void print_symbol_table(SymbolTable *table, int indent, FILE *file);

/* Helper function to get symbol kind name */
const char* get_symbol_kind_name(SymbolKind kind);

/* Build the symbol table by traversing the AST */
void build_symbol_table_phase1(ASTNode *node, SymbolTable *current_table, Symbol *current_function);
void build_symbol_table_phase2(ASTNode *node, SymbolTable *current_table, Symbol *current_function);

/* Get the type of an expression */
const char *get_expression_type(ASTNode *node, SymbolTable *current_table);

/* Check if two types are compatible */
int are_types_compatible(const char *type1, const char *type2);

/* Get the type of an expression */
const char* get_expression_type(ASTNode *node, SymbolTable *current_table);

/* Get the type of an expression */
const char* get_expression_type(ASTNode *node, SymbolTable *current_table);
 

#endif /* SYMBOL_TABLE_H */
