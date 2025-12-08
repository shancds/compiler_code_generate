bison -d parser.y
flex scanner.l
gcc parser.tab.c lex.yy.c ast.c location.c symbol_table.c token.c error.c tac.c tac_support.c -o compiler



cmd /c "type test\func_main.txt | compiler.exe"