lex mccompiler.l
yacc -v -d mccompiler.y
gcc -o mccompiler y.tab.c lex.yy.c -ll -ly -Wreturn-type
rm mccompiler.zip
zip -r mccompiler.zip ./* > .zipsuccess
