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

  "unknown",
  "undef"
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
  self->an_n_pointers = 0;
  self->an_array_size = -1;

  self->an_n_params = 0;
  self->an_params = (sym_t**) malloc(sizeof(sym_t*));

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

void ast_set_type_from_st(sym_t *st, node_t *node_id, char* func_name) { // ler também nas declarations para chamada de funcoes nao definidas, no fim.
  sym_t *cur_st_node = st->next;

  // first thing to do is verify if node_id corresponds to a function
  if (func_name != NULL) { // find declaration
    while (cur_st_node != NULL && cur_st_node->node_type != FUNC_TABLE) {
      if (cur_st_node->node_type == FUNC_DECLARATION) {
        if (!strcmp(cur_st_node->id, node_id->value)) {
          node_id->an_type = cur_st_node->type;
          node_id->an_n_pointers = cur_st_node->n_pointers;
          node_id->an_params = cur_st_node->params;
          node_id->an_n_params = cur_st_node->n_params;

          int i;
          for (i = 0; i < cur_st_node->n_params; i++) {
            sym_t *param = (sym_t*) malloc(sizeof(sym_t));
            param->type = cur_st_node->params[i]->type;
            param->n_pointers = cur_st_node->params[i]->n_pointers;
            node_id->an_params[i] = param;
          }

          return;
        }
      }

      cur_st_node = cur_st_node->next;
    }
  }

  // if we got here, then it's not a function but yes a variable
  cur_st_node = st->next;

  if (func_name != NULL) { // find declaration
    while (cur_st_node != NULL && cur_st_node->node_type != FUNC_TABLE) {
      if (cur_st_node->node_type == FUNC_DECLARATION) {
        if (!strcmp(cur_st_node->id, func_name)) {
          int i;

          for (i = 0; i < cur_st_node->n_params; i++) {
            if (cur_st_node->params[i]->id != NULL) {
              if (!strcmp(cur_st_node->params[i]->id, node_id->value)) {
                node_id->an_type = cur_st_node->params[i]->type;
                node_id->an_n_pointers = cur_st_node->params[i]->n_pointers;
                node_id->an_array_size = cur_st_node->params[i]->array_size;
                return;
              }
            }
          }
        }
      }

      cur_st_node = cur_st_node->next;
    }
  }

  // check global variables

  cur_st_node = st->next;

  while (cur_st_node != NULL && cur_st_node->node_type != FUNC_TABLE) {
    if (!strcmp(cur_st_node->id, node_id->value)) {
      node_id->an_type = cur_st_node->type;
      node_id->an_n_pointers = cur_st_node->n_pointers;
      node_id->an_array_size = cur_st_node->array_size;
      return;
    }

    cur_st_node = cur_st_node->next;
  }

  cur_st_node = st->next;

  while (cur_st_node != NULL) {
    if (cur_st_node->id != NULL) {
      if (!strcmp(cur_st_node->id, func_name) && cur_st_node->node_type == FUNC_DECLARATION) {
        cur_st_node = cur_st_node->definition->next;

        while (cur_st_node != NULL && cur_st_node->node_type != FUNC_TABLE) {
          if (cur_st_node->id != NULL) {
            if (!strcmp(cur_st_node->id, node_id->value)) {
              node_id->an_type = cur_st_node->type;
              node_id->an_n_pointers = cur_st_node->n_pointers;
              node_id->an_array_size = cur_st_node->array_size;
              return;
            }
          }

          cur_st_node = cur_st_node->next;
        }

        return;
      }
    }

    cur_st_node = cur_st_node->next;
  }

  node_id->an_type = TYPE_UNKNOWN;
}

char *ast_get_function_name(node_t *definition_node) {
  int cur_pointer = 1;

  while (definition_node->childs[cur_pointer]->type == NODE_POINTER) {
    cur_pointer++;
  }

  return definition_node->childs[cur_pointer]->value;
}

void ast_set_function_type(sym_t *st, node_t *call_node) {
  sym_t *cur_st_node = st->next;
  char *func_name = call_node->childs[0]->value;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, func_name)) {
      call_node->an_type = cur_st_node->type;
      call_node->an_n_pointers = cur_st_node->n_pointers;
    }

    cur_st_node = cur_st_node->next;
  }
}

void ast_set_add_type(sym_t *st, node_t *add_node) {
  if (add_node->childs[0]->an_type == add_node->childs[1]->an_type) {
    add_node->an_type = add_node->childs[0]->an_type;
  } else if ((((add_node->childs[0]->an_n_pointers >= 1 || add_node->childs[0]->an_array_size >= 1)) ||
              (add_node->childs[0]->an_n_pointers >= 1 || add_node->childs[0]->an_array_size >= 1))) { // one of them is array/pointer
    if (add_node->childs[0]->an_n_pointers == 0 && add_node->childs[0]->an_array_size == -1) { // first is not pointer
      add_node->an_type = add_node->childs[1]->an_type;
      add_node->an_n_pointers = add_node->childs[1]->an_n_pointers >= 1 ? add_node->childs[1]->an_n_pointers : 1;
    } else if (add_node->childs[1]->an_n_pointers == 0 && add_node->childs[1]->an_array_size == -1) {// second is not pointer
      add_node->an_type = add_node->childs[0]->an_type;
      add_node->an_n_pointers = add_node->childs[0]->an_n_pointers >= 1 ? add_node->childs[0]->an_n_pointers : 1;
    } else { // they are both pointers can not add
      printf("waat\n");
    }
  }
}

void ast_set_sub_type(sym_t *st, node_t *sub_node) {
  if (sub_node->childs[0]->an_type == sub_node->childs[1]->an_type) {
    sub_node->an_type = sub_node->childs[0]->an_type;
  }
}

void ast_an_tree(node_t *where, sym_t *st, char *func_name) {
  if (where->type == NODE_ARRAYDECLARATION || where->type == NODE_FUNCDECLARATION ||
      where->type == NODE_DECLARATION) {
    return;
  }

  if (where->type == NODE_INTLIT || where->type == NODE_CHRLIT) {
    where->an_type = node_type_to_sym_type(where->type);
  }

  if (where->type == NODE_STRLIT) {
    where->an_type = TYPE_CHAR;
    where->an_array_size = strlen(where->value) - 2 + 1; // remove the two quotes ("") and add the null byte
  }

  if (where->type == NODE_ID) {
    ast_set_type_from_st(st, where, func_name);
  }

  int i;
  if (where->type == NODE_FUNCDEFINITION) {
    func_name = (char *) strdup(ast_get_function_name(where));

    int i;

    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->type == NODE_FUNCBODY) {
        ast_an_tree(where->childs[i], st, func_name);
        break;
      }
    }
  } else {
    for (i = 0; i < where->n_childs; i++) {
      ast_an_tree(where->childs[i], st, func_name);
    }
  }

  if (where->type == NODE_ADD) {
    ast_set_add_type(st, where);
  } else if (where->type == NODE_SUB) {
    ast_set_sub_type(st, where);
  } else if (where->type == NODE_COMMA) { // comma gets last child's value
    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->an_type != TYPE_UNKNOWN) {
        where->an_type = where->childs[i]->an_type;
      }
    }
  } else if (where->type == NODE_EQ || where->type == NODE_GT || where->type == NODE_SUB ||
      where->type == NODE_AND || where->type == NODE_NE || where->type == NODE_LT || where->type == NODE_GE ||
      where->type == NODE_LE || where->type == NODE_MUL || where->type == NODE_DIV || where->type == NODE_MOD ||
      where->type == NODE_NOT || where->type == NODE_ADDR) {
    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->an_type != TYPE_UNKNOWN) {
        where->an_type = where->childs[i]->an_type;
      }
    }
  } else if (where->type == NODE_STORE) {
    where->an_type = where->childs[0]->an_type;
    where->an_n_pointers = where->childs[0]->an_n_pointers;
    where->an_array_size = where->childs[0]->an_array_size;
  } else if (where->type == NODE_ADD) {
    int i;

    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->type == NODE_ID) {
        where->an_type = where->childs[i]->an_type;
      }
    }
  } else if (where->type == NODE_CALL) {
    ast_set_function_type(st, where);
  } else if (where->type == NODE_DEREF) {
    where->an_type = where->childs[0]->an_type;
    where->an_n_pointers = where->childs[0]->an_n_pointers - 1;
    where->an_array_size = where->childs[0]->an_array_size;
  }

  if (where->type == NODE_FUNCDEFINITION || where->type == NODE_PROGRAM || where->type == NODE_FUNCBODY ||
      where->type == NODE_FOR || where->type == NODE_RETURN || where->type == NODE_STATLIST || where->type == NODE_IF ||
      where->type == NODE_NULL) {
    return;
  }

  if (where->an_type == TYPE_UNKNOWN) {
    where->an_type = TYPE_UNDEF;
  }
}

void print_asterisks2(int n_pointers) {
  int i;

  for (i = 0; i < n_pointers; i++) {
    printf("*");
  }
}

void ast_print_an_node(node_t* n) {
  if (n->type == NODE_ID || n->type == NODE_CHRLIT || n->type == NODE_INTLIT || n->type == NODE_STRLIT) {
    if (n->an_type == TYPE_UNKNOWN) {
      printf("%s(%s)\n", node_types[n->type], n->value);
    } else if (n->an_n_params > 0) {
      printf("%s(%s) - %s", node_types[n->type], n->value, type_str[n->an_type]);
      print_asterisks2(n->an_n_pointers);
      printf("(");

      int i;

      for (i = 0; i < n->an_n_params; i++) {
        sym_t *arg = n->an_params[i];

        printf("%s", type_str[arg->type]);
        print_asterisks2(arg->n_pointers);

        if (i != n->an_n_params - 1) printf(",");
      }

      printf(")\n");
    } else {
      printf("%s(%s) - %s", node_types[n->type], n->value, type_str[n->an_type]);
      print_asterisks2(n->an_n_pointers);

      if (n->an_array_size > 0) {
        printf("[%d]", n->an_array_size);
      }

      printf("\n");
    }
  } else {
    if (n->an_type == TYPE_UNKNOWN) {
      printf("%s\n", node_types[n->type]);
    } else {
      printf("%s - %s", node_types[n->type], type_str[n->an_type]);
      print_asterisks2(n->an_n_pointers);

      if (n->an_array_size > 0) {
        printf("[%d]", n->an_array_size);
      }

      printf("\n");
    }
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
