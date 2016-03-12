%{
  #define YY_USER_ACTION col+=yyleng; //YY_USER_ACTION -  action which is always executed prior to the matched rule's action

  #define PRINT_TOKEN(x) { printf("%s\n", x); return x; }

  #include "y.tab.h"

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

{CHAR}  {printf("CHAR\n"); return CHAR;}
{ELSE} {printf("ELSE\n"); return ELSE;}
{FOR} {printf("FOR\n"); return FOR;}
{IF} {printf("IF\n"); return IF;}
{INT} {printf("INT\n"); return INT;}
{RETURN} {printf("RETURN\n"); return RETURN;}
{VOID} {printf("VOID\n"); return VOID;}

{RESERVED}  {printf("RESERVED(%s)\n", yytext); return RESERVED;}
{INTLIT}  {printf("INTLIT(%s)\n", yytext); return INTLIT;}
{ID}  {printf("ID(%s)\n", yytext); return ID;}

{AMP} {printf("AMP\n"); return AMP;}
{AND} {printf("AND\n"); return AND;}
{ASSIGN} {printf("ASSIGN\n"); return ASSIGN;}
{AST} {printf("AST\n"); return AST;}
{COMMA} {printf("COMMA\n"); return COMMA;}
{DIV} {printf("DIV\n"); return DIV;}
{EQ} {printf("EQ\n"); return EQ;}
{GE} {printf("GE\n"); return GE;}
{GT} {printf("GT\n"); return GT;}
{LBRACE} {printf("LBRACE\n"); return LBRACE;}
{LE} {printf("LE\n"); return LE;}
{LPAR} {printf("LPAR\n"); return LPAR;}
{LSQ} {printf("LSQ\n"); return LSQ;}
{LT} {printf("LT\n"); return LT;}
{MINUS} {printf("MINUS\n"); return MINUS;}
{MOD} {printf("MOD\n"); return MOD;}
{NE} {printf("NE\n"); return NE;}
{NOT} {printf("NOT\n"); return NOT;}
{OR} {printf("OR\n"); return OR;}
{PLUS} {printf("PLUS\n"); return PLUS;}
{RBRACE} {printf("RBRACE\n"); return RBRACE;}
{RPAR} {printf("RPAR\n"); return RPAR;}
{RSQ} {printf("RSQ\n"); return RSQ;}
{SEMI} {printf("SEMI\n"); return SEMI;}

{whitespace}  {;}
\n {col = 1;}
<<eof>> {col=1; return 0;}
. {printf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}

%%
int yywrap()
{
  return 1;
}
