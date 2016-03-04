%option yylineno

%X  COMMENT
%X  ERRORS

whitespace  [\t ]
letra [a-zA-Z]|\_
ID  {letra}(_*[0-9a-zA-Z]*_*)*
numero [0-9]+
INTLIT  {numero}
RESERVED  "++"|"--"|auto|break|case|const|continue|default|do|long|register|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|while|double|enum|extern|float|goto
escape  \\(\\|\"|\'|n|t|[0-7]{1,3})
CHRLIT '({escape}|[^'\n\\]){1}'
UN_CHRLIT '[^'\n]*|{escape}\n
INV_CHRLIT  '[^'\n]*'
STRLIT  \"({escape}|[^\"\n\\])+\"
UN_STRLIT \"[^\"\n]*{escape}*\n
INV_STRLIT  \"[^\"\n]*\"

%{
  #define YY_USER_ACTION col+=yyleng; //YY_USER_ACTION -  action which is always executed prior to the matched rule's action
  int col = 1, com_col;
  int flag = 0;
%}

%%
"/*"  {BEGIN COMMENT; com_col = col-yyleng;}
<COMMENT>"*/"  {if(flag == 1){BEGIN ERRORS;} else {BEGIN 0;}}
<COMMENT>.  {;}
<COMMENT>\n {col = 1;}
<COMMENT><<eof>>  {printf("Line %d, col %d: unterminated comment\n", yylineno, com_col); return 0;}

<ERRORS>"/*"  {BEGIN COMMENT; com_col = col-yyleng;}

<ERRORS>{CHRLIT} {;}
<ERRORS>{UN_CHRLIT} {printf("Line %d, col %d: unterminated char constant\n",yylineno, col-(int)yyleng); col = 1;}
<ERRORS>{INV_CHRLIT}  {printf("Line %d, col %d: invalid char constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

<ERRORS>{STRLIT}  {;}
<ERRORS>{UN_STRLIT} {printf("Line %d, col %d: unterminated string constant\n",yylineno, col-(int)yyleng); col = 1;}
<ERRORS>{INV_STRLIT}  {printf("Line %d, col %d: invalid string constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

<ERRORS>char {;}
<ERRORS>else {;}
<ERRORS>for {;}
<ERRORS>if {;}
<ERRORS>int {;}
<ERRORS>return {;}
<ERRORS>void {;}

<ERRORS>{RESERVED}  {;}
<ERRORS>{INTLIT}  {;}
<ERRORS>{ID}  {;}

<ERRORS>& {;}
<ERRORS>&& {;}
<ERRORS>"=" {;}
<ERRORS>"*" {;}
<ERRORS>"," {;}
<ERRORS>"/" {;}
<ERRORS>"==" {;}
<ERRORS>">=" {;}
<ERRORS>">" {;}
<ERRORS>"{" {;}
<ERRORS>"<=" {;}
<ERRORS>"(" {;}
<ERRORS>"[" {;}
<ERRORS>"<" {;}
<ERRORS>"-" {;}
<ERRORS>"%" {;}
<ERRORS>"!=" {;}
<ERRORS>"!" {;}
<ERRORS>"||" {;}
<ERRORS>"+" {;}
<ERRORS>"}" {;}
<ERRORS>")" {;}
<ERRORS>"]" {;}
<ERRORS>";" {;}

<ERRORS>{whitespace}  {;}
<ERRORS>\n {col = 1;}
<ERRORS>. {printf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}


{CHRLIT} {printf("CHRLIT(%s)\n", yytext);}
{UN_CHRLIT} {printf("Line %d, col %d: unterminated char constant\n",yylineno, col-(int)yyleng); col = 1;}
{INV_CHRLIT}  {printf("Line %d, col %d: invalid char constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

{STRLIT}  {printf("STRLIT(%s)\n", yytext);}
{UN_STRLIT} {printf("Line %d, col %d: unterminated string constant\n",yylineno, col-(int)yyleng); col = 1;}
{INV_STRLIT}  {printf("Line %d, col %d: invalid string constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

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
  flag = 1;
  BEGIN ERRORS;
  if (argc > 1) {
    if (strcmp(argv[1], "-l") == 0) {
      flag = 0;
      BEGIN 0;
    }
  }
  yylex();
  return 0;
}

int yywrap()
{
  return 1;
}
