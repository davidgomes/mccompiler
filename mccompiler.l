%X  COMMENT
ID  _*([a-z]|[A-Z])+_*([a-z]|[A-Z])*_*
numero [0-9]+
INTLIT  {numero}
RESERVED  "++"|"--"
CHRLIT \\(\\|\"|\'|n|t|[0-7]{3})|[a-z]|[A-Z]
STRLIT  \".+\"

%{
  int col, line;
%}

%%
"/*"  {BEGIN COMMENT;}
<COMMENT>"*/"  {BEGIN 0;}
<COMMENT>.  {;}
<COMMENT><<eof>>  {printf("Line <num linha>, col <num coluna>: unterminated comment\n"); return 0;}
{CHRLIT} {printf("CHRLIT(%s)\n", yytext);}
{STRLIT}  {printf("STRLIT(%s)\n", yytext);}
char {printf("CHAR\n");}
else {printf("ELSE\n");}
for {printf("FOR\n");}
if {printf("IF\n");}
int {printf("INT\n");}
return {printf("RETURN\n");}
void {printf("VOID\n");}
{ID}  {printf("ID(%s)\n", yytext);}
{INTLIT}  {printf("INTLIT(%s)\n", yytext);}
& {printf("AMP\n");}
&& {printf("AND\n");}
"=" {printf("ASSIGN\n");}
"*" {printf("AST\n");}
"," {printf("COMMA\n");}
"/" {printf("DIV\n");}
"==" {printf("EQ\n");}
">=" {printf("GE\n");}
">" {printf("GT\n");}
"{" {printf("LBRACE\n");}
"<=" {printf("LE\n");}
"(" {printf("LPAR\n");}
"[" {printf("LSQ\n");}
"<" {printf("LT\n");}
"-" {printf("MINUS\n");}
"%" {printf("MOD\n");}
"!=" {printf("NE\n");}
"!" {printf("NOT\n");}
"||" {printf("OR\n");}
"+" {printf("PLUS\n");}
"}" {printf("RBRACE\n");}
")" {printf("RPAR\n");}
"]" {printf("RSQ\n");}
";" {printf("SEMI\n");}
{RESERVED}  {printf("RESERVED\n");}
.|\n {;}
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
