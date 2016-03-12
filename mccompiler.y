%{
  #include <stdio.h>
  #include <string.h>

  int yyerror (char *s);
  int yylex();

  extern char* yytext;
  extern int yylineno, col, yyleng;
%}

%token whitespace letra ID numero INTLIT RESERVED escape
%token CHRLIT UN_CHRLIT INV_CHRLIT STRLIT INV_STRLIT

%{

%}

%%
Prog: '.'  {}

%%

int yyerror (char *s) {
     printf ("Line %d, col %d: %s: %s\n", yylineno, col - (int)yyleng, s, yytext);
}

int main(int argc, char **argv) {
  int flag_t = 0;
  int flag_l = 0;
  int flag_1 = 0;

  while (argc--) {
    if (!strcmp(*argv, "-l")) {
      flag_l = 1;
    } else if (!strcmp(*argv, "-1")) {
      flag_1 = 1;
    } else if (!strcmp(*argv, "-t")) {
      flag_t = 1;
    }

    argv++;
  }

  if (flag_l || flag_1) {
    yylex();
  } else if (flag_t) {
    yyparse();
  } else {
    // display only syntactic or lexical errors
  }
}
