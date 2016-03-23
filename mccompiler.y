%{
  #include <stdio.h>
  #include <string.h>
  #include <stdarg.h>

  #include "ast.h"

  #define YYDEBUG 1

  node_t *ast;

  int yyerror();
  int yylex();

  extern char* yytext;
  extern int yylineno, col, yyleng;

  void myprintf2(__const char *__restrict __format, ...) {
    va_list args;
    va_start(args, __format);
    //printf(__format, args);
    va_end(args);
  }
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

Program: Block | Program Block { myprintf2("Program\n"); };

Block: FunctionDefinition | FunctionDeclaration | Declaration { myprintf2("Block\n"); };

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody { myprintf2("FunctionDefinition\n"); };

FunctionBody: LBRACE FunctionBodyDeclaration FunctionBodyStatement RBRACE { myprintf2("FunctionBody\n"); }
            | LBRACE FunctionBodyStatement RBRACE                         { myprintf2("FunctionBody\n"); }
            | LBRACE FunctionBodyDeclaration RBRACE                       { myprintf2("FunctionBody\n"); }
            | LBRACE RBRACE                                               { myprintf2("FunctionBody\n"); }
            | LBRACE error RBRACE                                         { myprintf2("ErrorFunctionBody\n"); };

FunctionBodyDeclaration: FunctionBodyDeclaration Declaration  { myprintf2("FunctionBodyDeclaration\n"); }
                       | Declaration              { myprintf2("FunctionBodyDeclaration\n"); };

FunctionBodyStatement: FunctionBodyStatement Statement  { myprintf2("FunctionBodyStatement\n"); }
                     | StatementNotErrorSemi            { myprintf2("FunctionBodyStatement\n"); };

Declaration: TypeSpec Declarator CommaDeclarator SEMI { myprintf2("Declaration\n"); } // int a CommaDeclarator;
           | error SEMI                               { myprintf2("Error Declaration\n"); };

CommaDeclarator: CommaDeclarator COMMA Declarator // int a, b, c, d ...*/
               | /* empty */ {};

Declarator: Id | Id LSQ INTLIT RSQ { myprintf2("Declarator\n"); };

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { myprintf2("FunctionDeclaration\n"); };

TypeSpec: CHAR { myprintf2("TypeSpec CHAR\n"); }
        | INT  { myprintf2("TypeSpec INT\n"); }
        | VOID { myprintf2("TypeSpec VOID\n"); };

FunctionDeclarator: Id LPAR ParameterList RPAR              { myprintf2("FunctionDeclarator\n"); };

ParameterList: ParameterDeclaration                     { myprintf2("ParameterList\n"); }
             | ParameterList COMMA ParameterDeclaration { myprintf2("ParameterList\n"); };

ParameterDeclaration: TypeSpec Asterisk ID                  { myprintf2("ParameterDeclaration\n"); }
                    | TypeSpec ID
                    | TypeSpec Asterisk
                    | TypeSpec
                    ;

Id: AST Id | ID { myprintf2("Id\n"); };

Asterisk: Asterisk AST
        | AST { myprintf2("Asterisk\n"); };

StatementNotErrorSemi: CommaExpression SEMI                                                           { myprintf2("CommaExpression Statement\n"); }
         | LBRACE StatementList RBRACE                                                                { myprintf2("Block Statement\n"); }
         | LBRACE RBRACE                                                                              { myprintf2("Block Statement\n"); }
         | LBRACE error RBRACE                                                                        { myprintf2("Error Block Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement %prec THEN                                          { myprintf2("If Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement ELSE Statement                                      { myprintf2("If Else Statement\n"); }
         | FOR LPAR ForCommaExpression SEMI ForCommaExpression SEMI ForCommaExpression RPAR Statement { myprintf2("For Statement\n"); }
         | RETURN CommaExpression SEMI                                                                { myprintf2("Return Statement\n");}

Statement: CommaExpression SEMI                                                                       { myprintf2("CommaExpression Statement\n"); }
         | LBRACE StatementList RBRACE                                                                { myprintf2("Block Statement\n"); }
         | LBRACE RBRACE                                                                              { myprintf2("Block Statement\n"); }
         | LBRACE error RBRACE                                                                        { myprintf2("Error Block Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement %prec THEN                                          { myprintf2("If Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement ELSE Statement                                      { myprintf2("If Else Statement\n"); }
         | FOR LPAR ForCommaExpression SEMI ForCommaExpression SEMI ForCommaExpression RPAR Statement { myprintf2("For Statement\n"); }
         | RETURN CommaExpression SEMI                                                                { myprintf2("Return Statement\n");}
         | error SEMI                                                                                 { myprintf2("Error Statement\n"); };

StatementList: StatementList Statement { myprintf2("StatementList\n"); }
             | Statement            { myprintf2("StatementList\n"); };

ForCommaExpression: CommaExpression
                  | /* empty */ {  };

CommaExpression: CommaExpression COMMA Expression { myprintf2("CommaExpression\n"); }
               | Expression                       { myprintf2("CommaExpression\n"); };

Expression: Expression ASSIGN Expression    { myprintf2("Expression\n"); }
          | Expression AND Expression       { myprintf2("Expression\n"); }
          | Expression OR Expression        { myprintf2("Expression\n"); }
          | Expression EQ Expression        { myprintf2("Expression\n"); }
          | Expression NE Expression        { myprintf2("Expression\n"); }
          | Expression LT Expression        { myprintf2("Expression\n"); }
          | Expression GT Expression        { myprintf2("Expression\n"); }
          | Expression LE Expression        { myprintf2("Expression\n"); }
          | Expression GE Expression        { myprintf2("Expression\n"); }
          | Expression AST Expression       { myprintf2("Expression\n"); }
          | Expression PLUS Expression      { myprintf2("Expression\n"); }
          | Expression MINUS Expression     { myprintf2("Expression\n"); }
          | Expression DIV Expression       { myprintf2("Expression\n"); }
          | Expression MOD Expression       { myprintf2("Expression\n"); }
          | AMP Expression                  { myprintf2("Expression\n"); }
          | AST Expression                  { myprintf2("Expression\n"); }
          | PLUS Expression                 { myprintf2("Expression\n"); }
          | MINUS Expression                { myprintf2("Expression\n"); }
          | NOT Expression                  { myprintf2("Expression\n"); }
          | Expression LSQ Expression RSQ   { myprintf2("Expression\n"); }
          | ID LPAR ExpressionList RPAR     { myprintf2("Expression\n"); }
          | ID                              { myprintf2("Expression\n"); }
          | INTLIT                          { myprintf2("Expression\n"); }
          | CHRLIT                          { myprintf2("Expression\n"); }
          | STRLIT                          { myprintf2("Expression\n"); }
          | LPAR Expression RPAR            { myprintf2("Expression\n"); }
          | LPAR error RPAR                 { myprintf2("Expression\n"); }
          | ID LPAR error RPAR              { myprintf2("Expression\n"); };

ExpressionList: CommaExpression | /* empty */ {};
%%

int yyerror (char *s) {
  printf("Line %d, col %d: %s: %s\n", yylineno, col - (int) yyleng, s, yytext);
  return 0;
}
