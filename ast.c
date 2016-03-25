#include "ast.h"

node_t* ast_insert_node(nodetype_t nodetype, int to_use, int node_operands, ...){
  //insert node in ast
}

node_t* ast_create_node(nodetype_t nodetype) {
  node_t* self = (node_t*) malloc(sizeof(node_t));
  self->type = nodetype;
  self->sibling_right = NULL;
  return self;
}

void ast_print(node_t* ast) {
  printf("printing tree\n");
}
