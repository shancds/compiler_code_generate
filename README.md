
1. bison -d parser.y
2. flex scanner.l
3. gcc parser.tab.c lex.yy.c ast.c location.c symbol_table.c token.c error.c tac.c tac_support.c -o compiler
4. cmd /c "type test\func_main.txt | compiler.exe"


5. nasm -f win64 -o output.obj ./output/05_assembly.txt
6. gcc -o output.exe output.obj -lkernel32
7. ./output.exe