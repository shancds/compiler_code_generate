#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum
{
    NODE_PROGRAM,
    NODE_CLASS_DECL,
    NODE_IMPL_DEF,
    NODE_FUNC_DECL,
    NODE_VAR_DECL,
    NODE_PARAM_DECL,
    NODE_ASSIGN_STAT,
    NODE_IF_STAT,
    NODE_WHILE_STAT,
    NODE_RETURN_STAT,
    NODE_EXPR,
    NODE_VAR,
    NODE_FUNC_CALL,
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_ADD,
    NODE_SUBTRACT,
    NODE_MULTIPLY,
    NODE_DIVIDE,
    NODE_OR,
    NODE_AND,
    NODE_NOT,
    NODE_POSITIVE,
    NODE_NEGATIVE,
    NODE_EQ,
    NODE_NEQ,
    NODE_LT,
    NODE_GT,
    NODE_LEQ,
    NODE_GEQ,
    NODE_ASSIGN,
    NODE_VISIBILITY,
    NODE_IO_STAT,
    NODE_TYPE_NODE
} NodeType;


typedef struct ASTNode
{
    NodeType type;
    int locationX;
    int locationY;
    union
    {
        int intval;
        double floatval;
        char *name;
        struct
        {
            NodeType op_type;
            struct ASTNode *left;
            struct ASTNode *right;
        } op;
        struct
        {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stat;
        struct
        {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_stat;
        struct
        {
            struct ASTNode *var;
            struct ASTNode *expr;
        } assign_stat;
        struct
        {
            char *name;
            struct ASTNode *params;
            struct ASTNode *body;
            struct ASTNode *return_type;
        } func_decl;
        struct
        {
            char *name;
            struct ASTNode *params;
            struct ASTNode *nested;
        } func_call;
        struct
        {
            char *name;
            struct ASTNode *type;
            struct ASTNode *dimensions;
        } var_decl;
        struct
        {
            char *name;
            struct ASTNode *type;
            struct ASTNode *dimensions;
        } param_decl;
        struct
        {
            char *name;
            struct ASTNode *members;
            struct ASTNode *perent;
        } class_decl;
        struct
        {
            char *name;
            struct ASTNode *methods;
        } impl_def;
        struct
        {
            struct ASTNode *expr;
        } return_stat;
        struct
        {
            char *visibility;
        } visibility;
        struct
        {
            char *type_name;
        } type_node;
        struct
        {
            char *io_type;
            struct ASTNode *expr;
        } io_stat;
        struct
        {
            char *name; // Added 'name' member here
            struct ASTNode *nested;
            struct ASTNode *indices;
            struct Symbol *symbol;
        } variable;
    } data;
    struct ASTNode *next;
} ASTNode;

// Function prototypes
ASTNode *create_literal_int(int value, int locationX, int locationY);
ASTNode *create_literal_float(double value, int locationX, int locationY);
ASTNode *create_variable(char *name, int locationX, int locationY);
ASTNode *create_func_call(char *name, ASTNode *params, int locationX, int locationY);
ASTNode *create_assign_stat(ASTNode *var, ASTNode *expr, int locationX, int locationY);
ASTNode *create_if_stat(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int locationX, int locationY);
ASTNode *create_while_stat(ASTNode *condition, ASTNode *body, int locationX, int locationY);
ASTNode *create_func_decl(char *name, ASTNode *params, ASTNode *return_type, ASTNode *body, int locationX, int locationY);
ASTNode *create_var_decl(char *name, ASTNode *type, ASTNode *dimensions, int locationX, int locationY);
ASTNode *create_param_decl(char *name, ASTNode *type, ASTNode *dimensions, int locationX, int locationY);
ASTNode *create_class_decl(char *name, ASTNode *members, ASTNode *parent, int locationX, int locationY);
ASTNode *create_impl_def(char *name, ASTNode *methods, int locationX, int locationY);
ASTNode *create_return_stat(ASTNode *expr, int locationX, int locationY);
ASTNode *create_io_stat(char *io_type, ASTNode *expr, int locationX, int locationY);
ASTNode *create_op_node(NodeType type, int locationX, int locationY);
ASTNode *create_binary_op(NodeType type, ASTNode *left, ASTNode *right, int locationX, int locationY);
ASTNode *create_unary_op(NodeType type, ASTNode *operand, int locationX, int locationY);
ASTNode *create_visibility(char *visibility, int locationX, int locationY);
ASTNode *create_type_node(char *type_name, int locationX, int locationY);
ASTNode *append_node(ASTNode *list, ASTNode *new_node);
void free_ast(ASTNode *node);
void print_ast(ASTNode *node);
void print_ast_to_file(ASTNode *node, FILE *file);

#endif
