lex mccompiler.l
/usr/local/bin/bison -v -d mccompiler.y
gcc -g -c ast.c -o ast
gcc -g -c symbol_t.c -o symbol_t
gcc -g -c errors.c -o errors
gcc -g -o mccompiler ast symbol_t errors semantics mccompiler.tab.c lex.yy.c -ll -ly
rm mccompiler.zip
zip mccompiler.zip mccompiler.l mccompiler.y ast.c ast.h symbol_t.c symbol_t.h errors.c errors.h
