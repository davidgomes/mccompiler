#include "ast.h"
#include <stdarg.h>
#include <string.h>

node_t *merge_nodes[2048];

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
  "ChrLit",
  "Id",
  "Int",
  "IntLit",
  "Pointer",
  "StrLit",
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

node_t* ast_insert_terminal(nodetype_t nodetype, char* s){
  node_t* new_terminal = ast_create_node(nodetype, 1);
  new_terminal->value = (char *) strdup(s);

  return new_terminal;
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

void ast_print_node(node_t* n){
  if(n->type == NODE_ID || n->type == NODE_CHRLIT || n->type == NODE_INTLIT || n->type == NODE_STRLIT)
    printf("%s(%s)\n", node_types[n->type], n->value);
  else
    printf("%s\n", node_types[n->type]);
}

void ast_print_tree(node_t* n, int d) {
  printf("printing tree\n");
  int i, k;
  for (k = 0; k < d; k++)
    printf("..");

  ast_print_node(n);

  for (i = 0; i < n->n_childs; i++)
    ast_print_tree(n->childs[i], d++);

}
