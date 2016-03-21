%{
  #include <stdio.h>
  #include <string.h>

  #include "ast.h"

  node_t *ast;

  int yyerror (char *s);
  int yylex();

  extern char* yytext;
  extern int yylineno, col, yyleng;

  void myprintf(__const char *__restrict __format, ...) {
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

Program: Block | Program Block { myprintf("Program\n"); };

Block: FunctionDefinition | FunctionDeclaration | Declaration { myprintf("Block\n"); };

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody { myprintf("FunctionDefinition\n"); };

FunctionBody: LBRACE FunctionBodyDeclaration FunctionBodyStatement RBRACE { myprintf("FunctionBody\n"); }
            | LBRACE FunctionBodyStatement RBRACE                         { myprintf("FunctionBody\n"); }
            | LBRACE FunctionBodyDeclaration RBRACE                       { myprintf("FunctionBody\n"); }
            | LBRACE RBRACE                                               { myprintf("FunctionBody\n"); }
            | LBRACE error RBRACE                                         { myprintf("ErrorFunctionBody\n"); };

FunctionBodyDeclaration: FunctionBodyDeclaration Declaration  { myprintf("FunctionBodyDeclaration\n"); }
                       | Declaration                          { myprintf("FunctionBodyDeclaration\n"); };

FunctionBodyStatement: FunctionBodyStatement Statement  { myprintf("FunctionBodyStatement\n"); }
                     | Statement                        { myprintf("FunctionBodyStatement\n"); };

Declaration: TypeSpec Declarator CommaDeclarator SEMI { myprintf("Declaration\n"); } // int a CommaDeclarator;
           | error SEMI                               { myprintf("Error Declaration\n"); };


CommaDeclarator: CommaDeclarator COMMA Declarator // int a, b, c, d ...*/
               | /* empty */ {};

Declarator: Id | Id LSQ INTLIT RSQ { myprintf("Declarator\n"); };

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { myprintf("FunctionDeclaration\n"); };

TypeSpec: CHAR { myprintf("TypeSpec CHAR\n"); }
        | INT  { myprintf("TypeSpec INT\n"); }
        | VOID { myprintf("TypeSpec VOID\n"); };

FunctionDeclarator: Id LPAR ParameterList RPAR              { myprintf("FunctionDeclarator\n"); };

ParameterList: ParameterDeclaration                     { myprintf("ParameterList\n"); }
             | ParameterList COMMA ParameterDeclaration { myprintf("ParameterList\n"); };

ParameterDeclaration: TypeSpec Id                       { myprintf("ParameterDeclaration\n"); };

//Asterisk: AST | Asterisk AST | /* empty */ { printf("Asterisk\n"); };
Id: AST Id | ID { myprintf("Id\n"); };

Statement: CommaExpression SEMI                                                                       { myprintf("CommaExpression Statement\n"); }
         | LBRACE StatementList RBRACE                                                                { myprintf("Block Statement\n"); }
         | LBRACE RBRACE                                                                              { myprintf("Block Statement\n"); }
         | LBRACE error RBRACE                                                                        { myprintf("Error Block Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement %prec THEN                                          { myprintf("If Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement ELSE Statement                                      { myprintf("If Else Statement\n"); }
         | FOR LPAR ForCommaExpression SEMI ForCommaExpression SEMI ForCommaExpression RPAR Statement { myprintf("For Statement\n"); }
         | RETURN CommaExpression SEMI                                                                { myprintf("Return Statement\n");}
         | error SEMI                                                                                 { myprintf("Error Statement\n"); };

StatementList: StatementList Statement { myprintf("StatementList\n"); }
             | Statement            { myprintf("StatementList\n"); };

ForCommaExpression: CommaExpression
                  | /* empty */ {  };

CommaExpression: CommaExpression COMMA Expression { myprintf("CommaExpression\n"); }
               | Expression                       { myprintf("CommaExpression\n"); };

Expression: Expression ASSIGN Expression    { myprintf("Expression\n"); }
          | Expression AND Expression       { myprintf("Expression\n"); }
          | Expression OR Expression        { myprintf("Expression\n"); }
          | Expression EQ Expression        { myprintf("Expression\n"); }
          | Expression NE Expression        { myprintf("Expression\n"); }
          | Expression LT Expression        { myprintf("Expression\n"); }
          | Expression GT Expression        { myprintf("Expression\n"); }
          | Expression LE Expression        { myprintf("Expression\n"); }
          | Expression GE Expression        { myprintf("Expression\n"); }
          | Expression AMP Expression       { myprintf("Expression\n"); }
          | Expression AST Expression       { myprintf("Expression\n"); }
          | Expression PLUS Expression      { myprintf("Expression\n"); }
          | Expression MINUS Expression     { myprintf("Expression\n"); }
          | Expression DIV Expression       { myprintf("Expression\n"); }
          | Expression MOD Expression       { myprintf("Expression\n"); }
          | Expression NOT Expression       { myprintf("Expression\n"); }
          | Expression LSQ Expression RSQ   { myprintf("Expression\n"); }
          | ID LPAR ExpressionList RPAR     { myprintf("Expression\n"); }
          | ID                              { myprintf("Expression\n"); }
          | INTLIT                          { myprintf("Expression\n"); }
          | CHRLIT                          { myprintf("Expression\n"); }
          | STRLIT                          { myprintf("Expression\n"); }
          | LPAR Expression RPAR            { myprintf("Expression\n"); }
          | LPAR error RPAR                 { myprintf("Expression\n"); }
          | ID LPAR error RPAR              { myprintf("Expression\n"); };

ExpressionList: CommaExpression | /* empty */ {};
%%

int yyerror (char *s) {
  printf("Line %d, col %d: %s: %s\n", yylineno, col - (int) yyleng, s, yytext);
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
    //print_ast(ast);
  } else {
    // display only syntactic or lexical errors
  }
}
