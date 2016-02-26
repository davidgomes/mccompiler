ID  _*([a-z]|[A-Z])+_*([a-z]|[A-Z])*
%%
{ID}  {printf("ID(%s)", yytext);}
\"(.)+\"  {printf(" string java");}
. {;}
%%
int main()
{
  yylex();
  return 0;
}

int yywrap()
{
  return 1;
}
