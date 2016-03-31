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
ParameterDeclaration Id Asterisk StatementCanError Statement StatementList ForCommaExpression
CommaExpression Expression ExpressionList TerminalIntlit ArrayDeclarator Start StartAgain

%%

/*Program: Block         { printf("BLOCK"); $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 1, $1); }
       | Program Block { printf("Program Block\n\n\n"); $$ = ast_insert_node(NODE_PROGRAM, 0, 2, $1, $2); }
       ;*/

Start: FunctionDefinition StartAgain  {$$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2);}
     | FunctionDeclaration StartAgain {$$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2);}
     | Declaration StartAgain         {printf("decl\n"); $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 2, $1, $2);}
     ;

StartAgain: FunctionDefinition StartAgain {$$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2);}
          | FunctionDeclaration StartAgain {$$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2);}
          | Declaration StartAgain         {$$ = ast_insert_node(NODE_BLOCK, 0, 2, $1, $2);printf("block\n");}
          | /* empty */ {}
          ;

/*Program: Block Program  { printf("BLOCK"); $$ = ast_insert_node(NODE_PROGRAM, 0, 2, $1, $2); }
       | Block          { printf("Program Block\n\n\n"); $$ = ast = ast_insert_node(NODE_PROGRAM, 1, 1, $1); }
       ;

Block: FunctionDefinition
     | FunctionDeclaration
     | Declaration { printf("segfaultXXX\n"); $$ = ast_insert_node(NODE_BLOCK, 0, 1, $1); printf("test\n"); }
     ;*/

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

Declaration: TypeSpec Declarator CommaDeclarator SEMI      { printf("here\n"); $$ = ast_insert_node(NODE_DECLARATION, 1, 3, $1, $2, $3); printf("hey\n"); } // int a CommaDeclarator;
           | TypeSpec ArrayDeclarator CommaDeclarator SEMI { printf("here2\n"); $$ = ast_insert_node(NODE_ARRAYDECLARATION, 1, 3, $1, $2, $3); }
           | error SEMI                                    {}
           ;

ArrayDeclarator: Id LSQ TerminalIntlit RSQ {$$ = ast_insert_node(NODE_ARRAYDECLARATOR, 0, 2, $1, $3); }
               ;

CommaDeclarator: CommaDeclarator COMMA Declarator { $$ = ast_insert_node(NODE_COMMA, 0, 2, $1, $3);}// int a, b, c, d ...*/
               | /* empty */ { $$ = NULL; }
               ;

Declarator: Id {printf("id\n\n\n"); $$ = ast_insert_node(NODE_DECLARATOR, 0, 1, $1);}
          ;

TerminalIntlit: INTLIT {$$ = ast_insert_terminal(NODE_INTLIT, $1);}
              ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { myprintf2("FunctionDeclaration\n"); }
                   ;

TypeSpec: CHAR { printf("hello\n"); $$ = ast_insert_terminal(NODE_CHAR, "Char"); myprintf2("TypeSpec CHAR\n"); }
        | INT  { printf("hello2\n");$$ = ast_insert_terminal(NODE_INT, "Int"); myprintf2("TypeSpec INT\n"); }
        | VOID { printf("hello3\n");$$ = ast_insert_terminal(NODE_VOID, "Void"); myprintf2("TypeSpec VOID\n"); }
        ;

FunctionDeclarator: Id LPAR ParameterList RPAR  { $$ = ast_insert_node(NODE_FUNCDECLARATOR, 0, 2, $1, $2); myprintf2("FunctionDeclarator\n"); }
                  ;

ParameterList: ParameterList COMMA ParameterDeclaration { $$ = ast_insert_node(NODE_PARAMLIST, 1, 2, $1, $3);}
             | ParameterDeclaration                     { $$ = ast_insert_node(NODE_PARAMLIST, 1, 1, $1);}
             ;

ParameterDeclaration: TypeSpec Asterisk ID  { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 3, $1, $2, $3);}
                    | TypeSpec ID           { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 2, $1, $2);}
                    | TypeSpec Asterisk     { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 2, $1, $2);}
                    | TypeSpec              { $$ = ast_insert_node(NODE_PARAMDECLARATION, 1, 1, $1);}
                    ;

Id: AST Id {$$ = $2;} //TODO is this the right way to fix this? no.
  | ID     { $$ = ast_insert_terminal(NODE_ID, $1); myprintf2("Id\n"); }
  ;

Asterisk: Asterisk AST
        | AST { myprintf2("Asterisk\n"); }
        ;

StatementCanError: Statement {}
                 | error SEMI {}
                 ;

Statement: CommaExpression SEMI                                                                       { $$ = ast_insert_node(NODE_STATEMENT, 0, 1, $1); }
         | SEMI                                                                                       {}
         | LBRACE StatementList RBRACE                                                                {}
         | LBRACE RBRACE                                                                              { myprintf2("Block Statement\n"); }
         | LBRACE error RBRACE                                                                        { myprintf2("Error Block Statement\n"); }
         | IF LPAR CommaExpression RPAR Statement %prec THEN                                          { $$ = ast_insert_node(NODE_IF, 1, 2, $1, $2); }
         | IF LPAR CommaExpression RPAR Statement ELSE Statement                                      { $$ = ast_insert_node(NODE_IF, 1, 3, $1, $2, $3); }
         | FOR LPAR ForCommaExpression SEMI ForCommaExpression SEMI ForCommaExpression RPAR Statement { $$ = ast_insert_node(NODE_FOR, 1, 4, $1, $2, $3, $4); }
         | RETURN CommaExpression SEMI                                                                { $$ = ast_insert_node(NODE_RETURN, 1, 1, $1);}
         | RETURN SEMI                                                                                { $$ = ast_insert_node(NODE_RETURN, 1, 0);}
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
          | Expression EQ Expression             { myprintf2("Expression\n"); }
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
          | ID LPAR ExpressionList RPAR          { myprintf2("Expression\n"); }
          | ID                                   { myprintf2("Expression\n"); }
          | INTLIT                               { myprintf2("Expression\n"); }
          | CHRLIT                               { myprintf2("Expression\n"); }
          | STRLIT                               { myprintf2("Expression\n"); }
          | LPAR CommaExpression RPAR            { myprintf2("Expression\n"); }
          | LPAR error RPAR                      { myprintf2("Expression\n"); }
          | ID LPAR error RPAR                   { myprintf2("Expression\n"); }
          ;

ExpressionList: CommaExpression
              | /* empty */ {}
              ;
%%
int yyerror (char *s) {
  printf("Line %d, col %d: %s: %s\n", yylineno, col - (int) yyleng, s, yytext);
  return 0;
}
