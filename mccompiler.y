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

  node_t *cur_typespec;
%}

%union{
  char* str;
	struct node *node;
}

%token <str> CHAR ELSE FOR IF INT RETURN VOID RESERVED INTLIT ID
%token <str> AMP AND ASSIGN AST COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE
%token <str> NOT OR PLUS RBRACE RPAR RSQ SEMI CHRLIT STRLIT

%nonassoc THEN

%left COMMA
%right ASSIGN
%left OR
%left AND
%left EQ NE
%left GT LT GE LE
%left PLUS MINUS
%left AST DIV MOD
%right NOT AMP
%left LPAR RPAR LSQ RSQ

%nonassoc ELSE

%type <node> FunctionDefinition FunctionBody FunctionBodyDeclaration FunctionBodyStatement
Declaration Declarator FunctionDeclaration TypeSpec FunctionDeclarator ParameterList
ParameterDeclaration Asterisk StatementCanError Statement StatementList ForCommaExpression
CommaExpression Expression ExpressionList TerminalIntlit Start StartAgain Id Ast DeclarationSecond
CommaExpressionTwo

%%

Start: FunctionDefinition StartAgain  { $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2); }
     | FunctionDeclaration StartAgain { $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2); }
     | Declaration StartAgain         { $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2); }
     ;

StartAgain: FunctionDefinition StartAgain  { $$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2); }
          | FunctionDeclaration StartAgain { $$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2); }
          | Declaration StartAgain         { $$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2); }
          | /* empty */                    { $$ = NULL; }
          ;

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody { $$ = ast_insert_node(NODE_FUNCDEFINITION, 1, 3, $1, $2, $3); }
                  ;

FunctionBody: LBRACE FunctionBodyDeclaration FunctionBodyStatement RBRACE { $$ = ast_insert_node(NODE_FUNCBODY, 1, 2, $2, $3); }
            | LBRACE FunctionBodyStatement RBRACE                         { $$ = ast_insert_node(NODE_FUNCBODY, 1, 1, $2); }
            | LBRACE FunctionBodyDeclaration RBRACE                       { $$ = ast_insert_node(NODE_FUNCBODY, 1, 1, $2); }
            | LBRACE RBRACE                                               { $$ = ast_insert_node(NODE_FUNCBODY, 1, 0); }
            | LBRACE error RBRACE                                         { $$ = NULL;}
            ;

FunctionBodyDeclaration: FunctionBodyDeclaration Declaration  { $$ = ast_insert_node(NODE_FUNCBODYDECLARATION, 0, 2, $1, $2); }
                       | Declaration                          { $$ = ast_insert_node(NODE_FUNCBODYDECLARATION, 0, 1, $1); }
                       ;

FunctionBodyStatement: FunctionBodyStatement StatementCanError  { $$ = ast_insert_node(NODE_FUNCTIONBODYSTATEMENT, 0, 2, $1, $2); }
                     | Statement                                { $$ = ast_insert_node(NODE_FUNCTIONBODYSTATEMENT, 0, 1, $1); }
                     ;

Declaration: TypeSpec DeclarationSecond SEMI  { if ($2 != NULL) { ast_add_typespec($1, $2); $$ = $2; } else { $$ = $2; } }
           | error SEMI                       { $$ = NULL; }
           ;

DeclarationSecond: DeclarationSecond COMMA Declarator { $$ = ast_insert_node(NODE_DECLARATION, 0, 2, $1, $3); }
                 | Declarator                         { $$ = $1; }
                 ;

Declarator: Id                                 { $$ = ast_insert_node(NODE_DECLARATION, 1, 1, $1); }
          | Asterisk Id                        { $$ = ast_insert_node(NODE_DECLARATION, 1, 2, $1, $2); }
          | Id LSQ TerminalIntlit RSQ          { $$ = ast_insert_node(NODE_ARRAYDECLARATION, 1, 2, $1, $3); }
          | Asterisk Id LSQ TerminalIntlit RSQ { $$ = ast_insert_node(NODE_ARRAYDECLARATION, 1, 3, $1, $2, $4); }
          ;

TerminalIntlit: INTLIT { $$ = ast_insert_terminal(NODE_INTLIT, $1); }
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

ParameterList: ParameterList COMMA ParameterDeclaration { $1->to_use = 0; $$ = ast_insert_node(NODE_PARAMLIST, 1, 2, $1, $3); }
             | ParameterDeclaration                     { $$ = ast_insert_node(NODE_PARAMLIST, 1, 1, $1); }
             ;

ParameterDeclaration: TypeSpec Asterisk Id { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 3, $1, $2, $3); }
                    | TypeSpec Id          { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 2, $1, $2); }
                    | TypeSpec Asterisk    { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 2, $1, $2); }
                    | TypeSpec             { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 1, $1); }
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

StatementCanError: Statement { $$ = $1; }
                 | error SEMI { $$ = NULL; }
                 ;

Statement: CommaExpression SEMI                                                                       { $$ = ast_insert_node(NODE_STATEMENT, 0, 1, $1); }
         | SEMI                                                                                       { $$ = NULL; }
         | LBRACE StatementList RBRACE                                                                { if ($2 != NULL && $2->n_childs >= 2) { $$ = ast_insert_node(NODE_STATLIST, 1, 1, $2); } else { $$ = $2; } }
         | LBRACE RBRACE                                                                              { $$ = NULL; }
         | LBRACE error RBRACE                                                                        { $$ = NULL; }
         | IF LPAR CommaExpression RPAR Statement %prec THEN                                          { $3 = ast_fix_to_null($3); $5 = ast_fix_to_null($5); node_t *null_node = ast_insert_node(NODE_NULL, 1, 0); $$ = ast_insert_node(NODE_IF, 1, 3, $3, $5, null_node); }
         | IF LPAR CommaExpression RPAR Statement ELSE Statement                                      { $3 = ast_fix_to_null($3); $5 = ast_fix_to_null($5); $7 = ast_fix_to_null($7); $$ = ast_insert_node(NODE_IF, 1, 3, $3, $5, $7); }
         | FOR LPAR ForCommaExpression SEMI ForCommaExpression SEMI ForCommaExpression RPAR Statement { if ($9 == NULL) {node_t* null_node = ast_insert_node(NODE_NULL, 1, 0); $$ = ast_insert_node(NODE_FOR, 1, 4, $3, $5, $7, null_node);} else { $$ = ast_insert_node(NODE_FOR, 1, 4, $3, $5, $7, $9); } }
         | RETURN CommaExpression SEMI                                                                { $$ = ast_insert_node(NODE_RETURN, 1, 1, $2); }
         | RETURN SEMI                                                                                { node_t* null_node = ast_insert_node(NODE_NULL, 1, 0); $$ = ast_insert_node(NODE_RETURN, 1, 1, null_node); }
         ;

//StatementList StatementCanError { printf("%d\n", $1->n_childs); if($1 == NULL && $2 == NULL) {$$ = ast_insert_node(NODE_STATLIST, 0, 2, $1, $2);} else if (($2 != NULL && $1->n_childs >= 1) || ($1 != NULL && $2 != NULL)){ $$ = ast_insert_node(NODE_STATLIST, 1, 2, $1, $2);}}
// StatementList StatementCanError { int not_nulls = ast_count_not_nulls($1, $2); /*printf("n_nulls: %d\n", not_nulls);*/ if (not_nulls >= 2 && $1->type != 8) { $$ = ast_insert_node(NODE_STATLIST, 1, 2, $1, $2); } else { $$ = ast_insert_node(NODE_STATLIST, 0, 0); } }

StatementList: StatementList StatementCanError { if ($1 == NULL && $2 != NULL) { $$ = ast_insert_node(NODE_STATLIST, 0, 1, $2); } else if ($1 != NULL && $2 == NULL) { $$ = ast_insert_node(NODE_STATLIST, 0, 1, $1); } else if ($1 != NULL && $2 != NULL) $$ = ast_insert_node(NODE_STATLIST, 0, 2, $1, $2); else { $$ = NULL; } }
             | StatementCanError               { $$ = ast_insert_node(NODE_STATLIST, 0, 1, $1); }
             ;

ForCommaExpression: CommaExpression { $$ = $1; }
                  | /* empty */     { $$ = ast_insert_node(NODE_NULL, 1, 0); }
                  ;

CommaExpression: CommaExpression COMMA CommaExpression { $$ = ast_insert_node(NODE_COMMA, 1, 2, $1, $3); }
               | Expression                            { $$ = $1; }
               ;

CommaExpressionTwo: CommaExpressionTwo COMMA CommaExpressionTwo { $$ = ast_insert_node(NODE_COMMA, 0, 2, $1, $3); }
                  | Expression                            { $$ = $1; }
                  ;

Expression: Expression ASSIGN Expression         { $$ = ast_insert_node(NODE_STORE, 1, 2, $1, $3); }
          | Expression AND Expression            { $$ = ast_insert_node(NODE_AND, 1, 2, $1, $3); }
          | Expression OR Expression             { $$ = ast_insert_node(NODE_OR, 1, 2, $1, $3); }
          | Expression EQ Expression             { $$ = ast_insert_node(NODE_EQ, 1, 2, $1, $3); }
          | Expression NE Expression             { $$ = ast_insert_node(NODE_NE, 1, 2, $1, $3); }
          | Expression LT Expression             { $$ = ast_insert_node(NODE_LT, 1, 2, $1, $3); }
          | Expression GT Expression             { $$ = ast_insert_node(NODE_GT, 1, 2, $1, $3); }
          | Expression LE Expression             { $$ = ast_insert_node(NODE_LE, 1, 2, $1, $3); }
          | Expression GE Expression             { $$ = ast_insert_node(NODE_GE, 1, 2, $1, $3); }
          | Expression AST Expression            { $$ = ast_insert_node(NODE_MUL, 1, 2, $1, $3); }
          | Expression PLUS Expression           { $$ = ast_insert_node(NODE_ADD, 1, 2, $1, $3); }
          | Expression MINUS Expression          { $$ = ast_insert_node(NODE_SUB, 1, 2, $1, $3); }
          | Expression DIV Expression            { $$ = ast_insert_node(NODE_DIV, 1, 2, $1, $3); }
          | Expression MOD Expression            { $$ = ast_insert_node(NODE_MOD, 1, 2, $1, $3); }
          | AMP Expression                       { $$ = ast_insert_node(NODE_ADDR, 1, 1, $2); }
          | AST Expression          %prec NOT    { $$ = ast_insert_node(NODE_DEREF, 1, 1, $2); }
          | PLUS Expression         %prec NOT    { $$ = ast_insert_node(NODE_PLUS, 1, 1, $2); }
          | MINUS Expression        %prec NOT    { $$ = ast_insert_node(NODE_MINUS, 1, 1, $2); }
          | NOT Expression                       { $$ = ast_insert_node(NODE_NOT, 1, 1, $2); }
          | Expression LSQ CommaExpression RSQ   { node_t* add = ast_insert_node(NODE_ADD, 1, 2, $1, $3); $$ = ast_insert_node(NODE_DEREF, 1, 1, add); }
          | Id LPAR ExpressionList RPAR          { $$ = ast_insert_node(NODE_CALL, 1, 2, $1, $3); }
          | Id                                   { $$ = ast_insert_node(NODE_EXPRESSION, 0, 1, $1); }
          | INTLIT                               { $$ = ast_insert_terminal(NODE_INTLIT, $1); }
          | CHRLIT                               { $$ = ast_insert_terminal(NODE_CHRLIT, $1); }
          | STRLIT                               { $$ = ast_insert_terminal(NODE_STRLIT, $1); }
          | LPAR CommaExpression RPAR            { $$ = ast_insert_node(NODE_EXPRESSION, 0, 1, $2); }
          | LPAR error RPAR                      { $$ = NULL; }
          | Id LPAR error RPAR                   { $$ = NULL; }
          ;

ExpressionList: CommaExpressionTwo { $$ = $1; }
              | /* empty */        { $$ = NULL; }
              ;
%%
int yyerror (char *s) {
  where_there_errors = 1;
  printf("Line %d, col %d: %s: %s\n", yylineno, col - (int) yyleng, s, yytext);
  return 0;
}
