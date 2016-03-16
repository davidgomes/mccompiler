#include "ast.h"

node_t* node_create(nodetype_t nodetype) {
  node_t* self = (node_t*) malloc(sizeof(node_t));
  self->type = nodetype;
  self->sibling_right = NULL;
  return self;
}

void print_ast(node_t* ast) {
  printf("printing tree\n");
}
