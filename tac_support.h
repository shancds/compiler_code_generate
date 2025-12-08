#ifndef TAC_SUPPORT_
#define TAC_SUPPORT_

char *create_tac_ins_allocate(const char *variableName);
char *generate_tac_open_func_label(const char *funcName);
char *generate_tac_close_func_label(const char *funcName);
char *generate_tac_assign_statement(const char *type, const char *src, const char *dest);
char *generate_tac_load_int_literal(int literal, const char *temp_var);
char *generate_tac_load_float_literal(float literal, const char *temp_var);
char* generate_tac_write_int(const char *temp_var);
char* generate_tac_write_float(const char *temp_var);
char *generate_tac_read_int(const char *dest);
char *generate_tac_read_float(const char *dest);
char *generate_tac_add_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_sub_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_mul_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_div_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_and_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_or_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_eq_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_neq_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_lt_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_gt_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_leq_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_geq_instruction(const char *op1, const char *op2, const char *dest);
char *generate_tac_loop_start_label(int *label_count);
char *generate_tac_loop_end_label(int *label_count);
char *generate_tac_if_condition_label(int *label_count);
char *generate_tac_if_then_label(int *label_count);
char *generate_tac_if_else_label(int *label_count);
char *generate_tac_if_end_label(int *label_count);
#endif