%{
  #define YY_USER_ACTION col+=yyleng; //YY_USER_ACTION -  action which is always executed prior to the matched rule's action

  #include "y.tab.h"
  #include <stdarg.h>

  int col = 1, comment_start_col, comment_start_line;
  int flag = 0;

  void myprintf(__const char *__restrict __format, ...) {
    va_list args;
    va_start(args, __format);
    printf(__format, args);
    va_end(args);
  }
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

{CHRLIT} {myprintf("CHRLIT(%s)\n", yytext);}
{UN_CHRLIT} {printf("Line %d, col %d: unterminated char constant\n",yylineno, col-(int)yyleng); col = 1;}
{INV_CHRLIT}  {printf("Line %d, col %d: invalid char constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

{STRLIT}  {myprintf("STRLIT(%s)\n", yytext);}
{UN_STRLIT} {printf("Line %d, col %d: unterminated string constant\n",yylineno, col-(int)yyleng); col = 1;}
{INV_STRLIT}  {printf("Line %d, col %d: invalid string constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

{CHAR}  {myprintf("CHAR\n"); return CHAR;}
{ELSE} {myprintf("ELSE\n"); return ELSE;}
{FOR} {myprintf("FOR\n"); return FOR;}
{IF} {myprintf("IF\n"); return IF;}
{INT} {myprintf("INT\n"); return INT;}
{RETURN} {myprintf("RETURN\n"); return RETURN;}
{VOID} {myprintf("VOID\n"); return VOID;}

{RESERVED}  {myprintf("RESERVED(%s)\n", yytext); return RESERVED;}
{INTLIT}  {myprintf("INTLIT(%s)\n", yytext); return INTLIT;}
{ID}  {myprintf("ID(%s)\n", yytext); return ID;}

{AMP} {myprintf("AMP\n"); return AMP;}
{AND} {myprintf("AND\n"); return AND;}
{ASSIGN} {myprintf("ASSIGN\n"); return ASSIGN;}
{AST} {myprintf("AST\n"); return AST;}
{COMMA} {myprintf("COMMA\n"); return COMMA;}
{DIV} {myprintf("DIV\n"); return DIV;}
{EQ} {myprintf("EQ\n"); return EQ;}
{GE} {myprintf("GE\n"); return GE;}
{GT} {myprintf("GT\n"); return GT;}
{LBRACE} {myprintf("LBRACE\n"); return LBRACE;}
{LE} {myprintf("LE\n"); return LE;}
{LPAR} {myprintf("LPAR\n"); return LPAR;}
{LSQ} {myprintf("LSQ\n"); return LSQ;}
{LT} {myprintf("LT\n"); return LT;}
{MINUS} {myprintf("MINUS\n"); return MINUS;}
{MOD} {myprintf("MOD\n"); return MOD;}
{NE} {myprintf("NE\n"); return NE;}
{NOT} {myprintf("NOT\n"); return NOT;}
{OR} {myprintf("OR\n"); return OR;}
{PLUS} {myprintf("PLUS\n"); return PLUS;}
{RBRACE} {myprintf("RBRACE\n"); return RBRACE;}
{RPAR} {myprintf("RPAR\n"); return RPAR;}
{RSQ} {myprintf("RSQ\n"); return RSQ;}
{SEMI} {myprintf("SEMI\n"); return SEMI;}
{whitespace}  {;}
\n {col = 1;}
<<eof>> {col=1; return 0;}
. {myprintf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}

%%
int yywrap()
{
  return 1;
}
