#include "tac_support.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int global_label_counter = 1;  

char *create_tac_ins_allocate(const char *variableName)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "allocate variable %s", variableName);
    return instruction;
}

char *generate_tac_open_func_label(const char *funcName)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "function_%s_start:", funcName); 
    return instruction;
}

char *generate_tac_close_func_label(const char *funcName)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "function_%s_end:", funcName);   
    return instruction;
}

char *generate_tac_assign_statement(const char *type, const char *src, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "assign %s %s to %s", type, src, dest);
    return instruction;
}

char *generate_tac_load_int_literal(int literal, const char *temp_var)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "load integer literal %d to %s", literal, temp_var);
    return instruction;
}

char *generate_tac_load_float_literal(float literal, const char *temp_var)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "load float literal %f to %s", literal, temp_var);
    return instruction;
}

char *generate_tac_write_int(const char *temp_var)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "write integer %s", temp_var);
    return instruction;
}

char *generate_tac_write_float(const char *temp_var)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "write float %s", temp_var);
    return instruction;
}

char *generate_tac_read_int(const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "read integer %s", dest);
    return instruction;
}

char *generate_tac_read_float(const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "read float %s", dest);
    return instruction;
}

char *generate_tac_add_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "add %s, %s, %s", op1, op2, dest);   // ← comma, no arrow
    return instruction;
}

char *generate_tac_sub_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "sub %s, %s, %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_mul_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "mul %s, %s, %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_div_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "div %s, %s, %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_and_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "logic-and %s, %s -> %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_or_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "logic-or %s, %s -> %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_eq_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "equal %s, %s -> %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_neq_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "not-equal %s, %s -> %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_lt_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "less-than %s, %s -> %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_gt_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "greater-than %s, %s -> %s", op1, op2, dest);  // Fixed typo: "grater" → "greater"
    return instruction;
}

char *generate_tac_leq_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "less-than-or-equal %s, %s -> %s", op1, op2, dest);
    return instruction;
}

char *generate_tac_geq_instruction(const char *op1, const char *op2, const char *dest)
{
    char *instruction = (char *)malloc(200 * sizeof(char));
    snprintf(instruction, 200, "greater-than-or-equal %s, %s -> %s", op1, op2, dest);  // Fixed typo
    return instruction;
}



char *generate_tac_loop_start_label(int *label_count)
{
    char *label = (char *)malloc(100 * sizeof(char));
    snprintf(label, 100, "L_loop_start_%d:", global_label_counter++);
    return label;
}

char *generate_tac_loop_end_label(int *label_count)
{
    char *label = (char *)malloc(100 * sizeof(char));
    snprintf(label, 100, "L_loop_end_%d:", global_label_counter++);
    return label;
}

char *generate_tac_if_condition_label(int *label_count)
{
    char *label = (char *)malloc(100 * sizeof(char));
    snprintf(label, 100, "L_if_cond_%d:", global_label_counter++);
    return label;
}

char *generate_tac_if_then_label(int *label_count)
{
    char *label = (char *)malloc(100 * sizeof(char));
    snprintf(label, 100, "L_if_then_%d:", global_label_counter++);
    return label;
}

char *generate_tac_if_else_label(int *label_count)
{
    char *label = (char *)malloc(100 * sizeof(char));
    snprintf(label, 100, "L_if_else_%d:", global_label_counter++);
    return label;
}

char *generate_tac_if_end_label(int *label_count)
{
    char *label = (char *)malloc(100 * sizeof(char));
    snprintf(label, 100, "L_if_end_%d:", global_label_counter++);
    return label;
}