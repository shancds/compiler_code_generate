%{


#include <stdio.h>
#include "location.h"
#include "ast.h"
#include "token.h"
#include "error.h"
#include "symbol_table.h"
#include "tac.h"



int locationX = 1;      
int locationY = 1;      
int tokenLength = 0;   
FILE *tokenOutFile = NULL;
FILE *syntaxAnalasisTokenOutFile = NULL;
FILE *errorMessageFile = NULL;

int yylex();
void yyerror(const char *msg);
void createAbstractSyntaxTree();
void createSymbolTable();
void createIntermediateCode();

 ASTNode *ast = NULL;
%}

%union 
{
    int ival;
    double fval;
    char *sval;
    ASTNode *node; 
};


%token CLASS
%token IMPLEMENT
%token FUNC
%token ISA
%token PUBLIC
%token PRIVATE
%token LET
%token INTEGER
%token FLOAT
%token VOID
%token IF
%token THEN
%token ELSE
%token WHILE
%token READ
%token WRITE
%token RETURN
%token NOT
%token SELF
%token CONSTRCT
%token LOCAL
%token ATTRIBUTE

%token ASSIGNOP
%token EQ
%token NEQ
%token LT
%token GT
%token LEQ
%token GEQ
%token ADDOP 
%token MINUOP 
%token OROP 
%token MULTOP 
%token DIVOP 
%token ANDOP 
  


%token LEFT_CURLY_BREACKET
%token RIGHT_CURLY_BREACKET
%token LEFT_CIR_BREACKET
%token RIGHT_CIR_BREACKET
%token LEFT_RECT_BREACKET
%token RIGHT_RECT_BREACKET
%token COMMA
%token DOT
%token SEMICOL
%token COLON
%token ARROW
 
%token <ival> INTLIT
%token <fval> FLOATLIT
%token <sval> LETTER ID

%type <node> prog classOrImplOrFuncList classOrImplOrFunc classDecl classOptInheritsIdBlockDecl classOptInheritsIdList
%type <node> classDeclBodyList classDeclBody implDef implBodyDefList funcDef visibility memberDecl funcDecl funcHead
%type <node> funcBody funcBodyList varDeclOrStmt attributeDecl localVarDecl varDecl varArrSizeList statement
%type <node> assignStat statBlock statementList expr relExpr arithExpr arithAddExprEnd sign term termTail factor
%type <node> variable idnestList indiceList idOrSelf idnest indice functionCall arraySize type returnType
%type <node> fParams arraySizeList fParamsTailList aParams aParamsTailList fParamsTail aParamsTail assignOp relOp addOp multOp

%start prog
%%

prog: classOrImplOrFuncList                                         { ast = $1; };

classOrImplOrFuncList: classOrImplOrFunc classOrImplOrFuncList      { $$ = append_node($1, $2); }
                     | /* epsilon */                                { $$ = NULL; };

classOrImplOrFunc: classDecl                                        { $$ = $1; }
                 | implDef                                          { $$ = $1; }
                 | funcDef                                          { $$ = $1; };

classDecl: CLASS ID classOptInheritsIdBlockDecl 
           LEFT_CURLY_BREACKET 
               classDeclBodyList 
           RIGHT_CURLY_BREACKET SEMICOL                             { $$ = create_class_decl($2, $5, $3, locationX, locationY); };

classOptInheritsIdBlockDecl: ISA ID classOptInheritsIdList          { $$ = append_node(create_variable($2, locationX, locationY), $3); }
                           | /* epsilon */                          { $$ = NULL; };

classOptInheritsIdList: COMMA ID classOptInheritsIdList             { $$ = append_node(create_variable($2, locationX, locationY), $3); }
                      | /* epsilon */                               { $$ = NULL; };

classDeclBodyList: classDeclBody classDeclBodyList                  { $$ = append_node($1, $2); }
                 | /* epsilon */                                    { $$ = NULL; };

classDeclBody: visibility memberDecl                                { $$ = append_node($1, $2); };

implDef: IMPLEMENT ID 
         LEFT_CURLY_BREACKET 
            implBodyDefList 
         RIGHT_CURLY_BREACKET                                      { $$ = create_impl_def($2, $4, locationX, locationY); };

implBodyDefList: funcDef implBodyDefList                            { $$ = append_node($1, $2); }
               | /* epsilon */                                      { $$ = NULL; };

funcDef: funcHead funcBody                                          { $$ = create_func_decl($1->data.func_decl.name, $1->data.func_decl.params, $1->data.func_decl.return_type, $2, locationX, locationY); };

visibility: PUBLIC                                                  { $$ = create_visibility("public", locationX, locationY); }
          | PRIVATE                                                 { $$ = create_visibility("private", locationX, locationY); };

memberDecl: funcDecl                                                { $$ = $1; }
          | attributeDecl                                           { $$ = $1; };

funcDecl: funcHead SEMICOL                                          { $$ = $1; }
        | funcHead funcBody SEMICOL                                 { $$ = create_func_decl($1->data.func_decl.name, $1->data.func_decl.params, $1->data.func_decl.return_type, $2, locationX, locationY); };

funcHead: FUNC ID LEFT_CIR_BREACKET fParams RIGHT_CIR_BREACKET 
          ARROW returnType                                          { $$ = create_func_decl($2, $4, $7, NULL, locationX, locationY); };

funcBody: LEFT_CURLY_BREACKET funcBodyList RIGHT_CURLY_BREACKET     { $$ = $2; };

funcBodyList: varDeclOrStmt funcBodyList                            { $$ = append_node($1, $2); }
            | /* epsilon */                                         { $$ = NULL; };


varDeclOrStmt: localVarDecl                                         { $$ = $1; }
             | statement                                            { $$ = $1; };

attributeDecl: ATTRIBUTE ID COLON type varArrSizeList SEMICOL        { $$ = create_var_decl($2, $4, $5, locationX, locationY); };

localVarDecl: LOCAL varDecl                                         { $$ = $2; };

varDecl: ID COLON type varArrSizeList SEMICOL                    { $$ = create_var_decl($1, $3, $4, locationX, locationY); };

varArrSizeList: arraySize varArrSizeList                            { $$ = append_node($1, $2); }
              | /* epsilon */                                       { $$ = NULL; };

statement: assignStat SEMICOL                                       { $$ = $1; }
         | IF LEFT_CIR_BREACKET relExpr RIGHT_CIR_BREACKET THEN 
              statBlock 
           ELSE 
              statBlock 
           SEMICOL                                                { $$ = create_if_stat($3, $6, $8, locationX, locationY); }
         | WHILE LEFT_CIR_BREACKET relExpr RIGHT_CIR_BREACKET 
              statBlock 
           SEMICOL                                                { $$ = create_while_stat($3, $5, locationX, locationY); }
         | READ 
           LEFT_CIR_BREACKET variable RIGHT_CIR_BREACKET SEMICOL   { $$ = create_io_stat("READ", $3, locationX, locationY); }
         | WRITE LEFT_CIR_BREACKET expr RIGHT_CIR_BREACKET SEMICOL { $$ = create_io_stat("WRITE", $3, locationX, locationY); }
         | RETURN LEFT_CIR_BREACKET expr RIGHT_CIR_BREACKET SEMICOL { $$ = create_return_stat($3, locationX, locationY); }
         | functionCall SEMICOL                                     { $$ = $1; };

assignStat: variable assignOp expr                                  { $$ = create_assign_stat($1, $3, locationX, locationY); };




statBlock: LEFT_CURLY_BREACKET statementList RIGHT_CURLY_BREACKET   { $$ = $2; }
         | statement                                                { $$ = $1; }
         | /* epsilon */                                            { $$ = NULL; };

statementList: statement statementList                              { $$ = append_node($1, $2); }
             | /* epsilon */                                        { $$ = NULL; };

expr: arithExpr                                                     { $$ = $1; }
    | relExpr                                                       { $$ = $1; };

relExpr: arithExpr relOp arithExpr                                  { $$ = create_binary_op($2->type, $1, $3, locationX, locationY); };

//arithExpr: term arithAddExprEnd                                     { $$ = $2 ? create_binary_op($2->type, $1, $2->data.op.right, locationX, locationY) : $1; };

arithExpr: term arithAddExprEnd {   
    if($2){
        $2->data.op.left = $1;
        $$ = $2;
    }
    else
    {
        $$ = $1;
    }
};

//arithAddExprEnd: addOp term arithAddExprEnd                         { ASTNode *opNode = create_binary_op($1->type, $2, NULL, locationX, locationY); if ($2) { opNode->data.op.right = $2; } $$ = opNode; }
 //              | /* epsilon */                                      { $$ = NULL; };

arithAddExprEnd: addOp term arithAddExprEnd {
    if($3){
        // For left-associativity: first create the left operation
        ASTNode *leftOp = create_binary_op($1->type, NULL, $2, locationX, locationY);
        // Then make it the left child of the next operation
        $3->data.op.left = leftOp;
        $$ = $3;
    }else{
        // Base case: just create the binary operation with right operand
        $$ = create_binary_op($1->type, NULL, $2, locationX, locationY);
    }
}
| /* epsilon */ {
    $$ = NULL;  
};

sign: ADDOP                                                         { $$ = create_op_node(NODE_POSITIVE, locationX, locationY); }
    | MINUOP                                                        { $$ = create_op_node(NODE_NEGATIVE, locationX, locationY); };

//term: factor termTail                                               { $$ = $2 ? create_binary_op($2->type, $1, $2->data.op.right, locationX, locationY) : $1; };
term: factor termTail { 
    if($2){
        $2->data.op.left = $1;
        $$ = $2;
    }
    else
    {
        $$ = $1;
    }
};


//termTail: multOp factor termTail                                    { ASTNode *opNode = create_binary_op($1->type, $2, NULL, locationX, locationY); if ($2) { opNode->data.op.right = $2; } $$ = opNode; }
//        | /* epsilon */                                             { $$ = NULL; };

termTail: multOp factor termTail { 
    if($3){
        // For left-associativity: first create the left operation
        ASTNode *leftOp = create_binary_op($1->type, NULL, $2, locationX, locationY);
        // Then make it the left child of the next operation
        $3->data.op.left = leftOp;
        $$ = $3;
    }else{
        // Base case: just create the binary operation with right operand
        $$ = create_binary_op($1->type, NULL, $2, locationX, locationY);
    }
} 
| /* epsilon */ { $$ = NULL; };
        

factor: variable                                                    { $$ = $1; }
      | functionCall                                                { $$ = $1; }
      | INTLIT                                                      { $$ = create_literal_int($1, locationX, locationY); }
      | FLOATLIT                                                    { $$ = create_literal_float($1, locationX, locationY); }
      | LEFT_CIR_BREACKET arithExpr RIGHT_CIR_BREACKET              { $$ = $2; }
      | NOT factor                                                  { $$ = create_unary_op(NODE_NOT, $2, locationX, locationY); }
      | sign factor                                                 { $$ = create_unary_op($1->type, $2, locationX, locationY); };

//variable: idnestList idOrSelf indiceList                            { $$ = create_variable($2, locationX, locationY); $$->data.variable.indices = $3; if ($1) { $$->data.variable.nested = $1; } };

//variable: idnestList idOrSelf indiceList                            {$$ = $2; $$->data.variable.indices = $3; if ($1) { $$->data.variable.nested = $1; }};

variable: idnestList idOrSelf indiceList { 
    $$ = create_variable($2->data.variable.name, locationX, locationY); 
    $$->data.variable.indices = $3; 
    if ($1) { $$->data.variable.nested = $1; } 
};

          


idnestList: idnestList idnest                                       { $$ = append_node($1, $2); }
          | /* epsilon */                                           { $$ = NULL; };

indiceList: indiceList indice                                       { $$ = append_node($1, $2); }
          | /* epsilon */                                           { $$ = NULL; };

idOrSelf: ID                                                        { $$ = create_variable($1, locationX, locationY); }
        | SELF                                                      { $$ = create_variable("self", locationX, locationY); };

idnest: ID indiceList DOT                                           { ASTNode *node = create_variable($1, locationX, locationY); node->data.variable.indices = $2; $$ = node; }
      | ID LEFT_CIR_BREACKET aParams RIGHT_CIR_BREACKET DOT         { ASTNode *node = create_func_call($1, $3, locationX, locationY); $$ = node; };

indice: LEFT_RECT_BREACKET arithExpr RIGHT_RECT_BREACKET            { $$ = $2; };

functionCall: idnestList ID 
              LEFT_CIR_BREACKET aParams RIGHT_CIR_BREACKET          { $$ = create_func_call($2, $4, locationX, locationY); if ($1) { $$->data.func_call.nested = $1; } };

arraySize: LEFT_RECT_BREACKET INTLIT RIGHT_RECT_BREACKET            { $$ = create_literal_int($2, locationX, locationY); }
         | LEFT_RECT_BREACKET RIGHT_RECT_BREACKET                   { $$ = NULL; };

type: INTEGER                                                       { $$ = create_type_node("integer", locationX, locationY); }
    | FLOAT                                                         { $$ = create_type_node("float", locationX, locationY); }
    | ID                                                            { $$ = create_type_node($1, locationX, locationY); };

returnType: type                                                    { $$ = $1; }
          | VOID                                                    { $$ = create_type_node("void", locationX, locationY); };

fParams: ID COLON type arraySizeList fParamsTailList                { ASTNode *param = create_param_decl($1, $3, $4, locationX, locationY); $$ = append_node(param, $5); }
       | /* epsilon */                                              { $$ = NULL; };

arraySizeList: arraySize arraySizeList                              { $$ = append_node($1, $2); }
             | /* epsilon */                                        { $$ = NULL; };

fParamsTailList: fParamsTail fParamsTailList                        { $$ = append_node($1, $2); }
               | /* epsilon */                                      { $$ = NULL; };

aParams: expr aParamsTailList                                       { $$ = append_node($1, $2); }
       | /* epsilon */                                              { $$ = NULL; };

aParamsTailList: aParamsTail aParamsTailList                        { $$ = append_node($1, $2); }
               | /* epsilon */                                      { $$ = NULL; };

fParamsTail: COMMA ID COLON type arraySizeList                      { $$ = create_param_decl($2, $4, $5, locationX, locationY); };

aParamsTail: COMMA expr                                             { $$ = $2; };

assignOp: ASSIGNOP                                                  { $$ = create_op_node(NODE_ASSIGN_STAT, locationX, locationY); };

relOp: EQ                                                           { $$ = create_op_node(NODE_EQ, locationX, locationY); }
     | NEQ                                                          { $$ = create_op_node(NODE_NEQ, locationX, locationY); }
     | LT                                                           { $$ = create_op_node(NODE_LT, locationX, locationY); }
     | GT                                                           { $$ = create_op_node(NODE_GT, locationX, locationY); }
     | LEQ                                                          { $$ = create_op_node(NODE_LEQ, locationX, locationY); }
     | GEQ                                                          { $$ = create_op_node(NODE_GEQ, locationX, locationY); };

addOp: ADDOP                                                        { $$ = create_op_node(NODE_ADD, locationX, locationY); }
     | MINUOP                                                       { $$ = create_op_node(NODE_SUBTRACT, locationX, locationY); }
     | OROP                                                         { $$ = create_op_node(NODE_OR, locationX, locationY); };

multOp: MULTOP                                                      { $$ = create_op_node(NODE_MULTIPLY, locationX, locationY); }
      | DIVOP                                                       { $$ = create_op_node(NODE_DIVIDE, locationX, locationY); }
      | ANDOP                                                       { $$ = create_op_node(NODE_AND, locationX, locationY); };

%%

/**
* Function to handle parsing errors
*/
void yyerror(const char *msg) {
    fprintf(stderr, "There is an error at line %d, col %d (%s)\n", getLocationY(), getLocationX(), msg); 
}

int main() {
    
    initializeErrorAnalyzer();
    initializeTokenController();

    if (yyparse() != 0){
        printf("Parsing failed!\n");
       
        return 0;
    } 
     

    createAbstractSyntaxTree();
    createSymbolTable();

    finalizeTokenController();

    if (hasErrors())
    {
        printf("%s\n", "There were errors on your source code.");
        return 0;
    }
    createIntermediateCode();
    return 0;
}

void createAbstractSyntaxTree()
{
     
    FILE *file = fopen("./output/abstract_syntax_tree_02.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }
    print_ast_to_file(ast, file);
    fclose(file);
}

void createSymbolTable()
{
   
    // Initialize the global symbol table
    SymbolTable *global_table = create_symbol_table(NULL);
    
    // Build the symbol table starting from the AST root
    build_symbol_table_phase1(ast, global_table, NULL);
    build_symbol_table_phase2(ast, global_table, NULL);

    // Print the symbol table
    FILE *symOutFile = fopen("./output/symbol_table_03.txt", "w");
    if (symOutFile == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }
    print_symbol_table(global_table, 0, symOutFile);
    fclose(symOutFile);
}

void createIntermediateCode()
{
 
     
   TAC *tac_list = NULL; // Single pointer, initialized as NULL
    generate_tac_recursive(&tac_list, ast, 0);


   FILE *tacFile = fopen("./output/three_address_code_04.txt", "w");
    if (tacFile == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }
 

    print_tac(&tac_list, tacFile);
    fclose(tacFile); 


    FILE *asmFile = fopen("./output/assembly_04.txt", "w");
    if (asmFile == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }
    generate_assembly(&tac_list, asmFile);
    fclose(asmFile); 


    const char *asm_file = "./output/assembly_04.txt"; // Name of the assembly file
    const char *output_file = "output";  // Desired name for the binary

    assemble_and_run(asm_file, output_file);
}