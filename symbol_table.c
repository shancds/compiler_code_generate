

#include "symbol_table.h"
#include "ast.h"
#include "error.h"


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


SymbolTable *create_symbol_table(SymbolTable *parent)
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->scope_level = parent ? parent->scope_level + 1 : 0;
    table->symbols = NULL;
    table->parent = parent;
    return table;
}


void free_symbol_tables(SymbolTable *table)
{
    if (!table)
        return;

    
    Symbol *sym = table->symbols;
    while (sym)
    {
        Symbol *temp = sym;
        sym = sym->next;

        
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

        
        if (temp->kind == SYMBOL_FUNC && temp->function_scope)
        {
            free_symbol_tables(temp->function_scope);
        }

        free(temp->name);
        free(temp->type);
        free(temp);
    }

    
    free(table);
}


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


void insert_symbol(SymbolTable *table, Symbol *symbol)
{
    symbol->next = table->symbols;
    table->symbols = symbol;
}


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
    return NULL; 
}


void print_symbol_table(SymbolTable *table, int indent, FILE *file)
{
    if (!table)
        return;

    Symbol *sym = table->symbols;

    if (!sym)
        return;
        
    
    // fprintf(file, "\n");
    for (int i = 0; i < indent; i++)
        fprintf(file, "    ");

    fprintf(file, "Scope Level %d:\n", table->scope_level);

    

    while (sym)
    {
        
        for (int i = 0; i < indent + 1; i++)
            fprintf(file, "    ");
        fprintf(file, "Name: %s, Type: %s, Kind: %s\n", sym->name, sym->type, get_symbol_kind_name(sym->kind));

        
        if (sym->kind == SYMBOL_FUNC)
        {
            
            Symbol *param = sym->params;
            while (param)
            {
                for (int i = 0; i < indent + 2; i++)
                    fprintf(file, "    "); 
                fprintf(file, "Param: %s, Type: %s\n", param->name, param->type);
                param = param->next;
            }

            
            if (sym->function_scope)
            {
                print_symbol_table(sym->function_scope, indent + 2, file);
            }
        }
        if (sym->kind == SYMBOL_CLASS)
        {
            
            Symbol *param = sym->params;
            while (param)
            {
                for (int i = 0; i < indent + 2; i++)
                    fprintf(file, "    "); 
                fprintf(file, "Param: %s, Type: %s\n", param->name, param->type);
                param = param->next;
            }

            
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


void build_symbol_table(ASTNode *node, SymbolTable *current_table, Symbol *current_function)
{
    if (!node)
        return;

    switch (node->type)
    {
    case NODE_CLASS_DECL:
    {
        
        const char *class_name = node->data.class_decl.name;

        
        Symbol *existingSymbol = lookup_symbol(current_table, class_name);
        if (existingSymbol != NULL)
        {
            fprintf(errorMessageFile, "Error: Attempt to redefine class '%s' in row:%d col:%d\n", class_name, node->locationY, node->locationX);
            break;
        }

        
        Symbol *symbol = create_symbol(
            class_name,
            class_name, 
            SYMBOL_CLASS,
            current_table->scope_level);

        
        insert_symbol(current_table, symbol);

        
        
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

        
        ASTNode *member_node = node->data.class_decl.members;
        while (member_node)
        {
            if (member_node->type == NODE_VISIBILITY && member_node->next->type == NODE_VAR_DECL)
            {
                
                Symbol *existingSymbol = lookup_symbol(class_scope, member_node->next->data.var_decl.name);
                if (existingSymbol != NULL)
                {
                    fprintf(errorMessageFile, "Error: The variable '%s' is already declared in row:%d col:%d\n", member_node->next->data.var_decl.name, member_node->next->locationY, member_node->next->locationX);
                }
                else
                {
                    
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

                    
                    Symbol *existingSymbol = lookup_symbol(current_table, member_node->next->data.var_decl.name);
                    if (existingSymbol != NULL)
                    {
                        fprintf(errorMessageFile, "Error: The variable '%s' is already declared in row:%d col:%d\n", member_node->next->data.var_decl.name, member_node->next->locationY, member_node->next->locationX);
                        break;
                    }

                    
                    Symbol *symbol = create_symbol(
                        member_node->next->data.var_decl.name,
                        type_name,
                        SYMBOL_VAR,
                        class_scope->scope_level);

                    insert_symbol(class_scope, symbol);
                }
            }

            
            member_node = member_node->next->next;
        }

        symbol->class_scope = class_scope;
        break;
    }

    case NODE_IMPL_DEF:
    {
        
        const char *type_name = node->data.impl_def.name;

        
        Symbol *type_symbol = lookup_symbol(current_table, type_name);
        if (!type_symbol)
        {
            fprintf(errorMessageFile, "Error: Undefined type '%s' in implementation in row:%d col:%d\n", type_name, node->locationY, node->locationX);
            break;
        }

        
        if (type_symbol->kind != SYMBOL_CLASS /* && type_symbol->kind != SYMBOL_INTERFACE */)
        {
            fprintf(errorMessageFile, "Error: Type '%s' is not a class or interface in row:%d col:%d\n", type_name, node->locationY, node->locationX);
            break;
        }

        
        ASTNode *func_node = node->data.impl_def.methods;

        
        build_symbol_table(func_node, type_symbol->class_scope, NULL);
        func_node = func_node->next;

        break;
    }

    
    case NODE_VAR_DECL:
    {
        
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

        
        Symbol *existingSymbol = lookup_symbol(current_table, node->data.variable.name);
        if (existingSymbol != NULL)
        {
            fprintf(errorMessageFile, "Error: The variable '%s' is already declared in row:%d col:%d\n", node->data.variable.name, node->locationY, node->locationX);
            break;
        }

        
        Symbol *symbol = create_symbol(
            node->data.var_decl.name,
            type_name,
            SYMBOL_VAR,
            current_table->scope_level);

        insert_symbol(current_table, symbol);
        break;
    }

    
    case NODE_FUNC_DECL:
    {
        
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
        
        

        
        Symbol *existingSymbol = lookup_symbol(current_table, node->data.func_decl.name);
        if (existingSymbol != NULL)
        {
            fprintf(errorMessageFile, "Error: Attempt to redifine the function '%s' in row:%d col:%d\n", node->data.variable.name, node->locationY, node->locationX);
            break;
        }

        
        Symbol *symbol = create_symbol(
            node->data.func_decl.name,
            return_type,
            SYMBOL_FUNC,
            current_table->scope_level);

        
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
            
            *param_list_tail = param_symbol;
            param_list_tail = &(param_symbol->next);
            param_node = param_node->next;
        }
        // symbol->params = param_list;

        
        SymbolTable *function_scope = create_symbol_table(current_table);

        
        Symbol *param_iter = param_list;
        while (param_iter)
        {
            
            Symbol *param_in_scope = create_symbol(
                param_iter->name,
                param_iter->type,
                SYMBOL_PARAM,
                function_scope->scope_level);
            insert_symbol(function_scope, param_in_scope);
            param_iter = param_iter->next;
        }

        
        symbol->function_scope = function_scope;

        
        insert_symbol(current_table, symbol);

        
        build_symbol_table(node->data.func_decl.body, function_scope, symbol);

        break;
    }

    
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
            node->data.variable.symbol = symbol; 
        }
        break;
    }

    
    case NODE_ASSIGN_STAT:
    {
        
        build_symbol_table(node->data.assign_stat.var, current_table, current_function);
        build_symbol_table(node->data.assign_stat.expr, current_table, current_function);

        
        const char *var_type = get_expression_type(node->data.assign_stat.var, current_table);
        const char *expr_type = get_expression_type(node->data.assign_stat.expr, current_table);
        if (!are_types_compatible(var_type, expr_type) && var_type != NULL && expr_type != NULL)
        {
            fprintf(errorMessageFile, "Type error: Cannot assign '%s' to variable of type '%s' in row:%d col:%d\n", expr_type, var_type, node->locationY, node->locationX);
        }
        break;
    }

        
    case NODE_RETURN_STAT:
    {
        
        build_symbol_table(node->data.return_stat.expr, current_table, current_function);

        
        const char *expr_type = get_expression_type(node->data.return_stat.expr, current_table);

        
        if (current_function == NULL)
        {
            fprintf(errorMessageFile, "Error: Return statement not inside a function in row:%d col:%d\n", node->locationY, node->locationX);
        }
        else
        {
            
            const char *func_return_type = current_function->type;

            
            if (!are_types_compatible(expr_type, func_return_type))
            {
                fprintf(errorMessageFile, "Type error: Return type '%s' does not match function return type '%s' in row:%d col:%d\n",
                        expr_type ? expr_type : "void",
                        func_return_type ? func_return_type : "void", node->locationY, node->locationX);
            }
        }
        break;
    }

    
    default:
    {
        
        if (node->data.op.left)
        {
            build_symbol_table(node->data.op.left, current_table, current_function);
        }
        if (node->data.op.right)
        {
            build_symbol_table(node->data.op.right, current_table, current_function);
        }
        break;
    }
    }

    
    if (node->next)
    {
        build_symbol_table(node->next, current_table, current_function);
    }
}

const char *get_expression_type(ASTNode *node, SymbolTable *current_table)
{
    if (!node)
        return NULL;

    switch (node->type)
    {
    
    case NODE_LITERAL_INT:
        return "integer";

    
    case NODE_LITERAL_FLOAT:
        return "float";

    
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

    
    case NODE_ADD:
    case NODE_SUBTRACT:
    case NODE_MULTIPLY:
    case NODE_DIVIDE:
    {
        
        const char *left_type = get_expression_type(node->data.op.left, current_table);
        const char *right_type = get_expression_type(node->data.op.right, current_table);

        
        if (!are_types_compatible(left_type, right_type))
        {
            fprintf(errorMessageFile, "Type error: Incompatible types '%s' and '%s' in arithmetic operation in row:%d col:%d\n", left_type, right_type, node->locationY, node->locationX);
            return NULL;
        }

        
        return left_type;
    }

    
    case NODE_AND:
    case NODE_OR:
    case NODE_EQ:
    case NODE_NEQ:
    case NODE_LT:
    case NODE_GT:
    case NODE_LEQ:
    case NODE_GEQ:
    {
        
        const char *left_type = get_expression_type(node->data.op.left, current_table);
        const char *right_type = get_expression_type(node->data.op.right, current_table);

        
        if (!are_types_compatible(left_type, right_type))
        {
            fprintf(errorMessageFile, "Type error: Incompatible types '%s' and '%s' in logical operation in row:%d col:%d\n", left_type, right_type, node->locationY, node->locationX);
            return NULL;
        }

        
        return "integer";
    }

    
    case NODE_NOT:
    {
        const char *expr_type = get_expression_type(node->data.op.left, current_table);
        
        
        if (node->type == NODE_NOT && strcmp(expr_type, "bool") != 0)
        {
            fprintf(errorMessageFile, "Type error: 'not' operator requires a boolean expression in row:%d col:%d\n", node->locationY, node->locationX);
            return NULL;
        }
        return expr_type;
    }

    
    case NODE_FUNC_CALL:
    {
        Symbol *symbol = lookup_symbol(current_table, node->data.func_call.name);
        if (!symbol || (symbol->kind != SYMBOL_FUNC && symbol->kind != SYMBOL_CLASS))
        {
            fprintf(errorMessageFile, "Error: Undeclared function '%s' in row:%d col:%d\n", node->data.func_call.name, node->locationY, node->locationX);
            return NULL;
        }

        
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

        
        if (arg_node || param_symbol)
        {
            fprintf(errorMessageFile, "Error: Mismatched number of arguments in function call to '%s' in row:%d col:%d\n", symbol->name, node->locationY, node->locationX);
            return NULL;
        }

        
        return symbol->type;
    }

        
        

    default:
        fprintf(errorMessageFile, "Error: Unsupported expression type in get_expression_type in row:%d col:%d\n", node->locationY, node->locationX);
        return NULL;
    }
}

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

    
    return strcmp(type1, type2) == 0;
}
