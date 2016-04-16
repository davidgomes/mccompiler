#include <stdarg.h>
#include <string.h>

#include "ast.h"

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
  "ProgramBlock",
  "NodeExpresion"
};

char *type_str[] = {
  "int",
  "char",
  "void",

  "char",
  "int",

  "unknown"
};

type_t node_type_to_sym_type(nodetype_t type) {
  if (type == NODE_INT || type == NODE_INTLIT) {
    return TYPE_INT;
  } else if (type == NODE_CHAR || type == NODE_CHRLIT) {
    return TYPE_CHAR;
  } else if (type == NODE_VOID) {
    return TYPE_VOID;
  }

  return TYPE_UNKNOWN;
}

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
    } else if (!t->to_use) {
      for (nodes += t->n_childs, i = 0; i < t->n_childs; i++) {
        *tmp++ = t->childs[i];
      }
    } else {
      *tmp++ = t;
      nodes++;
    }
  }

  if (nodes != 0) {
    new_node->childs = (node_t **) malloc (nodes * sizeof(node_t *));
    memcpy(new_node->childs, merge_nodes, nodes * sizeof(node_t *));
    new_node->n_childs = nodes;
  }

  va_end(args);
  return new_node;
}

node_t* ast_insert_terminal(nodetype_t nodetype, char* s) {
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
  self->an_type = TYPE_UNKNOWN;
  return self;
}

node_t *save_nodes[2048];

void _ast_add_typespec_to_declaration(node_t *typespec, node_t *declaration) {
  node_t **ptr = save_nodes;

  int u;
  for (u = 0; u < declaration->n_childs; u++) {
    *ptr++ = declaration->childs[u];
  }

  declaration->n_childs++;
  declaration->childs = (node_t **) malloc (declaration->n_childs * sizeof(node_t*));
  declaration->childs[0] = typespec;

  ptr = save_nodes;
  for (u = 1; u < declaration->n_childs; u++) {
    declaration->childs[u] = *ptr++;
  }
}

void ast_add_typespec(node_t *typespec, node_t *declarator) {
  if (strcmp(node_types[declarator->childs[0]->type], "Declaration") == 0 || strcmp(node_types[declarator->childs[0]->type], "ArrayDeclaration") == 0) {
    int i;
    for (i = 0; i < declarator->n_childs; i++) {
      _ast_add_typespec_to_declaration(typespec, declarator->childs[i]);
    }
  } else {
    _ast_add_typespec_to_declaration(typespec, declarator);
  }
}

node_t *ast_fix_to_null(node_t *which) {
  if (which == NULL) {
    return ast_insert_node(NODE_NULL, 1, 0);
  } else if (strcmp(node_types[which->type], "StatList") == 0 && which->n_childs == 0) {
    return ast_insert_node(NODE_NULL, 1, 0);
  }

  return which;
}

void ast_print_node(node_t* n) {
  if (n->type == NODE_ID || n->type == NODE_CHRLIT || n->type == NODE_INTLIT || n->type == NODE_STRLIT) {
    printf("%s(%s)\n", node_types[n->type], n->value);
  } else {
    printf("%s\n", node_types[n->type]);
  }
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
  int i, k;
  for (k = 0; k < d; k++)
    printf("..");

  ast_print_node(n);

  for (i = 0; i < n->n_childs; i++) {
    ast_print_tree(n->childs[i], d + 1);
  }
}

type_t ast_find_type_in_st(sym_t *st, node_t *node_id, char* func_name) { // ler também nas declarations para chamada de funcoes nao definidas
  sym_t *cur_st_node = st->next;

  if (func_name == NULL) {
    while (cur_st_node != NULL && cur_st_node->node_type != FUNC_TABLE) {
      if (!strcmp(cur_st_node->id, node_id->value)) {
        return cur_st_node->type;
      }

      cur_st_node = cur_st_node->next;
    }

    return TYPE_UNKNOWN;
  }

  while (cur_st_node != NULL) {
    if (cur_st_node->id != NULL) {
      if (!strcmp(cur_st_node->id, func_name)) {
        while (cur_st_node != NULL && cur_st_node->node_type != FUNC_TABLE) {
          if (cur_st_node->id != NULL) {
            if (!strcmp(cur_st_node->id, node_id->value)) {
              return cur_st_node->type;
            }
          }

          cur_st_node = cur_st_node->next;
        }

        return TYPE_UNKNOWN;
      }
    }

    cur_st_node = cur_st_node->next;
  }

  return TYPE_UNKNOWN;
}

char *ast_get_function_name(node_t *definition_node) {
  int cur_pointer = 1;

  while (definition_node->childs[cur_pointer]->type == NODE_POINTER) {
    cur_pointer++;
  }

  return definition_node->childs[cur_pointer]->value;
}

void ast_an_tree(node_t *where, sym_t *st, char *func_name) {
  if (where->type == NODE_ARRAYDECLARATION || where->type == FUNC_DECLARATION ||
      where->type == NODE_DECLARATION) {
    return;
  }

  if (where->type == NODE_INTLIT || where->type == NODE_CHRLIT) {
    where->an_type = node_type_to_sym_type(where->type);
  }

  if (where->type == NODE_ID) {
    where->an_type = ast_find_type_in_st(st, where, func_name);
  }

  int i;
  if (where->type == NODE_FUNCDEFINITION) {
    func_name = (char *) strdup(ast_get_function_name(where));

    int i;

    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->type == NODE_FUNCBODY) {
        ast_an_tree(where->childs[i], st, func_name);
      }
    }
  } else {
    for (i = 0; i < where->n_childs; i++) {
      ast_an_tree(where->childs[i], st, func_name);
    }
  }
}

void ast_print_an_node(node_t* n) {
  if (n->type == NODE_ID || n->type == NODE_CHRLIT || n->type == NODE_INTLIT || n->type == NODE_STRLIT) {
    if (n->an_type == TYPE_UNKNOWN) {
      printf("%s(%s)\n", node_types[n->type], n->value);
    } else {
      printf("%s(%s) - %s\n", node_types[n->type], n->value, type_str[n->an_type]);
    }
  } else {
    printf("%s\n", node_types[n->type]);
  }
}

void ast_print_an_tree(node_t* n, int d) {
  int i, k;
  for (k = 0; k < d; k++)
    printf("..");

  ast_print_an_node(n);

  for (i = 0; i < n->n_childs; i++) {
    ast_print_an_tree(n->childs[i], d + 1);
  }
}
