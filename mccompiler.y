%{
  #include <stdio.h>
  #include <string.h>

  int yyerror (char *s);
  int yylex();

  extern char* yytext;
  extern int yylineno, col, yyleng;
%}

%token CHAR ELSE FOR IF INT RETURN VOID RESERVED INTLIT ID
%token AMP AND ASSIGN AST COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE
%token NOT OR PLUS RBRACE RPAR RSQ SEMI CHRLIT STRLIT

%%
Program: Block | Program Block {};
Block: FunctionDefinition | FunctionDeclaration | Declaration {};

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody {};
FunctionBody: LBRACE Declaration Statement RBRACE {};
Declaration: TypeSpec Declarator CommaDeclarator SEMI {};
CommaDeclarator: Declarator | COMMA Declarator {};
Declarator: Asterisk ID | Asterisk ID LSQ INTLIT RSQ {};

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI {};
TypeSpec: CHAR | INT | VOID {};

FunctionDeclarator: Asterisk ID LPAR ParameterList RPAR {};
Asterisk: AST | Asterisk AST | /* empty */ {};
%%

int yyerror (char *s) {
     printf ("Line %d, col %d: %s: %s\n", yylineno, col - (int) yyleng, s, yytext);
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
