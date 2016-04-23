#include "semantics.h"

void print_asterisks2(int n_pointers) {
  int i;

  for (i = 0; i < n_pointers; i++) {
    printf("*");
  }
}

void print_an_node(node_t* n) {
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

void print_an_tree(node_t* n, int d) {
  int i, k;
  for (k = 0; k < d; k++)
    printf("..");

  print_an_node(n);

  for (i = 0; i < n->n_childs; i++) {
    print_an_tree(n->childs[i], d + 1);
  }
}

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

void print_node(node_t *which) {
  printf("%s", type_str[which->an_type]);
  print_asterisks2(which->an_n_pointers);
}

void operator_applied2(nodetype_t operator, node_t *node1, node_t *node2) {
  printf("Operator %s cannot be applied to types ", node_types[operator]);
  print_node(node1);
  printf(", ");
  print_node(node2);
  printf("\n");
}

void set_type_from_st(sym_t *st, node_t *node_id, char* func_name) { // ler também nas declarations para chamada de funcoes nao definidas, no fim.
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

char *get_function_name(node_t *definition_node) {
  int cur_pointer = 1;

  while (definition_node->childs[cur_pointer]->type == NODE_POINTER) {
    cur_pointer++;
  }

  return definition_node->childs[cur_pointer]->value;
}

void set_function_type(sym_t *st, node_t *call_node) {
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

void parse_sub_node(sym_t *st, node_t *sub_node) {
  if (sub_node->childs[0]->an_type == sub_node->childs[1]->an_type) {
    sub_node->an_type = sub_node->childs[0]->an_type;
  }
}

void parse_add_node(sym_t *st, node_t *add_node) {
  if ((((add_node->childs[0]->an_n_pointers >= 1 || add_node->childs[0]->an_array_size >= 1)) ||
        (add_node->childs[1]->an_n_pointers >= 1 || add_node->childs[1]->an_array_size >= 1))) { // one of them is array/pointer

      if (add_node->childs[0]->an_n_pointers == 0 && add_node->childs[0]->an_array_size == -1) { // first is not pointer
        add_node->an_type = add_node->childs[1]->an_type;
        add_node->an_n_pointers = add_node->childs[1]->an_n_pointers >= 1 ? add_node->childs[1]->an_n_pointers : 1;
      } else if (add_node->childs[1]->an_n_pointers == 0 && add_node->childs[1]->an_array_size == -1) {// second is not pointer
        add_node->an_type = add_node->childs[0]->an_type;
        add_node->an_n_pointers = add_node->childs[0]->an_n_pointers >= 1 ? add_node->childs[0]->an_n_pointers : 1;
      } else { // they are both pointers can not add
        operator_applied2(add_node->type, add_node->childs[0], add_node->childs[1]);
      }
  } else if (add_node->childs[0]->an_type == add_node->childs[1]->an_type) {
    add_node->an_type = add_node->childs[0]->an_type;
  }
}

void an_tree(node_t *where, sym_t *st, char *func_name) {
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
    set_type_from_st(st, where, func_name);
  }

  int i;
  if (where->type == NODE_FUNCDEFINITION) {
    func_name = (char *) strdup(get_function_name(where));

    int i;

    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->type == NODE_FUNCBODY) {
        an_tree(where->childs[i], st, func_name);
        break;
      }
    }
  } else {
    for (i = 0; i < where->n_childs; i++) {
      an_tree(where->childs[i], st, func_name);
    }
  }

  if (where->type == NODE_ADD) {
    parse_add_node(st, where);
  } else if (where->type == NODE_SUB) {
    parse_sub_node(st, where);
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
  } else if (where->type == NODE_CALL) {
    set_function_type(st, where);
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