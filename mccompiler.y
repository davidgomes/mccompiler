%{
  #include <stdio.h>
  #include <string.h>
  #include <stdarg.h>

  #include "ast.h"

  #define YYDEBUG 1

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

%union{
  char* str;
	struct node *node;
}

%token <str> CHAR ELSE FOR IF INT RETURN VOID RESERVED INTLIT ID
%token <str> AMP AND ASSIGN AST COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE
%token <str> NOT OR PLUS RBRACE RPAR RSQ SEMI CHRLIT STRLIT

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

%type <node> FunctionDefinition FunctionBody FunctionBodyDeclaration FunctionBodyStatement
Declaration CommaDeclarator Declarator FunctionDeclaration TypeSpec FunctionDeclarator ParameterList
ParameterDeclaration Asterisk StatementCanError Statement StatementList ForCommaExpression
CommaExpression Expression ExpressionList TerminalIntlit ArrayDeclarator Start StartAgain Id Ast

%%

Start: FunctionDefinition StartAgain  { $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2); }
     | FunctionDeclaration StartAgain { $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2); }
     | Declaration StartAgain         { $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2); }
     ;

StartAgain: FunctionDefinition StartAgain { $$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2); }
          | FunctionDeclaration StartAgain { $$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2); }
          | Declaration StartAgain         { $$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2); }
          | /* empty */ {$$ = NULL;}
          ;

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody { $$ = ast_insert_node(NODE_FUNCDEFINITION, 1, 3, $1, $2, $3); }
                  ;

FunctionBody: LBRACE FunctionBodyDeclaration FunctionBodyStatement RBRACE { $$ = ast_insert_node(NODE_FUNCBODY, 1, 2, $1, $2); }
            | LBRACE FunctionBodyStatement RBRACE                         { $$ = ast_insert_node(NODE_FUNCBODY, 1, 1, $1); }
            | LBRACE FunctionBodyDeclaration RBRACE                       { $$ = ast_insert_node(NODE_FUNCBODY, 1, 2, $1, $2); }
            | LBRACE RBRACE                                               { $$ = ast_insert_node(NODE_FUNCBODY, 1, 0); }
            | LBRACE error RBRACE                                         {}
            ;

FunctionBodyDeclaration: FunctionBodyDeclaration Declaration  { $$ = ast_insert_node(NODE_FUNCBODYDECLARATION, 0, 2, $1, $2); }
                       | Declaration                          { $$ = ast_insert_node(NODE_FUNCBODYDECLARATION, 0, 1, $1); }
                       ;

FunctionBodyStatement: FunctionBodyStatement StatementCanError  { $$ = ast_insert_node(NODE_FUNCTIONBODYSTATEMENT, 0, 2, $1, $2); }
                     | Statement                                { $$ = ast_insert_node(NODE_FUNCTIONBODYSTATEMENT, 0, 1, $1); }
                     ;

Declaration: TypeSpec Declarator CommaDeclarator SEMI      { $$ = ast_insert_node(NODE_DECLARATION, 1, 3, $1, $2, $3); } // int a CommaDeclarator;
           | TypeSpec ArrayDeclarator CommaDeclarator SEMI { $$ = ast_insert_node(NODE_ARRAYDECLARATION, 1, 3, $1, $2, $3); }
           | error SEMI                                    {}
           ;

ArrayDeclarator: Asterisk Id LSQ TerminalIntlit RSQ { $$ = ast_insert_node(NODE_ARRAYDECLARATOR, 0, 2, $1, $3); }
               | Id LSQ TerminalIntlit RSQ          { $$ = ast_insert_node(NODE_ARRAYDECLARATOR, 0, 2, $1, $3); }
               ;

CommaDeclarator: CommaDeclarator COMMA Declarator      { $$ = ast_insert_node(NODE_COMMA, 0, 2, $1, $3); } // int a, b, c, d ...*/
               | CommaDeclarator COMMA ArrayDeclarator { $$ = ast_insert_node(NODE_COMMA, 0, 2, $1, $3); }
               | /* empty */ { $$ = NULL; }
               ;

Declarator: Id           { $$ = ast_insert_node(NODE_DECLARATOR, 0, 1, $1); }
          | Asterisk Id  { $$ = ast_insert_node(NODE_DECLARATOR, 0, 2, $1, $2); }
          ;

TerminalIntlit: INTLIT {$$ = ast_insert_terminal(NODE_INTLIT, $1); }
              ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { $$ = ast_insert_node(NODE_FUNCDECLARATION, 1, 2, $1, $2); }
                   ;

TypeSpec: CHAR { $$ = ast_insert_terminal(NODE_CHAR, "Char"); }
        | INT  { $$ = ast_insert_terminal(NODE_INT, "Int"); }
        | VOID { $$ = ast_insert_terminal(NODE_VOID, "Void"); }
        ;

FunctionDeclarator: Id LPAR ParameterList RPAR          { $$ = ast_insert_node(NODE_FUNCDECLARATOR, 0, 2, $1, $3); }
                  | Asterisk Id LPAR ParameterList RPAR { $$ = ast_insert_node(NODE_FUNCDECLARATOR, 0, 3, $1, $2, $4); }
                  ;

ParameterList: ParameterList COMMA ParameterDeclaration { $$ = ast_insert_node(NODE_PARAMLIST, 1, 2, $1, $3); }
             | ParameterDeclaration                     { $$ = ast_insert_node(NODE_PARAMLIST, 0, 1, $1); }
             ;

ParameterDeclaration: TypeSpec Asterisk Id           { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 3, $1, $2, $3); }
                    | TypeSpec Id                    { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 2, $1, $2); }
                    | TypeSpec Asterisk              { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 2, $1, $2); }
                    | TypeSpec                       { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 1, $1); }
                    ;

/*Id: AST Id { $$ = ast_insert_terminal(NODE_POINTER, "Pointer"); } //TODO is this the right way to fix this? no.
  | ID     { $$ = ast_insert_terminal(NODE_ID, $1); }
  ;*/

/*Id: Asterisk ID {  }
  | ID          { $$ = ast_insert_terminal(NODE_ID, $1); }
  ;*/

Id: ID { $$ = ast_insert_terminal(NODE_ID, $1); }
  ;

Asterisk: Asterisk Ast { $$ = ast_insert_node(NODE_POINTER, 0, 2, $1, $2); }
        | Ast          { $$ = ast_insert_node(NODE_POINTER, 0, 1, $1); }
        ;

Ast: AST { $$ = ast_insert_terminal(NODE_POINTER, "Pointer"); }
   ;

StatementCanError: Statement {}
                 | error SEMI {}
                 ;

Statement: CommaExpression SEMI                                                                       { $$ = ast_insert_node(NODE_STATEMENT, 0, 1, $1); }
         | SEMI                                                                                       { }
         | LBRACE StatementList RBRACE                                                                { }
         | LBRACE RBRACE                                                                              { myprintf2("Block Statement\n"); }
         | LBRACE error RBRACE                                                                        { myprintf2("Error Block Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement %prec THEN                                          { $$ = ast_insert_node(NODE_IF, 1, 2, $1, $2); }
         | IF LPAR CommaExpression RPAR Statement ELSE Statement                                      { $$ = ast_insert_node(NODE_IF, 1, 3, $1, $2, $3); }
         | FOR LPAR ForCommaExpression SEMI ForCommaExpression SEMI ForCommaExpression RPAR Statement { $$ = ast_insert_node(NODE_FOR, 1, 4, $1, $2, $3, $4); }
         | RETURN CommaExpression SEMI                                                                { $$ = ast_insert_node(NODE_RETURN, 1, 1, $1); }
         | RETURN SEMI                                                                                { $$ = ast_insert_node(NODE_RETURN, 1, 0); }
         ;

StatementList: StatementList StatementCanError {  }
             | StatementCanError               { myprintf2("StatementList\n"); }
             ;

ForCommaExpression: CommaExpression
                  | /* empty */ {}
                  ;

CommaExpression: CommaExpression COMMA CommaExpression { myprintf2("CommaExpression\n"); }
               | Expression                            { myprintf2("CommaExpression\n"); }
               ;

Expression: Expression ASSIGN Expression         { myprintf2("Expression\n"); }
          | Expression AND Expression            { myprintf2("Expression\n"); }
          | Expression OR Expression             { myprintf2("Expression\n"); }
          | Expression EQ Expression             { $$ = ast_insert_node(NODE_EQ, 1, 2, $1, $3); }
          | Expression NE Expression             { myprintf2("Expression\n"); }
          | Expression LT Expression             { myprintf2("Expression\n"); }
          | Expression GT Expression             { myprintf2("Expression\n"); }
          | Expression LE Expression             { myprintf2("Expression\n"); }
          | Expression GE Expression             { myprintf2("Expression\n"); }
          | Expression AST Expression            { myprintf2("Expression\n"); }
          | Expression PLUS Expression           { myprintf2("Expression\n"); }
          | Expression MINUS Expression          { myprintf2("Expression\n"); }
          | Expression DIV Expression            { myprintf2("Expression\n"); }
          | Expression MOD Expression            { myprintf2("Expression\n"); }
          | AMP Expression                       { myprintf2("Expression\n"); }
          | AST Expression                       { myprintf2("Expression\n"); }
          | PLUS Expression                      { myprintf2("Expression\n"); }
          | MINUS Expression                     { myprintf2("Expression\n"); }
          | NOT Expression                       { myprintf2("Expression\n"); }
          | Expression LSQ CommaExpression RSQ   { myprintf2("Expression\n"); }
          | ID LPAR ExpressionList RPAR          { $$ = ast_insert_node(NODE_CALL, 1, 2, $1, $3); }
          | ID                                   { myprintf2("Expression\n"); }
          | INTLIT                               { $$ = ast_insert_terminal(NODE_INTLIT, $1); }
          | CHRLIT                               { $$ = ast_insert_terminal(NODE_CHRLIT, $1); }
          | STRLIT                               { $$ = ast_insert_terminal(NODE_STRLIT, $1); }
          | LPAR CommaExpression RPAR            { myprintf2("Expression\n"); }
          | LPAR error RPAR                      { myprintf2("Expression\n"); }
          | ID LPAR error RPAR                   { myprintf2("Expression\n"); }
          ;

ExpressionList: CommaExpression { }
              | /* empty */ { $$ = NULL; }
              ;
%%
int yyerror (char *s) {
  where_there_errors = 1;
  printf("Line %d, col %d: %s: %s\n", yylineno, col - (int) yyleng, s, yytext);
  return 0;
}
