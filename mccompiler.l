%option yylineno

%{
  #include "y.tab.h"
  #define YY_USER_ACTION col+=yyleng; yylloc.first_line = yylineno; yylloc.first_column = col-(int)yyleng; yylval.str=(char *) strdup(yytext);

  #include <stdarg.h>

  #include "ast.h"
  #include "symbol_t.h"
  #include "semantics.h"
  #include "code_gen.h"

  int col = 1, comment_start_col, comment_start_line;
  int flag_yylex = 0;
  int flag_errors = 0;

  void myprintf(__const char *__restrict __format, ...) {
    va_list args;
    va_start(args, __format);
    printf(__format, args);
    va_end(args);
  }
%}

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
<COMMENT><<eof>> {printf("Line %d, col %d: unterminated comment\n",comment_start_line, comment_start_col); col=1; flag_yylex = 0; flag_errors = 0; return 0;}
<COMMENT>"*/"  {if(flag_errors){BEGIN ERRORS;} else {BEGIN 0;}}
<COMMENT>.  {;}
<COMMENT>\n {col = 1;}

<ERRORS>"/*"  {BEGIN COMMENT; comment_start_col = col-2; comment_start_line = yylineno;}

<ERRORS>{CHRLIT} {yylval.str=(char *) strdup(yytext); return CHRLIT;}
<ERRORS>{UN_CHRLIT} {printf("Line %d, col %d: unterminated char constant\n",yylineno, col-(int)yyleng); col = 1;}
<ERRORS>{INV_CHRLIT}  {printf("Line %d, col %d: invalid char constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

<ERRORS>{STRLIT}  {yylval.str=(char *) strdup(yytext); return STRLIT;}
<ERRORS>{UN_STRLIT} {printf("Line %d, col %d: unterminated string constant\n",yylineno, col-(int)yyleng); col = 1;}
<ERRORS>{INV_STRLIT}  {printf("Line %d, col %d: invalid string constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

<ERRORS>{CHAR}  {return CHAR;}
<ERRORS>{ELSE} {return ELSE;}
<ERRORS>{FOR} {return FOR;}
<ERRORS>{IF} {return IF;}
<ERRORS>{INT} {return INT;}
<ERRORS>{RETURN} {return RETURN;}
<ERRORS>{VOID} {return VOID;}

<ERRORS>{RESERVED}  {return RESERVED;}
<ERRORS>{INTLIT}  {yylval.str=(char *) strdup(yytext); return INTLIT;}
<ERRORS>{ID}  {yylval.str=(char *) strdup(yytext); return ID;}

<ERRORS>{AMP} {return AMP;}
<ERRORS>{AND} {return AND;}
<ERRORS>{ASSIGN} {return ASSIGN;}
<ERRORS>{AST} {return AST;}
<ERRORS>{COMMA} {return COMMA;}
<ERRORS>{DIV} {return DIV;}
<ERRORS>{EQ} {return EQ;}
<ERRORS>{GE} {return GE;}
<ERRORS>{GT} {return GT;}
<ERRORS>{LBRACE} {return LBRACE;}
<ERRORS>{LE}  {return LE;}
<ERRORS>{LPAR} {return LPAR;}
<ERRORS>{LSQ} {return LSQ;}
<ERRORS>{LT}  {return LT;}
<ERRORS>{MINUS} {return MINUS;}
<ERRORS>{MOD} {return MOD;}
<ERRORS>{NE} {return NE;}
<ERRORS>{NOT} {return NOT;}
<ERRORS>{OR} {return OR;}
<ERRORS>{PLUS} {return PLUS;}
<ERRORS>{RBRACE} {return RBRACE;}
<ERRORS>{RPAR} {return RPAR;}
<ERRORS>{RSQ} {return RSQ;}
<ERRORS>{SEMI} {return SEMI;}

<ERRORS>{whitespace}  {;}
<ERRORS>\n {col = 1;}
<ERRORS><<eof>> {col+=1; flag_errors = 0; return 0;}
<ERRORS>. {printf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}

{CHRLIT} {printf("CHRLIT(%s)\n", yytext); yylval.str=(char *) strdup(yytext); return CHRLIT;}
{UN_CHRLIT} {printf("Line %d, col %d: unterminated char constant\n",yylineno, col-(int)yyleng); col = 1;}
{INV_CHRLIT}  {printf("Line %d, col %d: invalid char constant (%s)\n",yylineno, col-(int)yyleng, yytext);}

{STRLIT}  {printf("STRLIT(%s)\n", yytext); yylval.str=(char *) strdup(yytext); return STRLIT;}
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
{INTLIT}  {printf("INTLIT(%s)\n", yytext); yylval.str=(char *) strdup(yytext); return INTLIT;}
{ID}  {yylval.str=(char *) strdup(yytext); printf("ID(%s)\n", yytext); return ID;}

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
<<eof>> {col+=1; flag_yylex = 0; return 0;}
. {printf("Line %d, col %d: illegal character (%s)\n",yylineno, col-(int)yyleng, yytext);}

%%

int main(int argc, char **argv) {
  if (argc > 1) {
    if (!strcmp(argv[1], "-l")) {
      flag_yylex = 1;
      while(flag_yylex)
        yylex();
    } else if (!strcmp(argv[1], "-1")) {
      flag_errors = 1;
      BEGIN ERRORS;

      while (flag_errors) {
        yylex();
      }
    } else if (!strcmp(argv[1], "-2")) {
      flag_errors = 1;
      BEGIN ERRORS;
      yyparse();
    } else if (!strcmp(argv[1], "-t")) {
      flag_errors = 1;
      BEGIN ERRORS;
      yyparse();

      if (!were_there_errors) {
        ast_print_tree(ast, 0);
      }

      //ast_destroy(ast);
    } else if (!strcmp(argv[1], "-s")) {
      flag_errors = 1;
      BEGIN ERRORS;
      yyparse();

      if (!were_there_errors) {
        init_st();

        an_tree(ast, st, NULL, 1, 0);
        st_print_table(st);
        printf("\n");
        print_an_tree(ast, 0, 1);
      }
    }
  } else {
    flag_errors = 1;
    BEGIN ERRORS;
    yyparse();

    if (!were_there_errors) {
      init_st();
      an_tree(ast, st, NULL, 1, 0);

      // todo maybe only call code_gen if there were no syntactic errors
      if (!semantic_errors) {
        code_gen(ast, NULL);
      }
    }
  }

  return 0;
}

int yywrap() {
  return 1;
}
