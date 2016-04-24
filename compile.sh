lex mccompiler.l
yacc -d --locations mccompiler.y
gcc -g -c ast.c -o ast
gcc -g -c symbol_t.c -o symbol_t
gcc -g -c semantics.c -o semantics
gcc -o mccompiler y.tab.c lex.yy.c ast symbol_t semantics -ly -g
rm mccompiler.zip
zip mccompiler.zip mccompiler.l mccompiler.y ast.c ast.h symbol_t.c symbol_t.h semantics.c semantics.h
rm -rf mccompiler.dSYM