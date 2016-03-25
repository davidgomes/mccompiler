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

node_t* ast_create_node(nodetype_t nodetype);
node_t* ast_insert_node(nodetype_t nodetype, int to_use, int node_operands, ...);
void ast_print(node_t* ast);

#endif
