#include "ast.h"

ASTNode *create_literal_int(int value, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_LITERAL_INT;
    node->data.intval = value;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_literal_float(double value, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_LITERAL_FLOAT;
    node->data.floatval = value;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_variable(char *name, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_VAR;
    node->data.variable.name = strdup(name);
    node->data.variable.nested = NULL;
    node->data.variable.indices = NULL;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_func_call(char *name, ASTNode *params, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_FUNC_CALL;
    node->data.func_call.name = strdup(name);
    node->data.func_call.params = params;
    node->data.func_call.nested = NULL;
    node->locationY = locationY;
    node->locationX = locationX;
    node->next = NULL;
    return node;
}

ASTNode *create_assign_stat(ASTNode *var, ASTNode *expr, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN_STAT;
    node->data.assign_stat.var = var;
    node->data.assign_stat.expr = expr;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_if_stat(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_IF_STAT;
    node->data.if_stat.condition = condition;
    node->data.if_stat.then_branch = then_branch;
    node->data.if_stat.else_branch = else_branch;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_while_stat(ASTNode *condition, ASTNode *body, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_WHILE_STAT;
    node->data.while_stat.condition = condition;
    node->data.while_stat.body = body;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_func_decl(char *name, ASTNode *params, ASTNode *return_type, ASTNode *body, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_FUNC_DECL;
    node->data.func_decl.name = strdup(name);
    node->data.func_decl.params = params;
    node->data.func_decl.body = body;
    node->data.func_decl.return_type = return_type;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_var_decl(char *name, ASTNode *type, ASTNode *dimensions, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_VAR_DECL;
    node->data.var_decl.name = strdup(name);
    node->data.var_decl.type = type;
    node->data.var_decl.dimensions = dimensions;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_param_decl(char *name, ASTNode *type, ASTNode *dimensions, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_PARAM_DECL;
    node->data.param_decl.name = strdup(name);
    node->data.param_decl.type = type;
    node->data.param_decl.dimensions = dimensions;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_class_decl(char *name, ASTNode *members, ASTNode *parent, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_CLASS_DECL;
    node->data.class_decl.name = strdup(name);
    node->data.class_decl.members = members;
    node->data.class_decl.perent = parent;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_impl_def(char *name, ASTNode *methods, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_IMPL_DEF;
    node->data.impl_def.name = strdup(name);
    node->data.impl_def.methods = methods;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_return_stat(ASTNode *expr, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_RETURN_STAT;
    node->data.return_stat.expr = expr;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_io_stat(char *io_type, ASTNode *expr, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_IO_STAT; // Could create a separate type for IO if needed
    node->data.io_stat.io_type = strdup(io_type);
    node->data.io_stat.expr = expr;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_op_node(NodeType type, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_binary_op(NodeType type, ASTNode *left, ASTNode *right, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->data.op.op_type = type;
    node->data.op.left = left;
    node->data.op.right = right;
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_unary_op(NodeType type, ASTNode *operand, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->data.op.op_type = type;
    node->data.op.left = operand;
    node->data.op.right = NULL;
    node->locationY = locationY;
    node->locationX = locationX;
    node->next = NULL;
    return node;
}

ASTNode *create_visibility(char *visibility, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_VISIBILITY; // Or create a new type if needed
    node->data.visibility.visibility = strdup(visibility);
    node->locationX = locationX;
    node->locationY = locationY;
    node->next = NULL;
    return node;
}

ASTNode *create_type_node(char *type_name, int locationX, int locationY)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_TYPE_NODE; // Or create a new type if needed
    node->data.type_node.type_name = strdup(type_name);
    node->next = NULL;
    node->locationX = locationX;
    node->locationY = locationY;
    return node;
}

ASTNode *append_node(ASTNode *list, ASTNode *new_node)
{
    if (!list)
        return new_node;
    ASTNode *temp = list;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
    return list;
}

void free_ast(ASTNode *node)
{
    if (!node)
        return;
    free_ast(node->next);
    // Free other allocated memory depending on node type
    switch (node->type)
    {
    case NODE_VAR:
        free(node->data.variable.name);
        break;
    case NODE_FUNC_CALL:
        free(node->data.func_call.name);
        break;
    case NODE_FUNC_DECL:
        free(node->data.func_decl.name);
        break;
    // Handle other node types accordingly
    default:
        break;
    }
    free(node);
}

void print_ast_recursive(ASTNode *node, int indent);

void print_ast(ASTNode *node)
{
    print_ast_recursive(node, 0);
}

void print_ast_recursive(ASTNode *node, int indent)
{
    if (!node)
        return;

    while (node)
    {
        // Indentation for readability
        for (int i = 0; i < indent; i++)
        {
           // printf("  ");
        }

        switch (node->type)
        {
        case NODE_PROGRAM:
            printf("Program\n");
            print_ast_recursive(node->next, indent);
            break;

        case NODE_CLASS_DECL:
            printf("Class Declaration: %s\n", node->data.class_decl.name);
            print_ast_recursive(node->data.class_decl.members, indent + 1);
            break;

        case NODE_IMPL_DEF:
            printf("Implementation of: %s\n", node->data.impl_def.name);
            print_ast_recursive(node->data.impl_def.methods, indent + 1);
            break;

        case NODE_FUNC_DECL:
            printf("Function Declaration: %s\n", node->data.func_decl.name);
            if (node->data.func_decl.params)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Parameters:\n");
                print_ast_recursive(node->data.func_decl.params, indent + 2);
            }
            if (node->data.func_decl.return_type)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Return Type:\n");
                print_ast_recursive(node->data.func_decl.return_type, indent + 2);
            }
            if (node->data.func_decl.body)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Body:\n");
                print_ast_recursive(node->data.func_decl.body, indent + 2);
            }
            break;

        case NODE_VAR_DECL:
            printf("Variable Declaration: %s\n", node->data.var_decl.name);
            if (node->data.var_decl.type)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Type:\n");
                print_ast_recursive(node->data.var_decl.type, indent + 2);
            }
            if (node->data.var_decl.dimensions)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Dimensions:\n");
                print_ast_recursive(node->data.var_decl.dimensions, indent + 2);
            }
            break;

        case NODE_PARAM_DECL:
            printf("Parameter Declaration: %s\n", node->data.param_decl.name);
            if (node->data.param_decl.type)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Type:\n");
                print_ast_recursive(node->data.param_decl.type, indent + 2);
            }
            if (node->data.param_decl.dimensions)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Dimensions:\n");
                print_ast_recursive(node->data.param_decl.dimensions, indent + 2);
            }
            break;

        case NODE_ASSIGN_STAT:
            printf("Assignment Statement\n");
            for (int i = 0; i < indent + 1; i++)
                printf("  ");
            printf("Variable:\n");
            print_ast_recursive(node->data.assign_stat.var, indent + 2);
            for (int i = 0; i < indent + 1; i++)
                printf("  ");
            printf("Expression:\n");
            print_ast_recursive(node->data.assign_stat.expr, indent + 2);
            break;

        case NODE_IF_STAT:
            printf("If Statement\n");
            for (int i = 0; i < indent + 1; i++)
                printf("  ");
            printf("Condition:\n");
            print_ast_recursive(node->data.if_stat.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++)
                printf("  ");
            printf("Then Branch:\n");
            print_ast_recursive(node->data.if_stat.then_branch, indent + 2);
            if (node->data.if_stat.else_branch)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Else Branch:\n");
                print_ast_recursive(node->data.if_stat.else_branch, indent + 2);
            }
            break;

        case NODE_WHILE_STAT:
            printf("While Statement\n");
            for (int i = 0; i < indent + 1; i++)
                printf("  ");
            printf("Condition:\n");
            print_ast_recursive(node->data.while_stat.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++)
                printf("  ");
            printf("Body:\n");
            print_ast_recursive(node->data.while_stat.body, indent + 2);
            break;

        case NODE_RETURN_STAT:
            printf("Return Statement\n");
            if (node->data.return_stat.expr)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Expression:\n");
                print_ast_recursive(node->data.return_stat.expr, indent + 2);
            }
            break;

        case NODE_ADD:
        case NODE_SUBTRACT:
        case NODE_MULTIPLY:
        case NODE_DIVIDE:
        case NODE_OR:
        case NODE_AND:
        case NODE_EQ:
        case NODE_NEQ:
        case NODE_LT:
        case NODE_GT:
        case NODE_LEQ:
        case NODE_GEQ:
        case NODE_NOT:
        case NODE_POSITIVE:
        case NODE_NEGATIVE:
            printf("Expression: ");
            // Print operator
            switch (node->data.op.op_type)
            {
            case NODE_ADD:
                printf("Addition\n");
                break;
            case NODE_SUBTRACT:
                printf("Subtraction\n");
                break;
            case NODE_MULTIPLY:
                printf("Multiplication\n");
                break;
            case NODE_DIVIDE:
                printf("Division\n");
                break;
            case NODE_OR:
                printf("Logical OR\n");
                break;
            case NODE_AND:
                printf("Logical AND\n");
                break;
            case NODE_EQ:
                printf("Equal\n");
                break;
            case NODE_NEQ:
                printf("Not Equal\n");
                break;
            case NODE_LT:
                printf("Less Than\n");
                break;
            case NODE_GT:
                printf("Greater Than\n");
                break;
            case NODE_LEQ:
                printf("Less or Equal\n");
                break;
            case NODE_GEQ:
                printf("Greater or Equal\n");
                break;
            case NODE_NOT:
                printf("Logical NOT\n");
                break;
            case NODE_POSITIVE:
                printf("Unary Plus\n");
                break;
            case NODE_NEGATIVE:
                printf("Unary Minus\n");
                break;
            default:
                printf("Unknown Operator\n");
                break;
            }
            // Print operands
            if (node->data.op.left)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Left Operand:\n");
                print_ast_recursive(node->data.op.left, indent + 2);
            }
            if (node->data.op.right)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Right Operand:\n");
                print_ast_recursive(node->data.op.right, indent + 2);
            }
            break;

        case NODE_VAR:
            printf("Variable: %s\n", node->data.variable.name);
            if (node->data.variable.indices)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Indices:\n");
                print_ast_recursive(node->data.variable.indices, indent + 2);
            }
            if (node->data.variable.nested)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Nested Variables:\n");
                print_ast_recursive(node->data.variable.nested, indent + 2);
            }
            break;

        case NODE_FUNC_CALL:
            printf("Function Call: %s\n", node->data.func_call.name);
            if (node->data.func_call.params)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Arguments:\n");
                print_ast_recursive(node->data.func_call.params, indent + 2);
            }
            if (node->data.func_call.nested)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Nested Function Calls:\n");
                print_ast_recursive(node->data.func_call.nested, indent + 2);
            }
            break;

        case NODE_LITERAL_INT:
            printf("Integer Literal: %d\n", node->data.intval);
            break;

        case NODE_LITERAL_FLOAT:
            printf("Float Literal: %f\n", node->data.floatval);
            break;

        case NODE_TYPE_NODE:
            printf("Type: %s\n", node->data.type_node.type_name);
            break;

        case NODE_VISIBILITY:
            printf("Visibility: %s\n", node->data.visibility.visibility);
            break;

        case NODE_IO_STAT:
            printf("IO Statement: %s\n", node->data.io_stat.io_type);
            if (node->data.io_stat.expr)
            {
                for (int i = 0; i < indent + 1; i++)
                    printf("  ");
                printf("Expression:\n");
                print_ast_recursive(node->data.io_stat.expr, indent + 2);
            }
            break;

        default:
            printf("Unknown Node Type: %d\n", node->type);
            break;
        }

        node = node->next;
    }
}

void print_ast_recursive_to_file(ASTNode *node, int indent, FILE *file);
void print_ast_to_file(ASTNode *node, FILE *file)
{
    print_ast_recursive_to_file(node, 0, file);
}

void print_ast_recursive_to_file(ASTNode *node, int indent, FILE *file)
{
    if (!node)
        return;

    while (node)
    {
        // Indentation for readability
        for (int i = 0; i < indent; i++)
        {
            fprintf(file, "  ");
        }

        switch (node->type)
        {
        case NODE_PROGRAM:
            fprintf(file, "\nProgram\n");
            print_ast_recursive_to_file(node->next, indent, file);
            break;

        case NODE_CLASS_DECL:
            fprintf(file, "Class Declaration: %s\n", node->data.class_decl.name);
            print_ast_recursive_to_file(node->data.class_decl.members, indent + 1, file);
            break;

        case NODE_IMPL_DEF:
            fprintf(file, "Implementation of: %s\n", node->data.impl_def.name);
            print_ast_recursive_to_file(node->data.impl_def.methods, indent + 1, file);
            break;

        case NODE_FUNC_DECL:
            fprintf(file, "Function Declaration: %s\n", node->data.func_decl.name);
            if (node->data.func_decl.params)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Parameters:\n");
                print_ast_recursive_to_file(node->data.func_decl.params, indent + 2, file);
            }
            if (node->data.func_decl.return_type)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Return Type:\n");
                print_ast_recursive_to_file(node->data.func_decl.return_type, indent + 2, file);
            }
            if (node->data.func_decl.body)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Body:\n");
                print_ast_recursive_to_file(node->data.func_decl.body, indent + 2, file);
            }
            break;

        case NODE_VAR_DECL:
            fprintf(file, "Variable Declaration: %s\n", node->data.var_decl.name);
            if (node->data.var_decl.type)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Type:\n");
                print_ast_recursive_to_file(node->data.var_decl.type, indent + 2, file);
            }
            if (node->data.var_decl.dimensions)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Dimensions:\n");
                print_ast_recursive_to_file(node->data.var_decl.dimensions, indent + 2, file);
            }
            break;

        case NODE_PARAM_DECL:
            fprintf(file, "Parameter Declaration: %s\n", node->data.param_decl.name);
            if (node->data.param_decl.type)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Type:\n");
                print_ast_recursive_to_file(node->data.param_decl.type, indent + 2, file);
            }
            if (node->data.param_decl.dimensions)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Dimensions:\n");
                print_ast_recursive_to_file(node->data.param_decl.dimensions, indent + 2, file);
            }
            break;

        case NODE_ASSIGN_STAT:
            fprintf(file, "Assignment Statement\n");
            for (int i = 0; i < indent + 1; i++)
                fprintf(file, "  ");
            fprintf(file, "Variable:\n");
            print_ast_recursive_to_file(node->data.assign_stat.var, indent + 2, file);
            for (int i = 0; i < indent + 1; i++)
                fprintf(file, "  ");
            fprintf(file, "Expression:\n");
            print_ast_recursive_to_file(node->data.assign_stat.expr, indent + 2, file);
            break;

        case NODE_IF_STAT:
            fprintf(file, "If Statement\n");
            for (int i = 0; i < indent + 1; i++)
                fprintf(file, "  ");
            fprintf(file, "Condition:\n");
            print_ast_recursive_to_file(node->data.if_stat.condition, indent + 2, file);
            for (int i = 0; i < indent + 1; i++)
                fprintf(file, "  ");
            fprintf(file, "Then Branch:\n");
            print_ast_recursive_to_file(node->data.if_stat.then_branch, indent + 2, file);
            if (node->data.if_stat.else_branch)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Else Branch:\n");
                print_ast_recursive_to_file(node->data.if_stat.else_branch, indent + 2, file);
            }
            break;

        case NODE_WHILE_STAT:
            fprintf(file, "While Statement\n");
            for (int i = 0; i < indent + 1; i++)
                fprintf(file, "  ");
            fprintf(file, "Condition:\n");
            print_ast_recursive_to_file(node->data.while_stat.condition, indent + 2, file);
            for (int i = 0; i < indent + 1; i++)
                fprintf(file, "  ");
            fprintf(file, "Body:\n");
            print_ast_recursive_to_file(node->data.while_stat.body, indent + 2, file);
            break;

        case NODE_RETURN_STAT:
            fprintf(file, "Return Statement\n");
            if (node->data.return_stat.expr)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Expression:\n");
                print_ast_recursive_to_file(node->data.return_stat.expr, indent + 2, file);
            }
            break;

        case NODE_ADD:
        case NODE_SUBTRACT:
        case NODE_MULTIPLY:
        case NODE_DIVIDE:
        case NODE_OR:
        case NODE_AND:
        case NODE_EQ:
        case NODE_NEQ:
        case NODE_LT:
        case NODE_GT:
        case NODE_LEQ:
        case NODE_GEQ:
        case NODE_NOT:
        case NODE_POSITIVE:
        case NODE_NEGATIVE:
            fprintf(file, "Expression: ");
            // Print operator
            switch (node->data.op.op_type)
            {
            case NODE_ADD:
                fprintf(file, "Addition\n");
                break;
            case NODE_SUBTRACT:
                fprintf(file, "Subtraction\n");
                break;
            case NODE_MULTIPLY:
                fprintf(file, "Multiplication\n");
                break;
            case NODE_DIVIDE:
                fprintf(file, "Division\n");
                break;
            case NODE_OR:
                fprintf(file, "Logical OR\n");
                break;
            case NODE_AND:
                fprintf(file, "Logical AND\n");
                break;
            case NODE_EQ:
                fprintf(file, "Equal\n");
                break;
            case NODE_NEQ:
                fprintf(file, "Not Equal\n");
                break;
            case NODE_LT:
                fprintf(file, "Less Than\n");
                break;
            case NODE_GT:
                fprintf(file, "Greater Than\n");
                break;
            case NODE_LEQ:
                fprintf(file, "Less or Equal\n");
                break;
            case NODE_GEQ:
                fprintf(file, "Greater or Equal\n");
                break;
            case NODE_NOT:
                fprintf(file, "Logical NOT\n");
                break;
            case NODE_POSITIVE:
                fprintf(file, "Unary Plus\n");
                break;
            case NODE_NEGATIVE:
                fprintf(file, "Unary Minus\n");
                break;
            default:
                fprintf(file, "Unknown Operator\n");
                break;
            }
            // Print operands
            if (node->data.op.left)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Left Operand:\n");
                print_ast_recursive_to_file(node->data.op.left, indent + 2, file);
            }
            if (node->data.op.right)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Right Operand:\n");
                print_ast_recursive_to_file(node->data.op.right, indent + 2, file);
            }
            break;

        case NODE_VAR:
            fprintf(file, "Variable: %s\n", node->data.variable.name);
            if (node->data.variable.indices)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Indices:\n");
                print_ast_recursive_to_file(node->data.variable.indices, indent + 2, file);
            }
            if (node->data.variable.nested)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Nested Variables:\n");
                print_ast_recursive_to_file(node->data.variable.nested, indent + 2, file);
            }
            break;

        case NODE_FUNC_CALL:
            fprintf(file, "Function Call: %s\n", node->data.func_call.name);
            if (node->data.func_call.params)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Arguments:\n");
                print_ast_recursive_to_file(node->data.func_call.params, indent + 2, file);
            }
            if (node->data.func_call.nested)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Nested Function Calls:\n");
                print_ast_recursive_to_file(node->data.func_call.nested, indent + 2, file);
            }
            break;

        case NODE_LITERAL_INT:
            fprintf(file, "Integer Literal: %d\n", node->data.intval);
            break;

        case NODE_LITERAL_FLOAT:
            fprintf(file, "Float Literal: %f\n", node->data.floatval);
            break;

        case NODE_TYPE_NODE:
            fprintf(file, "Type: %s\n", node->data.type_node.type_name);
            break;

        case NODE_VISIBILITY:
            fprintf(file, "Visibility: %s\n", node->data.visibility.visibility);
            break;

        case NODE_IO_STAT:
            fprintf(file, "IO Statement: %s\n", node->data.io_stat.io_type);
            if (node->data.io_stat.expr)
            {
                for (int i = 0; i < indent + 1; i++)
                    fprintf(file, "  ");
                fprintf(file, "Expression:\n");
                print_ast_recursive_to_file(node->data.io_stat.expr, indent + 2, file);
            }
            break;

        default:
            fprintf(file, "Unknown Node Type: %d\n", node->type);
            break;
        }

        node = node->next;
    }
}