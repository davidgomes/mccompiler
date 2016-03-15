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

%left LE GT LT GE COMMA AND OR EQ NE PLUS MINUS AST MOD DIV AMP
%right ASSIGN NOT
%nonassoc ELSE

%%
Program: Block | Program Block { printf("Program\n"); };
Block: FunctionDefinition | FunctionDeclaration | Declaration { printf("Block\n"); };

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody { printf("FunctionDefinition\n"); };
FunctionBody: LBRACE FunctionBodyDeclaration FunctionBodyStatement RBRACE { printf("FunctionBody\n"); };

FunctionBodyDeclaration: Declaration
                       | FunctionBodyDeclaration Declarator
                       | /* empty */ { printf("FunctionBodyDeclaration\n"); };

FunctionBodyStatement: Statement
                     | FunctionBodyStatement Statement
                     | /* empty */ { printf("FunctionBodyStatement\n"); };

Declaration: TypeSpec Declarator CommaDeclarator SEMI { printf("Declaration\n"); }; // int a CommaDeclarator;

CommaDeclarator: COMMA Declarator // int a, b
               | CommaDeclarator COMMA Declarator // int a, b, c, d ...
               | /* empty */ {};

Declarator: Id | Id LSQ INTLIT RSQ { printf("Declarator\n"); };

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { printf("FunctionDeclaration\n"); };
TypeSpec: CHAR
        | INT
        | VOID { printf("TypeSpec\n"); };

FunctionDeclarator: Id LPAR ParameterList RPAR { printf("FunctionDeclarator\n"); };
ParameterList: ParameterDeclaration
             | ParameterList COMMA ParameterDeclaration { printf("ParameterList\n"); };
ParameterDeclaration: TypeSpec Id { printf("ParameterDeclaration\n"); };

//Asterisk: AST | Asterisk AST | /* empty */ { printf("Asterisk\n"); };
Id: AST Id | ID { printf("Id\n"); };

Statement: Expression SEMI
         | LBRACE Statement RBRACE
         | IF LPAR Expression RPAR Statement
         | IF LPAR Expression RPAR Statement ELSE Statement
         | FOR LPAR Expression SEMI Expression SEMI Expression RPAR Statement
         | RETURN Expression SEMI { printf("Statement\n");} ;

Expression: Expression ASSIGN Expression
          | Expression COMMA Expression
          | Expression AND Expression
          | Expression OR Expression
          | Expression EQ Expression
          | Expression NE Expression
          | Expression LT Expression
          | Expression GT Expression
          | Expression LE Expression
          | Expression GE Expression
          | AMP Expression
          | AST Expression
          | PLUS Expression
          | MINUS Expression
          | NOT Expression
          | Expression LSQ Expression RSQ
          | Id
          | INTLIT
          | CHRLIT
          | STRLIT
          | LPAR Expression RPAR { printf("Expression\n"); };

// Expr → ID LPAR [Expr {COMMA Expr}]

/*Expr → Expr (ASSIGN | COMMA) Expr
Expr → Expr (AND | OR) Expr
Expr → Expr (EQ | NE | LT | GT | LE | GE) Expr
Expr → Expr (PLUS | MINUS | AST | DIV | MOD) Expr
Expr → (AMP | AST | PLUS | MINUS | NOT) Expr
Expr → Expr LSQ Expr RSQ
Expr → ID LPAR [Expr {COMMA Expr}] RPAR
Expr → ID | INTLIT | CHRLIT | STRLIT | LPAR Expr RPAR*/
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
