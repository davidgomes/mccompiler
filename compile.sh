lex mccompiler.l
yacc -v -d mccompiler.y
gcc -c ast.c -o ast
gcc -o mccompiler ast y.tab.c lex.yy.c -ll -ly -Wreturn-type
rm mccompiler.zip
zip -r mccompiler.zip ./* > .zipsuccess
