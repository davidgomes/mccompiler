#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>

typedef enum { NODE_PROGRAM, NODE_DECLARATION, NODE_ARRAYDECLARATION, NODE_FUNCDECLARATION,
  NODE_FUNCDEFINITION, NODE_PARAMLIST, NODE_FUNCBODY, NODE_PARAMDECLARATION, NODE_STATLIST, NODE_IF, NODE_FOR, NODE_RETURN, NODE_OR,
  NODE_AND, NODE_EQ, NODE_NE, NODE_LT, NODE_GT, NODE_LE, NODE_GE, NODE_ADD, NODE_SUB, NODE_MUL,
  NODE_DIV, NODE_MOD, NODE_NOT, NODE_MINUS, NODE_PLUS, NODE_ADDR, NODE_DEREF, NODE_STORE,
  NODE_COMMA, NODE_CALL, NODE_CHAR, NODE_CHRLIT, NODE_ID, NODE_INT, NODE_INTLIT, NODE_POINTER,
  NODE_STRLIT, NODE_VOID, NODE_NULL, NODE_DECLARATOR, NODE_FUNCDECLARATOR, NODE_FUNCBODYDECLARATION, NODE_FUNCTIONBODYSTATEMENT, NODE_STATEMENT, NODE_ARRAYDECLARATOR, NODE_BLOCK, NODE_PROGRAM_BLOCK} nodetype_t;

typedef struct node{
  struct node *sibling_right;
  nodetype_t type;
  char* value;
  int to_use;
  int n_childs;
  struct node **childs;
} node_t;

node_t *ast;
int where_there_errors;

node_t* ast_create_node(nodetype_t nodetype, int to_use);
node_t* ast_insert_node(nodetype_t nodetype, int to_use, int node_operands, ...);
node_t* ast_insert_terminal(nodetype_t nodetype, char* s);
void ast_destroy(node_t *where);
void ast_print_tree(node_t* n, int d);
void ast_print_node(node_t* n);
void ast_add_typespec(node_t *typespec, node_t *declarator);

#endif
