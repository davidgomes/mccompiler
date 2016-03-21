lex mccompiler.l
/usr/local/bin/bison -v -d mccompiler.y
gcc -c ast.c -o ast
gcc -o mccompiler ast y.tab.c lex.yy.c -ll -ly -Wreturn-type
rm mccompiler.zip
zip -r mccompiler.zip ./* > .zipsuccess
