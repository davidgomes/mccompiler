lex mccompiler.l
/usr/local/bin/bison -v -d mccompiler.y
gcc -g -c ast.c -o ast
gcc -g -c symbol_t.c -o symbol_t
gcc -g -c semantics.c -o semantics
gcc -g -o mccompiler ast symbol_t semantics mccompiler.tab.c lex.yy.c -ll -ly
rm mccompiler.zip
zip mccompiler.zip mccompiler.l mccompiler.y ast.c ast.h symbol_t.c symbol_t.h semantics.c semantics.h
