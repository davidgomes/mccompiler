#include "ast.h"
#include <stdarg.h>
#include <string.h>

node_t *merge_nodes[2048];
int where_there_errors = 0;

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
  "Null",
  "Declarator",
  "FuncDeclarator",
  "FunctionBodyDeclaration",
  "FunctionBodyStatement",
  "Statement",
  "ArrayDeclarator",
  "Block",
  "ProgramBlock"
};

node_t* ast_insert_node(nodetype_t nodetype, int to_use, int node_operands, ...) {
  //printf("Inserting new node: %s\n", node_types[nodetype]);
  node_t *new_node, **tmp;
  int i, nodes = 0;
  va_list args;

  new_node = ast_create_node(nodetype, to_use);

  tmp = merge_nodes;

  va_start(args, node_operands);

  while (node_operands--) {
    node_t *t = va_arg(args, node_t *);

    if (t == NULL) {
      continue;
    } else if(!t->to_use) {
      for (nodes += t->n_childs, i = 0; i < t->n_childs; i++) {
        *tmp++ = t->childs[i];
      }
    } else {
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
  //printf("Inserting new terminal: %s\n", node_types[nodetype]);
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

void ast_add_typespec(node_t *typespec, node_t *declarator) {
  //printf("hey\n");
  //printf("%s\n", comma);

  //printf("node_types[declarator->type] = %s\n", node_types[declarator->type]);
  //printf("node_types[declarator->childs[0]->type] = %s\n", node_types[declarator->childs[0]->type]);
  //printf("n_childs: %d\n", declarator->n_childs);

  if (strcmp(node_types[declarator->type], "ArrayDeclaration") == 0) {
    node_t *tmp1 = declarator->childs[0];
    node_t *tmp2 = declarator->childs[1];

    declarator->n_childs++;

    declarator->childs = (node_t **) malloc (3 * sizeof(node_t *));
    declarator->childs[0] = typespec;
    declarator->childs[1] = tmp1;
    declarator->childs[2] = tmp2;
    return;
  }

  if (declarator->n_childs == 1) {
    node_t *tmp = declarator->childs[0];
    declarator->n_childs++;
    declarator->childs = (node_t **) malloc (2 * sizeof(node_t *));

    declarator->childs[0] = typespec;
    declarator->childs[1] = tmp;
    return;
  }

  int i;
  for (i = 0; i < declarator->n_childs; i++) {
    node_t *child = declarator->childs[i];

    if (strcmp(node_types[child->type], "ArrayDeclaration") == 0) {
      child->n_childs++;

      node_t *tmp1 = child->childs[0];
      node_t *tmp2 = child->childs[1];
      child->childs = (node_t **) malloc (3 * sizeof(node_t *));

      child->childs[0] = typespec;
      child->childs[1] = tmp1;
      child->childs[2] = tmp2;
    } else {
      child->n_childs++;

      node_t *tmp = child->childs[0];
      child->childs = (node_t **) malloc (2 * sizeof(node_t *));

      child->childs[0] = typespec;
      child->childs[1] = tmp;
      }
  }
}

void ast_print_node(node_t* n){
  if(n->type == NODE_ID || n->type == NODE_CHRLIT || n->type == NODE_INTLIT || n->type == NODE_STRLIT)
    printf("%s(%s)\n", node_types[n->type], n->value);
  else
    printf("%s\n", node_types[n->type]);
}

void ast_destroy(node_t *where) {
  if (where != NULL) {
    int i;
    for (i = 0; i < where->n_childs; i++) {
      ast_destroy(where->childs[i]);
    }
    free(where);
  }
}

void ast_print_tree(node_t* n, int d) {
  //printf("printing tree\n");
  int i, k;
  for (k = 0; k < d; k++)
    printf("..");

  ast_print_node(n);

  for (i = 0; i < n->n_childs; i++) {
    ast_print_tree(n->childs[i], d+1);
  }

}
