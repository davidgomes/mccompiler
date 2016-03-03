%option yylineno

%X  COMMENT

whitespace  [\t| ]
letra [a-zA-Z]|\_
ID  {letra}(_*[0-9a-zA-Z]+_*)+
numero [0-9]+
INTLIT  {numero}
RESERVED  "++"|"--"|auto|break|case|const|continue|default|do|long|register|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|while|double|enum|extern|float|goto
escape  \\(\\|\"|\'|n|t|[0-7]{3})
CHRLIT '({escape}|[^'\n])*'
STRLIT  \"({escape}|[^\"\n])*\"
UN_STRLIT \"({escape}|[^\"\n])*

%{
  #define YY_USER_ACTION col+=yyleng; //YY_USER_ACTION -  action which is always executed prior to the matched rule's action
  int col = 1, com_col, line;
%}

%%
"/*"  {BEGIN COMMENT; com_col = col-yyleng;}
<COMMENT>"*/"  {BEGIN 0;}
<COMMENT>\n {col = 1;}
<COMMENT>.  {;}
<COMMENT><<eof>>  {printf("Line %d, col %d: unterminated comment\n", yylineno, com_col);}

{CHRLIT} {printf("CHRLIT(%s)\n", yytext);}
{STRLIT}  {printf("STRLIT(%s)\n", yytext);}
{UN_STRLIT} {printf("Line %d, col %d: unterminated string constant\n",yylineno, col-(int)yyleng);}


char {printf("CHAR\n");}
else {printf("ELSE\n");}
for {printf("FOR\n");}
if {printf("IF\n");}
int {printf("INT\n");}
return {printf("RETURN\n");}
void {printf("VOID\n");}

{RESERVED}  {printf("RESERVED(%s)\n", yytext);}
{INTLIT}  {printf("INTLIT(%s)\n", yytext);}
{ID}  {printf("ID(%s)\n", yytext);}

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

{whitespace}  {;}
\n {col = 1;}
<<eof>> {return 0;}
. {printf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}

%%
int main(int argc, char **argv) {
  if (argc > 1) {
    if (strcmp(argv[1], "-l") == 0) {
      yylex();
    }
  }

  return 0;
}

int yywrap()
{
  return 1;
}
