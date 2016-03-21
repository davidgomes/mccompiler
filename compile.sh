lex mccompiler.l
/usr/local/bin/bison -v -d mccompiler.y
gcc -c ast.c -o ast
gcc -o mccompiler ast mccompiler.tab.c lex.yy.c -ll -ly
rm mccompiler.zip
zip -r mccompiler.zip ./* > .zipsuccess
