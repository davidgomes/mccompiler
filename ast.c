#include "ast.h"

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
  va_list args;

  return new_node;
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
