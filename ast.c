#include "ast.h"
#include <stdarg.h>
#include <string.h>

node_t *ast, *merge_nodes[2048];

char* node_types[] = {
  "Program",
  "Declaration",
  "ArrayDeclaration",
  "FuncDeclaration",
  "FuncDefinition",
  "ParamList",
  "FuncBody",
  "ParamDeclaration",
  "StatList",
  "If",
  "For",
  "Return",
  "Or",
  "And",
  "Eq",
  "Ne",
  "Lt",
  "Gt",
  "Le",
  "Ge",
  "Add",
  "Sub",
  "Mul",
  "Div",
  "Mod",
  "Not",
  "Minus",
  "Plus",
  "Addr",
  "Deref",
  "Store",
  "Comma",
  "Call",
  "Char",
  "Chrlit",
  "Id",
  "Int",
  "Intlit",
  "Pointer",
  "Strlit",
  "Void",
  "Null"
};

node_t* ast_insert_node(nodetype_t nodetype, int to_use, int node_operands, ...){
  node_t *new_node, **tmp;
  int i, nodes = 0;
  va_list args;

  new_node = ast_create_node(nodetype, to_use);

  tmp = merge_nodes;
  va_start(args, node_operands);

  while(node_operands--){
    node_t *t = va_arg(args, node_t *);

    if(t == NULL)
      continue;
    else if(!t->to_use)
      for(nodes += t->n_childs, i = 0; i < t->n_childs; i++)
        *tmp++ = t->childs[i];
    else{
      *tmp++ = t;
      nodes++;
    }

  }

  new_node->childs = (node_t **) malloc (nodes * sizeof(node_t *));
  memcpy(new_node->childs, merge_nodes, nodes * sizeof(node_t *));
  new_node->n_childs = nodes;

  va_end(args);
  return new_node;
}

node_t* ast_create_node(nodetype_t nodetype, int to_use) {
  node_t* self = (node_t*) malloc(sizeof(node_t));
  self->type = nodetype;
  self->sibling_right = NULL;
  self->value = NULL;
  self->to_use = to_use;
  self->n_childs = 0;
  self->childs = NULL;
  return self;
}

void ast_print(node_t* ast) {
  printf("printing tree\n");
}
