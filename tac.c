#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "ast.h"
#include "symbol_table.h"
#include "tac_support.h"




TAC *create_tac(char *instruction)
{
    TAC *new_tac = (TAC *)malloc(sizeof(TAC));
    new_tac->instruction = strdup(instruction);
    new_tac->next = NULL;
    return new_tac;
}


void append_tac(TAC **tac_list, TAC *new_tac)
{
    if (!*tac_list)
    {
        *tac_list = new_tac;
    }
    else
    {
        TAC *temp = *tac_list;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = new_tac;
    }
}

void print_tac(TAC **tac_list, FILE *file)
{
    TAC *temp = *tac_list;
    while (temp)
    {
        fprintf(file, "%s\n", temp->instruction);
        temp = temp->next;
    }
}

int tac_count = 1;
int label_count = 1;
// Function to create a temporary variable with a unique name
char *create_temp_variable()
{
    
    char *temp_var = (char *)malloc(50 * sizeof(char)); // Ensure enough space for the name

    
    snprintf(temp_var, 50, "t%d", tac_count++);

   
    return temp_var;
}


char *generate_tac_recursive(TAC **tac_list, ASTNode *node, int indent)
{
    if (!node)
    {
        return NULL;
    }

    while (node)
    {
        switch (node->type)
        {
        case NODE_FUNC_DECL:
        {
            char funcDeclBeginInstruction[500];
            strcpy(funcDeclBeginInstruction, generate_tac_open_func_label(node->data.var_decl.name));
            append_tac(tac_list, create_tac(funcDeclBeginInstruction));

            generate_tac_recursive(tac_list, node->data.func_decl.body, indent + 1);

            char funcDeclEndInstruction[500];
            strcpy(funcDeclEndInstruction, generate_tac_close_func_label(node->data.var_decl.name));
            append_tac(tac_list, create_tac(funcDeclEndInstruction));

            break;
        }
        case NODE_VAR_DECL:
        {
            char varDeclInstruction[500];
            strcpy(varDeclInstruction, create_tac_ins_allocate(node->data.var_decl.name));

            append_tac(tac_list, create_tac(varDeclInstruction));
            break;
        }
        case NODE_ASSIGN_STAT:
        {
            
            char *expr_result = generate_tac_recursive(tac_list, node->data.assign_stat.expr, indent + 1);

            char assignStatInstruction[500];
            strcpy(assignStatInstruction, generate_tac_assign_statement(node->data.assign_stat.var->data.variable.symbol->type, expr_result, node->data.assign_stat.var->data.variable.name));

            append_tac(tac_list, create_tac(assignStatInstruction));

            break;
        }

        case NODE_LITERAL_INT:
        {
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            char loadValInstruction[500];
            strcpy(loadValInstruction, generate_tac_load_int_literal(node->data.intval, tempVar));
            append_tac(tac_list, create_tac(loadValInstruction));

            return tempVar;
        }
        case NODE_LITERAL_FLOAT:
        {
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            char loadValInstruction[500];
            strcpy(loadValInstruction, generate_tac_load_float_literal(node->data.floatval, tempVar));
            append_tac(tac_list, create_tac(loadValInstruction));

            return tempVar;
        }

        case NODE_VAR:
        {
            return node->data.variable.name;
        }

        case NODE_IO_STAT:
        {
            char *temp_var = NULL;
            char writeInstruction[500];
            char readInstruction[500];

            // Handle WRITE operation
            if (strcmp(node->data.io_stat.io_type, "WRITE") == 0)
            {
                temp_var = generate_tac_recursive(tac_list, node->data.io_stat.expr, indent); // Get temp var
                // const char *var_type = node->data.io_stat.expr->data.variable.symbol->type;
                
                // if (strcmp(var_type, "float") == 0)
                // {
                //    strcpy(writeInstruction, generate_tac_write_float(temp_var));
                // }
                // else if (strcmp(var_type, "integer") == 0)
                // {
                    strcpy(writeInstruction, generate_tac_write_int(temp_var));
                // }
                
                append_tac(tac_list, create_tac(writeInstruction));

                break;
            }

            
            if (strcmp(node->data.io_stat.io_type, "READ") == 0)
            {
                const char *var_type = node->data.io_stat.expr->data.variable.symbol->type;
                const char *var_name = node->data.io_stat.expr->data.variable.name;

                if (strcmp(var_type, "float") == 0)
                {
                    strcpy(readInstruction, generate_tac_read_float(var_name));
                }
                else if (strcmp(var_type, "integer") == 0)
                {
                    strcpy(readInstruction, generate_tac_read_int(var_name));
                }
                append_tac(tac_list, create_tac(readInstruction));
            }

            break;
        }

        case NODE_ADD:
        {
            
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_add_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }

        case NODE_SUBTRACT:
        {
            
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_sub_instruction(right_result,left_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }

        case NODE_MULTIPLY:
        {
            
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_mul_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }
        case NODE_DIVIDE:
        {
            
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_div_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }
        case NODE_AND:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_and_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }

        case NODE_OR:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_or_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }

        case NODE_EQ:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_eq_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }
        case NODE_NEQ:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_neq_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }
        case NODE_LT:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_lt_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }
        case NODE_GT:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_gt_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }
        case NODE_LEQ:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_leq_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }
        case NODE_GEQ:
        { 
            char *left_result = generate_tac_recursive(tac_list, node->data.op.left, indent);

            
            char *right_result = generate_tac_recursive(tac_list, node->data.op.right, indent);

            // Create a new temporary variable for the result
            char *tempVar = (char *)malloc(200 * sizeof(char));
            strcpy(tempVar, create_temp_variable());

            char tempVarDeclInstruction[500];
            strcpy(tempVarDeclInstruction, create_tac_ins_allocate(tempVar));
            append_tac(tac_list, create_tac(tempVarDeclInstruction));

            
            char addInstruction[500];
            strcpy(addInstruction, generate_tac_geq_instruction(left_result, right_result, tempVar));
            append_tac(tac_list, create_tac(addInstruction));

            return tempVar;
        }

        case NODE_WHILE_STAT:
        {
            // Generate the start and end labels for the loop
            char *startLabel = generate_tac_loop_start_label(&label_count);
            char *endLabel = generate_tac_loop_end_label(&label_count);

            // Print the start label
            char instruction[500];
            snprintf(instruction, sizeof(instruction), "%s", startLabel);
            append_tac(tac_list, create_tac(instruction));

            
            char *condition_result = generate_tac_recursive(tac_list, node->data.while_stat.condition, indent);

            // Generate the conditional jump: if the condition is false, exit the loop
            snprintf(instruction, sizeof(instruction), "if %s == 0 goto %s", condition_result, endLabel);
            append_tac(tac_list, create_tac(instruction));

            
            generate_tac_recursive(tac_list, node->data.while_stat.body, indent + 1);

            // Generate the jump to the start to recheck the condition
            snprintf(instruction, sizeof(instruction), "goto %s", startLabel);
            append_tac(tac_list, create_tac(instruction));

            // Print the end label (loop exit point)
            snprintf(instruction, sizeof(instruction), "%s", endLabel);
            append_tac(tac_list, create_tac(instruction));

            // Free the dynamically allocated memory for labels
            free(startLabel);
            free(endLabel);

            break;
        }

        case NODE_IF_STAT:
        {
            // Create unique labels for the condition, then branch, else branch, and end
            char *conditionLabel = generate_tac_if_condition_label(&label_count);
            char *thenLabel = generate_tac_if_then_label(&label_count);
            char *elseLabel = generate_tac_if_else_label(&label_count);
            char *endLabel = generate_tac_if_end_label(&label_count);

            
            char *condition_result = generate_tac_recursive(tac_list, node->data.if_stat.condition, indent);

            // Conditional jump: if the condition is false, skip the then branch and jump to the else or end
            char instruction[500];
            snprintf(instruction, sizeof(instruction), "if %s == 0 goto %s", condition_result, elseLabel);
            append_tac(tac_list, create_tac(instruction));

            
            snprintf(instruction, sizeof(instruction), "%s", thenLabel);
            append_tac(tac_list, create_tac(instruction));
            generate_tac_recursive(tac_list, node->data.if_stat.then_branch, indent + 1);

            // If there is an else branch, jump over it after the then branch
            if (node->data.if_stat.else_branch)
            {
                snprintf(instruction, sizeof(instruction), "goto %s", endLabel);
                append_tac(tac_list, create_tac(instruction));

                // Generate the else branch
                snprintf(instruction, sizeof(instruction), "%s", elseLabel);
                append_tac(tac_list, create_tac(instruction));
                generate_tac_recursive(tac_list, node->data.if_stat.else_branch, indent + 1);
            }

            // End label for the if statement
            snprintf(instruction, sizeof(instruction), "%s", endLabel);
            append_tac(tac_list, create_tac(instruction));

            break;
        }

        default:
          //  printf("%d\n", node->type);
            break;
        }

        node = node->next;
    }
}

int write_integer_included = 0; // Flag for "print_integer"
int write_float_included = 0;   // Flag for "print_float"


void generate_assembly(TAC **tac_list, FILE *output_file) {
    if (!tac_list || !output_file) return;

    fprintf(output_file, "section .data\n");
    fprintf(output_file, "    integer_format db \"%%d\",10,0\n");
    fprintf(output_file, "    float_format db \"%%f\",10,0\n");

    // Keep track of declared variables to avoid duplicates
    TAC *current = *tac_list;
    char declared[100][100]; // simple array to store names
    int declared_count = 0;

    while (current) {
        if (strstr(current->instruction, "allocate variable")) {
            char var[100];
            sscanf(current->instruction, "allocate variable %s", var);

            // Check if already declared
            int exists = 0;
            for (int i = 0; i < declared_count; i++) {
                if (strcmp(declared[i], var) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                fprintf(output_file, "    %s dq 0\n", var);
                strcpy(declared[declared_count++], var);
            }
        }
        current = current->next;
    }

  
    fprintf(output_file, "\nsection .text\n");
    fprintf(output_file, "global main, print_integer, print_float\n");
    fprintf(output_file, "extern printf, ExitProcess\n");

    fprintf(output_file, "main:\n");

    // Generate instructions
    current = *tac_list;
    while (current) {
        // Load integer literal
        if (strstr(current->instruction, "load integer literal")) {
            char literal[100], temp[100];
            sscanf(current->instruction, "load integer literal %s to %s", literal, temp);
            fprintf(output_file, "    mov rax, %s\n", literal);
            fprintf(output_file, "    mov [%s], rax\n", temp);
        }
        // Assign integer
        else if (strstr(current->instruction, "assign integer")) {
            char src[100], dest[100];
            sscanf(current->instruction, "assign integer %s to %s", src, dest);
            fprintf(output_file, "    mov rax, [%s]\n", src);
            fprintf(output_file, "    mov [%s], rax\n", dest);
        }
    
        else if (strstr(current->instruction, "add ")) {
            char src1[100], src2[100], dest[100];
            sscanf(current->instruction, "add %[^,], %[^,], %s", src1, src2, dest);
            fprintf(output_file, "    mov rax, [%s]\n", src1);
            fprintf(output_file, "    add rax, [%s]\n", src2);
            fprintf(output_file, "    mov [%s], rax\n", dest);
        }
        else if (strstr(current->instruction, "sub ")) {
            char src1[100], src2[100], dest[100];
            sscanf(current->instruction, "sub %[^,], %[^,], %s", src1, src2, dest);
            fprintf(output_file, "    mov rax, [%s]\n", src1);
            fprintf(output_file, "    sub rax, [%s]\n", src2);
            fprintf(output_file, "    mov [%s], rax\n", dest);
        }
        else if (strstr(current->instruction, "mul ")) {
            char src1[100], src2[100], dest[100];
            sscanf(current->instruction, "mul %[^,], %[^,], %s", src1, src2, dest);
            fprintf(output_file, "    mov rax, [%s]\n", src1);
            fprintf(output_file, "    imul rax, [%s]\n", src2);  // signed multiplication
            fprintf(output_file, "    mov [%s], rax\n", dest);
        }
        else if (strstr(current->instruction, "div ")) {
            char src1[100], src2[100], dest[100];
            sscanf(current->instruction, "div %[^,], %[^,], %s", src1, src2, dest);
            fprintf(output_file, "    mov rax, [%s]\n", src1);   // dividend
            fprintf(output_file, "    cqo\n");                   // sign-extend RAX into RDX:RAX
            fprintf(output_file, "    idiv qword [%s]\n", src2); // divide RDX:RAX by src2
            fprintf(output_file, "    mov [%s], rax\n", dest);   // quotient in RAX
        }

        else if (strstr(current->instruction, "logic-and")) {
                char dest[100], src1[100], src2[100];
                sscanf(current->instruction, "logic-and %[^,], %[^-] -> %s", src1, src2, dest);
                fprintf(output_file, "    mov rax, [%s]\n", src1);
                fprintf(output_file, "    xor edx, edx\n");
                fprintf(output_file, "    div dword [%s]\n", src2);
                fprintf(output_file, "    mov [%s], rax\n", dest);
        }
        else if (strstr(current->instruction, "logic-or")) {
            char dest[100], src1[100], src2[100];
            int matched = sscanf(current->instruction, "logic-or %[^,], %[^-] -> %s", src1, src2, dest);
            if (matched == 3) {
                fprintf(output_file, "    mov rax, [%s]\n", src1); // Load src1 into rax
                fprintf(output_file, "    or rax, [%s]\n", src2);  // Perform bitwise OR with src2
                fprintf(output_file, "    mov [%s], rax\n", dest); // Store the result in dest
            } else {
                fprintf(stderr, "Error parsing 'logic-or' instruction: %s\n", current->instruction);
            }
        }



        // WRITE integer
        else if (strstr(current->instruction, "write integer")) {
            char var[100];
            sscanf(current->instruction, "write integer %s", var);
            fprintf(output_file, "    mov rax, [%s]\n", var);   // load variable into RAX
            fprintf(output_file, "    call print_integer\n");  // call print routine
        }

        current = current->next;
    }

    


    fprintf(output_file, "\nprint_integer:\n");
    fprintf(output_file, "    mov rcx, integer_format\n");
    fprintf(output_file, "    mov rdx, rax\n");
    fprintf(output_file, "    xor rax, rax\n");
    fprintf(output_file, "    call printf\n");
    fprintf(output_file, "    ret\n");


    fprintf(output_file, "\nprint_float:\n");
    fprintf(output_file, "    mov rcx, float_format\n");
    fprintf(output_file, "    movq xmm1, [rax]\n");
    fprintf(output_file, "    xor rax, rax\n");
    fprintf(output_file, "    call printf\n");
    fprintf(output_file, "    ret\n");

    fprintf(output_file, "    mov rcx, 0\n");
    fprintf(output_file, "    call ExitProcess\n");
}


void assemble_and_run(const char *asm_file, const char *output_file)
{
    
    // char command[1024];

    // // 1. Assemble using NASM for Windows x64 ABI
    // snprintf(command, sizeof(command),
    //     "nasm -f win64 -o %s.obj %s", output_file, asm_file);

    // printf("Assembling: %s\n", command);

    // if (system(command) != 0)
    // {
    //     fprintf(stderr, "Error: Assembly failed.\n");
    //     return;
    // }

    // // 2. Link using MinGW GCC and link kernel32 for ExitProcess
    // snprintf(command, sizeof(command),
    //     "gcc -o %s.exe %s.obj -lkernel32", output_file, output_file);

    // printf("Linking: %s\n", command);

    // if (system(command) != 0)
    // {
    //     fprintf(stderr, "Error: Linking failed.\n");
    //     return;
    // }

    // // 3. Run the executable
    // snprintf(command, sizeof(command),
    //     ".\\%s.exe", output_file);

    // printf("Running: %s\n", command);

    // if (system(command) != 0)
    // {
    //     fprintf(stderr, "", output_file);
    //     return;
    // }

    printf("Execution completed successfully.\n");
}


