%{
  #include <stdio.h>
  #include <string.h>

  #include "ast.h"

  node_t *ast;

  int yyerror (char *s);
  int yylex();

  extern char* yytext;
  extern int yylineno, col, yyleng;
%}

%token CHAR ELSE FOR IF INT RETURN VOID RESERVED INTLIT ID
%token AMP AND ASSIGN AST COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE
%token NOT OR PLUS RBRACE RPAR RSQ SEMI CHRLIT STRLIT

%nonassoc THEN
%nonassoc ELSE

%right ASSIGN

%left COMMA
%left LSQ
%left OR AND
%left EQ NE
%left GE GT LE LT
%left PLUS MINUS
%left AST DIV MOD
%left AMP NOT
%%

Program: Block | Program Block { printf("Program\n"); };

Block: FunctionDefinition | FunctionDeclaration | Declaration { printf("Block\n"); };

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody { printf("FunctionDefinition\n"); };

FunctionBody: LBRACE FunctionBodyDeclaration FunctionBodyStatement RBRACE { printf("FunctionBody\n"); };

FunctionBodyDeclaration: FunctionBodyDeclaration Declaration  { printf("FunctionBodyDeclaration\n"); }
                       | /* empty */                          { printf("FunctionBodyDeclaration\n"); };

FunctionBodyStatement: FunctionBodyStatement Statement  { printf("FunctionBodyStatement\n"); }
                     | /* empty */                      { printf("FunctionBodyStatement\n"); };

Declaration: TypeSpec Declarator CommaDeclarator SEMI { printf("Declaration\n"); }; // int a CommaDeclarator;

CommaDeclarator: CommaDeclarator COMMA Declarator // int a, b, c, d ...*/
               | /* empty */ {};

Declarator: Id | Id LSQ INTLIT RSQ { printf("Declarator\n"); };

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { printf("FunctionDeclaration\n"); };

TypeSpec: CHAR { printf("TypeSpec CHAR\n"); }
        | INT  { printf("TypeSpec INT\n"); }
        | VOID { printf("TypeSpec VOID\n"); };

FunctionDeclarator: Id LPAR ParameterList RPAR              { printf("FunctionDeclarator\n"); };

ParameterList: ParameterDeclaration                     { printf("ParameterList\n"); }
             | ParameterList COMMA ParameterDeclaration { printf("ParameterList\n"); };

ParameterDeclaration: TypeSpec Id                       { printf("ParameterDeclaration\n"); };

//Asterisk: AST | Asterisk AST | /* empty */ { printf("Asterisk\n"); };
Id: AST Id | ID { printf("Id\n"); };

Statement: CommaExpression SEMI                                                                       { printf("CommaExpression Statement\n"); }
         | LBRACE StatementList RBRACE                                                                    { printf("Block Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement %prec THEN                                          { printf("If Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement ELSE Statement                                      { printf("If Else Statement\n"); }
         | FOR LPAR ForCommaExpression SEMI ForCommaExpression SEMI ForCommaExpression RPAR Statement { printf("For Statement\n"); }
         | RETURN CommaExpression SEMI                                                                { printf("Return Statement\n");} ;

StatementList: StatementList Statement { printf("StatementList\n"); }
             | /* empty */             { printf("StatementList\n"); };

ForCommaExpression: CommaExpression
                  | /* empty */ {  };

CommaExpression: CommaExpression COMMA Expression { printf("CommaExpression\n"); }
               | Expression                       { printf("CommaExpression\n"); };

Expression: Expression ASSIGN Expression    { printf("Expression\n"); }
          | Expression AND Expression       { printf("Expression\n"); }
          | Expression OR Expression        { printf("Expression\n"); }
          | Expression EQ Expression        { printf("Expression\n"); }
          | Expression NE Expression        { printf("Expression\n"); }
          | Expression LT Expression        { printf("Expression\n"); }
          | Expression GT Expression        { printf("Expression\n"); }
          | Expression LE Expression        { printf("Expression\n"); }
          | Expression GE Expression        { printf("Expression\n"); }
          | Expression AMP Expression       { printf("Expression\n"); }
          | Expression AST Expression       { printf("Expression\n"); }
          | Expression PLUS Expression      { printf("Expression\n"); }
          | Expression MINUS Expression     { printf("Expression\n"); }
          | Expression DIV Expression       { printf("Expression\n"); }
          | Expression MOD Expression       { printf("Expression\n"); }
          | Expression NOT Expression       { printf("Expression\n"); }
          | Expression LSQ Expression RSQ   { printf("Expression\n"); }
          | ID LPAR ExpressionList RPAR     { printf("Expression\n"); }
          | ID                              { printf("Expression\n"); }
          | INTLIT                          { printf("Expression\n"); }
          | CHRLIT                          { printf("Expression\n"); }
          | STRLIT                          { printf("Expression\n"); }
          | LPAR Expression RPAR            { printf("Expression\n"); }; // not necessary??

ExpressionList: CommaExpression | ExpressionList CommaExpression | /* empty */ {};
%%

int yyerror (char *s) {
  printf ("Line %d, col %d: %s: %s\n", yylineno, col - (int) yyleng, s, yytext);
  return 0;
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
    print_ast(ast);
  } else {
    // display only syntactic or lexical errors
  }
}
