#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>

typedef enum { NODE_PROGRAM } nodetype_t;

struct {
  struct node_t *sibling_right;
  nodetype_t type;

  int n_childs;
  struct node_t **childs;
} typedef node_t;

node_t* node_create(nodetype_t nodetype);
node_t insert_node(node_t *ast, node_t *new);
void print_ast(node_t* ast);

#endif
