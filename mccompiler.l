%{
  #define YY_USER_ACTION col+=yyleng; //YY_USER_ACTION -  action which is always executed prior to the matched rule's action
  int col = 1, comment_start_col, comment_start_line;
  int flag = 0;
%}

%option yylineno

%X  COMMENT
%X  ERRORS

whitespace  [\t ]

letra [a-zA-Z]|\_
escape  \\(\\|\"|\'|n|t|[0-7]{1,3})

ID  {letra}(_*[0-9a-zA-Z]*_*)*
INTLIT  [0-9]+
RESERVED  "++"|"--"|auto|break|case|const|continue|default|do|long|register|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|while|double|enum|extern|float|goto

CHRLIT '({escape}|[^'\n\\])'
UN_CHRLIT '([^'\n\\]?|\\[^'\n]|{escape})*\\?
INV_CHRLIT  '([^'\n\\]*|\\[^'\n]|\\')*'

STRLIT  \"({escape}|[^\"\n\\])*\"
UN_STRLIT \"([^\"\n\\]*|\\[^'\n]|{escape}*)*\\?
INV_STRLIT  \"([^\"\n\\]*|\\[^'\n]|\\')*\"

CHAR  char
ELSE  else
FOR for
IF  if
INT int
RETURN  return
VOID void
AMP &
AND &&
ASSIGN  "="
AST "*"
COMMA ","
DIV "/"
EQ  "=="
GE  ">="
GT  ">"
LBRACE "{"
LE  "<="
LPAR  "("
LSQ "["
LT  "<"
MINUS "-"
MOD "%"
NE  "!="
NOT "!"
OR  "||"
PLUS  "+"
RBRACE  "}"
RPAR  ")"
RSQ "]"
SEMI  ";"

%%
"/*"  {BEGIN COMMENT; comment_start_col = col-2; comment_start_line = yylineno;}
<COMMENT><<eof>> {printf("Line %d, col %d: unterminated comment\n",comment_start_line, comment_start_col); col=1; return 0;}
<COMMENT>"*/"  {if(flag == 1){BEGIN ERRORS;} else {BEGIN 0;}}
<COMMENT>.  {;}
<COMMENT>\n {col = 1;}

<ERRORS>"/*"  {BEGIN COMMENT; comment_start_col = col-2; comment_start_line = yylineno;}

<ERRORS>{CHRLIT} {;}
<ERRORS>{UN_CHRLIT} {printf("Line %d, col %d: unterminated char constant\n",yylineno, col-(int)yyleng); col = 1;}
<ERRORS>{INV_CHRLIT}  {printf("Line %d, col %d: invalid char constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

<ERRORS>{STRLIT}  {;}
<ERRORS>{UN_STRLIT} {printf("Line %d, col %d: unterminated string constant\n",yylineno, col-(int)yyleng); col = 1;}
<ERRORS>{INV_STRLIT}  {printf("Line %d, col %d: invalid string constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

<ERRORS>{CHAR}  {;}
<ERRORS>{ELSE} {;}
<ERRORS>{FOR} {;}
<ERRORS>{IF} {;}
<ERRORS>{INT} {;}
<ERRORS>{RETURN} {;}
<ERRORS>{VOID} {;}

<ERRORS>{RESERVED}  {;}
<ERRORS>{INTLIT}  {;}
<ERRORS>{ID}  {;}

<ERRORS>{AMP} {;}
<ERRORS>{AND} {;}
<ERRORS>{ASSIGN} {;}
<ERRORS>{AST} {;}
<ERRORS>{COMMA} {;}
<ERRORS>{DIV} {;}
<ERRORS>{EQ} {;}
<ERRORS>{GE} {;}
<ERRORS>{GT} {;}
<ERRORS>{LBRACE} {;}
<ERRORS>{LE}  {;}
<ERRORS>{LPAR} {;}
<ERRORS>{LSQ} {;}
<ERRORS>{LT}  {;}
<ERRORS>{MINUS} {;}
<ERRORS>{MOD} {;}
<ERRORS>{NE} {;}
<ERRORS>{NOT} {;}
<ERRORS>{OR} {;}
<ERRORS>{PLUS} {;}
<ERRORS>{RBRACE} {;}
<ERRORS>{RPAR} {;}
<ERRORS>{RSQ} {;}
<ERRORS>{SEMI} {;}

<ERRORS>{whitespace}  {;}
<ERRORS>\n {col = 1;}
<ERRORS><<eof>> {col=1; return 0;}
<ERRORS>. {printf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}

{CHRLIT} {printf("CHRLIT(%s)\n", yytext);}
{UN_CHRLIT} {printf("Line %d, col %d: unterminated char constant\n",yylineno, col-(int)yyleng); col = 1;}
{INV_CHRLIT}  {printf("Line %d, col %d: invalid char constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

{STRLIT}  {printf("STRLIT(%s)\n", yytext);}
{UN_STRLIT} {printf("Line %d, col %d: unterminated string constant\n",yylineno, col-(int)yyleng); col = 1;}
{INV_STRLIT}  {printf("Line %d, col %d: invalid string constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

{CHAR}  {printf("CHAR\n");}
{ELSE} {printf("ELSE\n");}
{FOR} {printf("FOR\n");}
{IF} {printf("IF\n");}
{INT} {printf("INT\n");}
{RETURN} {printf("RETURN\n");}
{VOID} {printf("VOID\n");}

{RESERVED}  {printf("RESERVED(%s)\n", yytext);}
{INTLIT}  {printf("INTLIT(%s)\n", yytext);}
{ID}  {printf("ID(%s)\n", yytext);}

{AMP} {printf("AMP\n");}
{AND} {printf("AND\n");}
{ASSIGN} {printf("ASSIGN\n");}
{AST} {printf("AST\n");}
{COMMA} {printf("COMMA\n");}
{DIV} {printf("DIV\n");}
{EQ} {printf("EQ\n");}
{GE} {printf("GE\n");}
{GT} {printf("GT\n");}
{LBRACE} {printf("LBRACE\n");}
{LE} {printf("LE\n");}
{LPAR} {printf("LPAR\n");}
{LSQ} {printf("LSQ\n");}
{LT} {printf("LT\n");}
{MINUS} {printf("MINUS\n");}
{MOD} {printf("MOD\n");}
{NE} {printf("NE\n");}
{NOT} {printf("NOT\n");}
{OR} {printf("OR\n");}
{PLUS} {printf("PLUS\n");}
{RBRACE} {printf("RBRACE\n");}
{RPAR} {printf("RPAR\n");}
{RSQ} {printf("RSQ\n");}
{SEMI} {printf("SEMI\n");}

{whitespace}  {;}
\n {col = 1;}
<<eof>> {col=1; return 0;}
. {printf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}

%%
int yywrap()
{
  return 1;
}
