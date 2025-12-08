/* symbol_table.c */

#include "symbol_table.h"
#include "ast.h"
#include "error.h"

/* Helper function to get symbol kind name */
const char *get_symbol_kind_name(SymbolKind kind)
{
    switch (kind)
    {
    case SYMBOL_VAR:
        return "Variable";
    case SYMBOL_FUNC:
        return "Function";
    case SYMBOL_PARAM:
        return "Parameter";
    case SYMBOL_CLASS:
        return "Class";
    case SYMBOL_TYPEDEF:
        return "Typedef";
    default:
        return "Unknown";
    }
}

/* Create a new symbol table */
SymbolTable *create_symbol_table(SymbolTable *parent)
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->scope_level = parent ? parent->scope_level + 1 : 0;
    table->symbols = NULL;
    table->parent = parent;
    return table;
}

/* Free all symbol tables */
void free_symbol_tables(SymbolTable *table)
{
    if (!table)
        return;

    /* Free symbols in the current table */
    Symbol *sym = table->symbols;
    while (sym)
    {
        Symbol *temp = sym;
        sym = sym->next;

        /* Free parameters if any */
        if (temp->kind == SYMBOL_FUNC && temp->params)
        {
            Symbol *param = temp->params;
            while (param)
            {
                Symbol *temp_param = param;
                param = param->next;
                free(temp_param->name);
                free(temp_param->type);
                free(temp_param);
            }
        }

        /* Free function scope if any */
        if (temp->kind == SYMBOL_FUNC && temp->function_scope)
        {
            free_symbol_tables(temp->function_scope);
        }

        free(temp->name);
        free(temp->type);
        free(temp);
    }

    /* Free the current table */
    free(table);
}

/* Create a new symbol */
Symbol *create_symbol(const char *name, const char *type, SymbolKind kind, int scope_level)
{
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->type = strdup(type);
    symbol->kind = kind;
    symbol->scope_level = scope_level;
    symbol->params = NULL;
    symbol->next = NULL;
    symbol->function_scope = NULL;
    return symbol;
}

/* Insert a symbol into the symbol table */
void insert_symbol(SymbolTable *table, Symbol *symbol)
{
    symbol->next = table->symbols;
    table->symbols = symbol;
}

/* Lookup a symbol in the symbol table */
Symbol *lookup_symbol(SymbolTable *table, const char *name)
{
    SymbolTable *current = table;
    while (current)
    {
        Symbol *sym = current->symbols;
        while (sym)
        {
            if (strcmp(sym->name, name) == 0)
            {
                return sym;
            }
            sym = sym->next;
        }
        current = current->parent;
    }
    return NULL; /* Not found */
}

/* Print the symbol table (for debugging) */
void print_symbol_table(SymbolTable *table, int indent, FILE *file)
{
    if (!table)
        return;

    Symbol *sym = table->symbols;

    if (!sym)
        return;
        
    /* Indentation for the current scope */
    // fprintf(file, "\n");
    for (int i = 0; i < indent; i++)
        fprintf(file, "    ");

    fprintf(file, "Scope Level %d:\n", table->scope_level);

    /* Print symbols in the current scope */

    while (sym)
    {
        /* Indent symbols one level more */
        for (int i = 0; i < indent + 1; i++)
            fprintf(file, "    ");
        fprintf(file, "Name: %s, Type: %s, Kind: %s\n", sym->name, sym->type, get_symbol_kind_name(sym->kind));

        /* If the symbol is a function, print its parameters and its scope */
        if (sym->kind == SYMBOL_FUNC)
        {
            /* Print parameters */
            Symbol *param = sym->params;
            while (param)
            {
                for (int i = 0; i < indent + 2; i++)
                    fprintf(file, "    "); /* Indent parameters two levels more */
                fprintf(file, "Param: %s, Type: %s\n", param->name, param->type);
                param = param->next;
            }

            /* Print the function's scope */
            if (sym->function_scope)
            {
                print_symbol_table(sym->function_scope, indent + 2, file);
            }
        }
        if (sym->kind == SYMBOL_CLASS)
        {
            /* Print parameters */
            Symbol *param = sym->params;
            while (param)
            {
                for (int i = 0; i < indent + 2; i++)
                    fprintf(file, "    "); /* Indent parameters two levels more */
                fprintf(file, "Param: %s, Type: %s\n", param->name, param->type);
                param = param->next;
            }

            /* Print the class scope */
            if (sym->class_scope)
            {
                print_symbol_table(sym->class_scope, indent + 2, file);
            }
        }

        sym = sym->next;
        if (sym == NULL)
            fprintf(file, "\n");
    }
}

/* Build the symbol table by traversing the AST */
void build_symbol_table_phase1(ASTNode *node, SymbolTable *current_table, Symbol *current_function)
{
    if (!node)
        return;

    switch (node->type)
    {
    case NODE_CLASS_DECL:
    {
        /* Get the class name */
        const char *class_name = node->data.class_decl.name;

        /* Check for redefinition */
        Symbol *existingSymbol = lookup_symbol(current_table, class_name);
        if (existingSymbol != NULL)
        {
            fprintf(errorMessageFile, "Error: Attempt to redefine class '%s'\n in row:%d col:%d", class_name, node->locationY, node->locationX);
            break;
        }

        /* Create a symbol for the class */
        Symbol *symbol = create_symbol(
            class_name,
            class_name, /* Type name is the same as class name */
            SYMBOL_CLASS,
            current_table->scope_level);

        /* Insert the class symbol into the current symbol table */
        insert_symbol(current_table, symbol);

        /* Create a new scope for the class members (optional) */
        /* If you want class members to be in their own scope */
        SymbolTable *class_scope = create_symbol_table(current_table);

        // Process class inheritance
        if (node->data.class_decl.perent)
        {
            char *parent_name = node->data.class_decl.perent->data.name;
            Symbol *parentSymbol = lookup_symbol(current_table, parent_name);

            if (parent_name && parentSymbol->kind == SYMBOL_CLASS)
            {
                // Inherit fields and methods from the parent class
                Symbol *parent_member_symbol = parentSymbol->class_scope->symbols;
                while (parent_member_symbol)
                {
                    // Create a new symbol for each field/method and add it to the child class
                    Symbol *inherited_member = create_symbol(
                        parent_member_symbol->name,
                        parent_member_symbol->type,
                        parent_member_symbol->kind,
                        class_scope->scope_level);
                    insert_symbol(class_scope, inherited_member);

                    parent_member_symbol = parent_member_symbol->next;
                }
            }
            else
            {
                fprintf(errorMessageFile, "Error: Parent class '%s' not found for class '%s' in row:%d col:%d\n", parent_name, class_name, node->data.class_decl.perent->locationY, node->data.class_decl.perent->locationX);
            }
        }

        /* Process class members */
        ASTNode *member_node = node->data.class_decl.members;
        while (member_node)
        {
            if (member_node->type == NODE_VISIBILITY && member_node->next->type == NODE_VAR_DECL)
            {
                /* Check for redefinition */
                Symbol *existingSymbol = lookup_symbol(class_scope, member_node->next->data.var_decl.name);
                if (existingSymbol != NULL)
                {
                    fprintf(errorMessageFile, "Error: The variable '%s' is already declared in row:%d col:%d\n", member_node->next->data.var_decl.name, member_node->next->locationY, member_node->next->locationX);
                }
                else
                {
                    /* Get the type name from the type node */
                    const char *type_name = member_node->next->data.var_decl.type->data.type_node.type_name;

                    Symbol *typeSymbol = lookup_symbol(current_table, type_name);
                    if (!typeSymbol)
                    {
                        if (!(strcmp(type_name, "integer") == 0 ||
                              strcmp(type_name, "float") == 0))
                        {
                            fprintf(errorMessageFile, "Type Error: Invalid type '%s' in row:%d col:%d\n", type_name, node->locationY, node->locationX);
                            break;
                        }
                    }

                    /* Insert into the current symbol table */
                    Symbol *existingSymbol = lookup_symbol(current_table, member_node->next->data.var_decl.name);
                    if (existingSymbol != NULL)
                    {
                        fprintf(errorMessageFile, "Error: The variable '%s' is already declared in row:%d col:%d\n", member_node->next->data.var_decl.name, member_node->next->locationY, member_node->next->locationX);
                        break;
                    }

                    /* Create a symbol for the variable */
                    Symbol *symbol = create_symbol(
                        member_node->next->data.var_decl.name,
                        type_name,
                        SYMBOL_VAR,
                        class_scope->scope_level);

                    insert_symbol(class_scope, symbol);
                }
            }

            /* Each member is a variable declaration */
            member_node = member_node->next->next;
        }

        symbol->class_scope = class_scope;
        break;
    }

    case NODE_IMPL_DEF:
    {
        /* Get the type name */
        const char *type_name = node->data.impl_def.name;

        /* Lookup the type to ensure it exists */
        Symbol *type_symbol = lookup_symbol(current_table, type_name);
        if (!type_symbol)
        {
            fprintf(errorMessageFile, "Error: Undefined type '%s' in implementation in row:%d col:%d\n", type_name, node->locationY, node->locationX);
            break;
        }

        /* Ensure that the type is a class (or interface if applicable) */
        if (type_symbol->kind != SYMBOL_CLASS /* && type_symbol->kind != SYMBOL_INTERFACE */)
        {
            fprintf(errorMessageFile, "Error: Type '%s' is not a class or interface in row:%d col:%d\n", type_name, node->locationY, node->locationX);
            break;
        }

        /* Process the functions defined in the implementation */
        ASTNode *func_node = node->data.impl_def.methods;

        /* Each function is processed as usual */
        build_symbol_table_phase1(func_node, type_symbol->class_scope, NULL);
        func_node = func_node->next;

        break;
    }

    /* Handle variable declarations */
    case NODE_VAR_DECL:
    {
        /* Get the type name from the type node */
        const char *type_name = node->data.var_decl.type->data.type_node.type_name;

        Symbol *typeSymbol = lookup_symbol(current_table, type_name);
        if (!typeSymbol)
        {
            if (!(strcmp(type_name, "integer") == 0 ||
                  strcmp(type_name, "float") == 0))
            {
                fprintf(errorMessageFile, "Type Error: Invalid type '%s' in row:%d col:%d\n", type_name, node->locationY, node->locationX);
                break;
            }
        }

        /* Insert into the current symbol table */
        Symbol *existingSymbol = lookup_symbol(current_table, node->data.variable.name);
        if (existingSymbol != NULL)
        {
            fprintf(errorMessageFile, "Error: The variable '%s' is already declared in row:%d col:%d\n", node->data.variable.name, node->locationY, node->locationX);
            break;
        }

        /* Create a symbol for the variable */
        Symbol *symbol = create_symbol(
            node->data.var_decl.name,
            type_name,
            SYMBOL_VAR,
            current_table->scope_level);

        insert_symbol(current_table, symbol);
        break;
    }

    /* Handle function declarations */
    case NODE_FUNC_DECL:
    {
        /* Get the return type name */
        const char *return_type = node->data.func_decl.return_type
                                      ? node->data.func_decl.return_type->data.type_node.type_name
                                      : "void";

        if (
            !(strcmp(return_type, "void") == 0 ||
              strcmp(return_type, "integer") == 0 ||
              strcmp(return_type, "float") == 0))
        {
            fprintf(errorMessageFile, "Type Error: Invalid type '%s' in row:%d col:%d\n", return_type, node->locationY, node->locationX);
            break;
        }
        
        

        /* Insert into the current symbol table */
        Symbol *existingSymbol = lookup_symbol(current_table, node->data.func_decl.name);
        if (existingSymbol != NULL)
        {
            fprintf(errorMessageFile, "Error: Attempt to redifine the function '%s' in row:%d col:%d\n", node->data.variable.name, node->locationY, node->locationX);
            break;
        }

        /* Create a symbol for the function */
        Symbol *symbol = create_symbol(
            node->data.func_decl.name,
            return_type,
            SYMBOL_FUNC,
            current_table->scope_level);

        /* Handle function parameters */
        ASTNode *param_node = node->data.func_decl.params;
        Symbol *param_list = NULL;
        Symbol **param_list_tail = &param_list;
        while (param_node)
        {
            const char *param_type = param_node->data.param_decl.type->data.type_node.type_name;
            Symbol *param_symbol = create_symbol(
                param_node->data.param_decl.name,
                param_type,
                SYMBOL_PARAM,
                current_table->scope_level);
            /* Add to the end of the parameter list */
            *param_list_tail = param_symbol;
            param_list_tail = &(param_symbol->next);
            param_node = param_node->next;
        }
        // symbol->params = param_list;

        /* Create a new scope for the function */
        SymbolTable *function_scope = create_symbol_table(current_table);

        /* Add parameters to the function scope */
        Symbol *param_iter = param_list;
        while (param_iter)
        {
            /* Create a new symbol for the parameter in the function scope */
            Symbol *param_in_scope = create_symbol(
                param_iter->name,
                param_iter->type,
                SYMBOL_PARAM,
                function_scope->scope_level);
            insert_symbol(function_scope, param_in_scope);
            param_iter = param_iter->next;
        }

        /* Set the function's scope in the symbol */
        symbol->function_scope = function_scope;

        /* Insert function symbol into the current symbol table */
        insert_symbol(current_table, symbol);

        /* Traverse function body */
        build_symbol_table_phase1(node->data.func_decl.body, function_scope, symbol);

        break;
    }

    /* Handle variable usage */
    case NODE_VAR:
    {
        Symbol *symbol = lookup_symbol(current_table, node->data.variable.name);
        if (!symbol)
        {
            fprintf(errorMessageFile, "Error: Undeclared variable '%s' in row:%d col:%d\n", node->data.variable.name, node->locationY, node->locationX);
            break;
        }
        else
        {
            node->data.variable.symbol = symbol; /* Link the symbol to the AST node */
        }
        break;
    }

    /* Handle assignment statements */
    case NODE_ASSIGN_STAT:
    {
        /* Traverse the variable and expression */
        build_symbol_table_phase1(node->data.assign_stat.var, current_table, current_function);
        build_symbol_table_phase1(node->data.assign_stat.expr, current_table, current_function);

        /* Get types */
        const char *var_type = get_expression_type(node->data.assign_stat.var, current_table);
        const char *expr_type = get_expression_type(node->data.assign_stat.expr, current_table);
        if (!are_types_compatible(var_type, expr_type) && var_type != NULL && expr_type != NULL)
        {
            fprintf(errorMessageFile, "Type error: Cannot assign '%s' to variable of type '%s' in row:%d col:%d\n", expr_type, var_type, node->locationY, node->locationX);
        }
        break;
    }

        /* Handle return statements */
    case NODE_RETURN_STAT:
    {
        /* Traverse the return expression */
        build_symbol_table_phase1(node->data.return_stat.expr, current_table, current_function);

        /* Get the type of the return expression */
        const char *expr_type = get_expression_type(node->data.return_stat.expr, current_table);

        /* Check if we're inside a function */
        if (current_function == NULL)
        {
            fprintf(errorMessageFile, "Error: Return statement not inside a function in row:%d col:%d\n", node->locationY, node->locationX);
        }
        else
        {
            /* Get the function's declared return type */
            const char *func_return_type = current_function->type;

            /* Check if types are compatible */
            if (!are_types_compatible(expr_type, func_return_type))
            {
                fprintf(errorMessageFile, "Type error: Return type '%s' does not match function return type '%s' in row:%d col:%d\n",
                        expr_type ? expr_type : "void",
                        func_return_type ? func_return_type : "void", node->locationY, node->locationX);
            }
        }
        break;
    }

    /* Handle other nodes recursively */
    default:
    {
        /* Traverse child nodes if they exist */
        if (node->data.op.left)
        {
            build_symbol_table_phase1(node->data.op.left, current_table, current_function);
        }
        if (node->data.op.right)
        {
            build_symbol_table_phase1(node->data.op.right, current_table, current_function);
        }
        break;
    }
    }

    /* Traverse siblings */
    if (node->next)
    {
        build_symbol_table_phase1(node->next, current_table, current_function);
    }
}

void build_symbol_table_phase2(ASTNode *node, SymbolTable *current_table, Symbol *current_function) {}

/* Get the type of an expression */
const char *get_expression_type(ASTNode *node, SymbolTable *current_table)
{
    if (!node)
        return NULL;

    switch (node->type)
    {
    /* Handle integer literals */
    case NODE_LITERAL_INT:
        return "integer";

    /* Handle floating-point literals */
    case NODE_LITERAL_FLOAT:
        return "float";

    /* Handle variables */
    case NODE_VAR:
    {
        Symbol *symbol = lookup_symbol(current_table, node->data.variable.name);
        if (symbol)
        {
            return symbol->type;
        }
        else
        {
            return NULL;
        }
    }

    /* Handle binary arithmetic operations */
    case NODE_ADD:
    case NODE_SUBTRACT:
    case NODE_MULTIPLY:
    case NODE_DIVIDE:
    {
        /* Get types of left and right operands */
        const char *left_type = get_expression_type(node->data.op.left, current_table);
        const char *right_type = get_expression_type(node->data.op.right, current_table);

        /* Check if types are compatible */
        if (!are_types_compatible(left_type, right_type))
        {
            fprintf(errorMessageFile, "Type error: Incompatible types '%s' and '%s' in arithmetic operation in row:%d col:%d\n", left_type, right_type, node->locationY, node->locationX);
            return NULL;
        }

        /* Return the common type */
        return left_type;
    }

    /* Handle logical operations */
    case NODE_AND:
    case NODE_OR:
    case NODE_EQ:
    case NODE_NEQ:
    case NODE_LT:
    case NODE_GT:
    case NODE_LEQ:
    case NODE_GEQ:
    {
        /* Get types of left and right operands */
        const char *left_type = get_expression_type(node->data.op.left, current_table);
        const char *right_type = get_expression_type(node->data.op.right, current_table);

        /* Check if types are compatible */
        if (!are_types_compatible(left_type, right_type))
        {
            fprintf(errorMessageFile, "Type error: Incompatible types '%s' and '%s' in logical operation in row:%d col:%d\n", left_type, right_type, node->locationY, node->locationX);
            return NULL;
        }

        /* Logical operations result in int */
        return "integer";
    }

    /* Handle unary operations */
    case NODE_NOT:
    {
        const char *expr_type = get_expression_type(node->data.op.left, current_table);
        /* For arithmetic negation, type remains the same */
        /* For logical NOT, ensure the type is bool */
        if (node->type == NODE_NOT && strcmp(expr_type, "bool") != 0)
        {
            fprintf(errorMessageFile, "Type error: 'not' operator requires a boolean expression in row:%d col:%d\n", node->locationY, node->locationX);
            return NULL;
        }
        return expr_type;
    }

    /* Handle function calls */
    case NODE_FUNC_CALL:
    {
        Symbol *symbol = lookup_symbol(current_table, node->data.func_call.name);
        if (!symbol || (symbol->kind != SYMBOL_FUNC && symbol->kind != SYMBOL_CLASS))
        {
            fprintf(errorMessageFile, "Error: Undeclared function '%s' in row:%d col:%d\n", node->data.func_call.name, node->locationY, node->locationX);
            return NULL;
        }

        /* Check argument types */
        ASTNode *arg_node = node->data.func_call.params;
        Symbol *param_symbol = symbol->params;

        while (arg_node && param_symbol)
        {
            const char *arg_type = get_expression_type(arg_node, current_table);
            if (!are_types_compatible(arg_type, param_symbol->type))
            {
                fprintf(errorMessageFile, "Type error: Argument type '%s' does not match parameter type '%s' in function '%s' in row:%d col:%d\n", arg_type, param_symbol->type, symbol->name, node->locationY, node->locationX);
                return NULL;
            }
            arg_node = arg_node->next;
            param_symbol = param_symbol->next;
        }

        /* Check for mismatched number of arguments */
        if (arg_node || param_symbol)
        {
            fprintf(errorMessageFile, "Error: Mismatched number of arguments in function call to '%s' in row:%d col:%d\n", symbol->name, node->locationY, node->locationX);
            return NULL;
        }

        /* Return the return type of the function */
        return symbol->type;
    }

        /* Handle other expression types as needed */
        /* Add cases for array access, class fields, etc., if applicable */

    default:
        fprintf(errorMessageFile, "Error: Unsupported expression type in get_expression_type in row:%d col:%d\n", node->locationY, node->locationX);
        return NULL;
    }
}
/* Check if two types are compatible */
int are_types_compatible(const char *type1, const char *type2)
{
    if (!type1 || !type2)
        return 0;

    if (
        (strcmp(type1, "integer") == 0 || strcmp(type1, "float") == 0) &&
        (strcmp(type2, "integer") == 0 || strcmp(type2, "float") == 0))
    {
        return 1;
    }

    /* For simplicity, we consider types compatible if their names match */
    return strcmp(type1, type2) == 0;
}
