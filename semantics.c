#include "semantics.h"

int semantic_errors = 0;

void print_asterisks2(int n_pointers) {
  int i;

  for (i = 0; i < n_pointers; i++) {
    printf("*");
  }
}

void print_an_node(node_t* n, int an) {
  if (!an) {
    ast_print_node(n);
    return;
  }

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

void print_an_tree(node_t* n, int d, int an) {
  int i, k;
  for (k = 0; k < d; k++)
    printf("..");

  print_an_node(n, an);

  for (i = 0; i < n->n_childs; i++) {
    print_an_tree(n->childs[i], d + 1, an);
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

  if (which->an_array_size >= 1) {
    print_asterisks2(which->an_n_pointers + 1);
  } else {
    print_asterisks2(which->an_n_pointers);
  }
}

void print_sym_node2(sym_t *which_node) {
  printf("%s", type_str[which_node->type]);
  print_asterisks2(which_node->n_pointers);
}

void print_node_array(node_t *which) {
  if (which->an_type == TYPE_UNDEF) {
    printf("undef");
    return;
  }

  printf("%s", type_str[which->an_type]);
  print_asterisks2(which->an_n_pointers);

  if (which->an_array_size >= 1) {
    printf("[%d]", which->an_array_size);
  }
}

void print_sym_array(sym_t *which) {
  printf("%s", type_str[which->type]);
  print_asterisks2(which->n_pointers);

  if (which->array_size >= 1) {
    printf("[%d]", which->array_size);
  }
}

void unknown_symbol(node_t *symbol) {
  semantic_errors = 1;
  printf("Line %d, col %d: Unknown symbol %s\n", symbol->loc.first_line, symbol->loc.first_column, symbol->value);
}

void operator_applied1(node_t *operator, node_t *node1) {
  semantic_errors = 1;
  printf("Line %d, col %d: Operator %s cannot be applied to type ", operator->loc.first_line, operator->loc.first_column, node_types_err[operator->type]);
  print_node(node1);
  printf("\n");
}

void print_function_type(sym_t *decl_node) {
  printf("%s", type_str[decl_node->type]);
  print_asterisks2(decl_node->n_pointers);
  printf("(");

  if (decl_node->n_params == 0) {
    printf("void");
  } else {
    int i;
    for (i = 0; i < decl_node->n_params; i++) {
      sym_t *arg = decl_node->params[i];

      if (arg->n_pointers == 0) {
        printf("%s", type_str[arg->type]);
      } else {
        printf("%s", type_str[arg->type]);
        print_asterisks2(arg->n_pointers);
      }

      if (i != decl_node->n_params - 1) {
        printf(",");
      }
    }
  }

  printf(")");
}

sym_t *is_sym_function(sym_t *st, sym_t *which_node) {
  sym_t *cur_st_node = st;

  while (cur_st_node != NULL) {
    if (which_node->id != NULL) {
      if (!strcmp(cur_st_node->id, which_node->id) && cur_st_node->node_type == FUNC_DECLARATION) {
        return cur_st_node;
      }
    }

    cur_st_node = cur_st_node->next;
  }

  return NULL;
}

void operator_applied1_function(node_t *operator, sym_t *decl_node) {
  semantic_errors = 1;
  printf("Line %d, col %d: Operator %s cannot be applied to type ", operator->loc.first_line, operator->loc.first_column, node_types_err[operator->type]);
  print_function_type(decl_node);
  printf("\n");
}

void operator_applied2(node_t *operator, node_t *node1, node_t *node2) {
  semantic_errors = 1;
  printf("Line %d, col %d: Operator %s cannot be applied to types ", operator->loc.first_line, operator->loc.first_column, node_types_err[operator->type]);
  print_node_array(node1);
  printf(", ");
  print_node_array(node2);
  printf("\n");
}

void conflicting_types(node_t *node1, node_t *node2) {
  semantic_errors = 1;
  printf("Line %d, col %d: Conflicting types (got ", node1->loc.first_line, node2->loc.first_column);
  print_node_array(node1);
  printf(", expected ");
  print_node_array(node2);
  printf(")\n");
}

sym_t *is_array(sym_t *st, node_t *which_node, char *func_name) {
  sym_t *cur_st_node;

  cur_st_node = st;
  sym_t *func_node = NULL;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, func_name)) {
      func_node = cur_st_node;
      break;
    }

    cur_st_node = cur_st_node->next;
  }

  if (cur_st_node->definition == NULL) {
    return NULL;
  }

  cur_st_node = func_node->definition->next;

  while (cur_st_node != NULL) {
    if (cur_st_node->id != NULL && which_node->value != NULL) {
      if (!strcmp(cur_st_node->id, which_node->value)) {
        if (cur_st_node->node_type == ARRAY) {
          return cur_st_node;
        }
      }
    }

    cur_st_node = cur_st_node->next;
  }

  // check if it's a global array
  cur_st_node = st;
  while (cur_st_node != NULL) {
    if (which_node->value != NULL) {
      if (!strcmp(cur_st_node->id, which_node->value)) {
        if (cur_st_node->node_type == ARRAY) {
          return cur_st_node;
        }
      }
    }

    cur_st_node = cur_st_node->next;
  }

  return NULL;
}

sym_t *is_function(sym_t *st, node_t *which_node, char *func_name) {
  sym_t *cur_st_node = st;

  if (which_node->value == NULL) {
    return NULL;
  }

  // first, see if there is an override of which->node->value inside function called "func_name"
  if (func_name != NULL) {
    while (cur_st_node != NULL) {
      if (!strcmp(cur_st_node->id, func_name) && cur_st_node->node_type == FUNC_DECLARATION) {
        if (cur_st_node->definition == NULL) break;

        cur_st_node = cur_st_node->definition->next;

        while (cur_st_node != NULL) {
          if (cur_st_node->id != NULL) {
            if (!strcmp(cur_st_node->id, which_node->value)) {
              return NULL;
            }
          }

          cur_st_node = cur_st_node->next;
        }

        break;
      }

      cur_st_node = cur_st_node->next;
    }
  }

  // now check if func_name corresponds to some global function
  cur_st_node = st;
  while (cur_st_node != NULL) {
    if (which_node->value != NULL) {
      if (!strcmp(cur_st_node->id, which_node->value) && cur_st_node->node_type == FUNC_DECLARATION) {
        return cur_st_node;
      }
    }

    cur_st_node = cur_st_node->next;
  }

  return NULL;
}

void parse_id_node(sym_t *st, node_t *node_id, char* func_name, int an) { // ler também nas declarations para chamada de funcoes nao definidas, no fim.
  if (!an) { // todo: check for duplicate but not annotate
    return;
  }

  sym_t *cur_st_node = st->next;

  cur_st_node = st->next;

  if (func_name != NULL) {
    while (cur_st_node != NULL) {
      if (cur_st_node->id != NULL) {
        if (!strcmp(cur_st_node->id, func_name) && cur_st_node->node_type == FUNC_DECLARATION) {
          if (cur_st_node->definition == NULL) break;

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

          break;
        }
      }

      cur_st_node = cur_st_node->next;
    }
  }

  cur_st_node = st->next;

  // second thing to do is verify if node_id corresponds to a function
  if (func_name != NULL) { // find declaration
    while (cur_st_node != NULL) {
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
            param->array_size = -1;
            node_id->an_params[i] = param;
          }

          return;
        }
      }

      cur_st_node = cur_st_node->next;
    }
  }

  // check global variables

  cur_st_node = st->next;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, node_id->value)) {
      node_id->an_type = cur_st_node->type;
      node_id->an_n_pointers = cur_st_node->n_pointers;
      node_id->an_array_size = cur_st_node->array_size;
      return;
    }

    cur_st_node = cur_st_node->next;
  }

  node_id->an_type = TYPE_UNKNOWN;
  unknown_symbol(node_id);
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

void parse_sub_node(sym_t *st, node_t *sub_node, char *func_name) {
  int first_pointers = sub_node->childs[0]->an_n_pointers;
  int second_pointers = sub_node->childs[1]->an_n_pointers;

  if (sub_node->childs[0]->an_array_size >= 1) {
    first_pointers++;
  }

  if (sub_node->childs[1]->an_array_size >= 1) {
    second_pointers++;
  }

  sym_t *func_node1 = is_function(st, sub_node->childs[0], func_name);
  sym_t *func_node2 = is_function(st, sub_node->childs[1], func_name);

  if (func_node1 && !func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", sub_node->loc.first_line, sub_node->loc.first_column, node_types_err[sub_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_node_array(sub_node->childs[1]);
    printf("\n");
    return;
  } else if (!func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", sub_node->loc.first_line, sub_node->loc.first_column, node_types_err[sub_node->type]);
    print_node_array(sub_node->childs[0]);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    return;
  } else if (func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", sub_node->loc.first_line, sub_node->loc.first_column, node_types_err[sub_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    return;
  }

  int first_undef = sub_node->childs[0]->an_type == TYPE_UNDEF;
  int second_undef = sub_node->childs[1]->an_type == TYPE_UNDEF;

  if (first_undef && !second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", sub_node->loc.first_line, sub_node->loc.first_column, node_types_err[sub_node->type]);
    printf("undef");
    printf(", ");
    print_node_array(sub_node->childs[1]);
    printf("\n");
    return;
  } else if (!first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", sub_node->loc.first_line, sub_node->loc.first_column, node_types_err[sub_node->type]);
    print_node_array(sub_node->childs[0]);
    printf(", ");
    printf("undef");
    printf("\n");
    return;
  } else if (first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", sub_node->loc.first_line, sub_node->loc.first_column, node_types_err[sub_node->type]);
    printf("undef");
    printf(", ");
    printf("undef");
    printf("\n");
    return;
  }

  if ((sub_node->childs[0]->an_type == TYPE_VOID && sub_node->childs[0]->an_n_pointers == 0) ||
      (sub_node->childs[1]->an_type == TYPE_VOID && sub_node->childs[1]->an_n_pointers == 0)) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", sub_node->loc.first_line, sub_node->loc.first_column, node_types_err[sub_node->type]);
    print_node_array(sub_node->childs[0]);
    printf(", ");
    print_node_array(sub_node->childs[1]);
    printf("\n");
    return;
  }

  if (first_pointers == 0 && second_pointers == 0) {
    if (sub_node->childs[0]->an_type == TYPE_VOID || sub_node->childs[1]->an_type == TYPE_VOID) {
      operator_applied2(sub_node, sub_node->childs[0], sub_node->childs[1]);
      return;
    }

    sub_node->an_type = TYPE_INT;
  } else if ((first_pointers >= 1 && second_pointers == 0) || (first_pointers == 0 && second_pointers >= 1)) {
    node_t *is_pointer;
    int is_pointer_n_pointers;

    if (first_pointers >= 1) {
      is_pointer = sub_node->childs[0];
      is_pointer_n_pointers = first_pointers;
    } else {
      is_pointer = sub_node->childs[1];
      is_pointer_n_pointers = second_pointers;
    }

    node_t *is_not_pointer;

    if (first_pointers == 0) is_not_pointer = sub_node->childs[0];
    else is_not_pointer = sub_node->childs[1];

    if (is_not_pointer->an_type == TYPE_VOID) {
      operator_applied2(sub_node, sub_node->childs[0], sub_node->childs[1]);
    } else {
      if (is_pointer == sub_node->childs[1]) {
        operator_applied2(sub_node, sub_node->childs[0], sub_node->childs[1]);
      } else {
        sub_node->an_type = is_pointer->an_type;
        sub_node->an_n_pointers = is_pointer_n_pointers;
      }
    }
  } else if (first_pointers >= 1 && second_pointers >= 1) {
    if (first_pointers == second_pointers && sub_node->childs[0]->an_type == TYPE_VOID && sub_node->childs[1]->an_type == TYPE_VOID) {
      sub_node->an_type = TYPE_INT;
    } else {
      if (sub_node->childs[0]->an_type == sub_node->childs[1]->an_type) {
        if (first_pointers == second_pointers) {
          sub_node->an_type = TYPE_INT;
        } else {
          operator_applied2(sub_node, sub_node->childs[0], sub_node->childs[1]);
        }
      } else { // multiple pointers, different type
        node_t *is_there_void_asterisk = NULL;
        node_t *is_not_void_asterisk = NULL;
        int not_void_asterisk_pointers;

        if (sub_node->childs[0]->an_type == TYPE_VOID && first_pointers == 1) {
          is_there_void_asterisk = sub_node->childs[0];
          is_not_void_asterisk = sub_node->childs[1];
          not_void_asterisk_pointers = second_pointers;
        } else if (sub_node->childs[1]->an_type == TYPE_VOID && first_pointers == 1) {
          is_there_void_asterisk = sub_node->childs[1];
          is_not_void_asterisk = sub_node->childs[0];
          not_void_asterisk_pointers = first_pointers;
        }

        if (is_there_void_asterisk) {
          sub_node->an_type = is_not_void_asterisk->an_type;
          sub_node->an_n_pointers = not_void_asterisk_pointers;
        } else {
          operator_applied2(sub_node, sub_node->childs[0], sub_node->childs[1]);
        }
      }
    }
  }
}

void parse_add_node(sym_t *st, node_t *add_node, char * func_name) {
  int first_pointers = add_node->childs[0]->an_n_pointers;
  int second_pointers = add_node->childs[1]->an_n_pointers;

  if (add_node->childs[0]->an_array_size >= 1) {
    first_pointers++;
  }

  if (add_node->childs[1]->an_array_size >= 1) {
    second_pointers++;
  }

  /* array access */
  if (add_node->is_array_access) {
    sym_t *array_node = is_array(st, add_node->childs[0], func_name);

    sym_t *func_node = is_function(st, add_node->childs[0], func_name);
    sym_t *func_node2 = is_function(st, add_node->childs[1], func_name);

    if (second_pointers >= 1) {
      if (func_node != NULL) {
        semantic_errors = 1;
        printf("Line %d, col %d: Operator [ cannot be applied to types ", add_node->loc2.first_line, add_node->loc2.first_column);
        print_function_type(func_node);
        printf(", ");
        print_node_array(add_node->childs[1]);
        printf("\n");

        return;
      }

      semantic_errors = 1;
      printf("Line %d, col %d: Operator [ cannot be applied to types ", add_node->loc2.first_line, add_node->loc2.first_column);
      print_node_array(add_node->childs[0]);
      printf(", ");
      print_node_array(add_node->childs[1]);
      printf("\n");
      return;
    }

    if (func_node != NULL) {
      if (func_node2 != NULL) {
        semantic_errors = 1;
        printf("Line %d, col %d: Operator [ cannot be applied to types ", add_node->loc2.first_line, add_node->loc2.first_column);
        print_function_type(func_node);
        printf(", ");
        print_function_type(func_node2);
        printf("\n");
        return;
      }

      semantic_errors = 1;
      printf("Line %d, col %d: Operator [ cannot be applied to types ", add_node->loc2.first_line, add_node->loc2.first_column);
      print_function_type(func_node);
      printf(", ");
      print_node_array(add_node->childs[1]);
      printf("\n");
      return;
    }

    if (func_node2 != NULL) {
      semantic_errors = 1;
      printf("Line %d, col %d: Operator [ cannot be applied to types ", add_node->loc2.first_line, add_node->loc2.first_column);
      print_node_array(add_node->childs[0]);
      printf(", ");
      print_function_type(func_node2);
      printf("\n");
      return;
    }

    if (array_node == NULL && first_pointers == 0) {
      semantic_errors = 1;
      printf("Line %d, col %d: Operator [ cannot be applied to types ", add_node->loc2.first_line, add_node->loc2.first_column);
      print_node_array(add_node->childs[0]);
      printf(", ");
      print_node_array(add_node->childs[1]);
      printf("\n");

      if ((add_node->childs[0]->an_type == TYPE_INT && first_pointers == 0 && add_node->childs[1]->an_type == TYPE_INT && second_pointers == 0) ||
          (add_node->childs[0]->an_type == TYPE_CHAR && first_pointers == 0 && add_node->childs[1]->an_type == TYPE_CHAR && second_pointers == 0) ||
          (add_node->childs[0]->an_type == TYPE_INT && first_pointers == 0 && add_node->childs[1]->an_type == TYPE_CHAR && second_pointers == 0) ||
          (add_node->childs[0]->an_type == TYPE_CHAR && first_pointers == 0 && add_node->childs[1]->an_type == TYPE_INT && second_pointers == 0)) {
        add_node->an_type = TYPE_INT;
        add_node->has_given_error = 1;
      }

      return;
    }

    if (add_node->childs[1]->an_type != TYPE_INT && add_node->childs[1]->an_type != TYPE_CHAR) {
      semantic_errors = 1;
      printf("Line %d, col %d: Operator [ cannot be applied to types ", add_node->loc2.first_line, add_node->loc2.first_column);
      print_node_array(add_node->childs[0]);
      printf(", ");
      print_node_array(add_node->childs[1]);
      printf("\n");

      return;
    }
  }

  sym_t *func_node1 = is_function(st, add_node->childs[0], func_name);
  sym_t *func_node2 = is_function(st, add_node->childs[1], func_name);

  if (func_node1 && !func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", add_node->loc.first_line, add_node->loc.first_column, node_types_err[add_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_node_array(add_node->childs[1]);
    printf("\n");
    return;
  } else if (!func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", add_node->loc.first_line, add_node->loc.first_column, node_types_err[add_node->type]);
    print_node_array(add_node->childs[0]);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    return;
  } else if (func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", add_node->loc.first_line, add_node->loc.first_column, node_types_err[add_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    return;
  }

  int first_undef = add_node->childs[0]->an_type == TYPE_UNDEF;
  int second_undef = add_node->childs[1]->an_type == TYPE_UNDEF;

  if (first_undef && !second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", add_node->loc.first_line, add_node->loc.first_column, node_types_err[add_node->type]);
    printf("undef");
    printf(", ");
    print_node_array(add_node->childs[1]);
    printf("\n");
    return;
  } else if (!first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", add_node->loc.first_line, add_node->loc.first_column, node_types_err[add_node->type]);
    print_node_array(add_node->childs[0]);
    printf(", ");
    printf("undef");
    printf("\n");
    return;
  } else if (first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", add_node->loc.first_line, add_node->loc.first_column, node_types_err[add_node->type]);
    printf("undef");
    printf(", ");
    printf("undef");
    printf("\n");
    return;
  }

  if ((add_node->childs[0]->an_type == TYPE_VOID && add_node->childs[0]->an_n_pointers == 0) ||
      (add_node->childs[1]->an_type == TYPE_VOID && add_node->childs[1]->an_n_pointers == 0)) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", add_node->loc.first_line, add_node->loc.first_column, node_types_err[add_node->type]);
    print_node_array(add_node->childs[0]);
    printf(", ");
    print_node_array(add_node->childs[1]);
    printf("\n");
    return;
  }

  if (first_pointers >= 1 || second_pointers >= 1) {
    if (first_pointers == 0) { // first is not pointer
      add_node->an_type = add_node->childs[1]->an_type;
      add_node->an_n_pointers = second_pointers >= 1 ? second_pointers : 1;
    } else if (second_pointers == 0) { // second is not pointer
      add_node->an_type = add_node->childs[0]->an_type;
      add_node->an_n_pointers = first_pointers >= 1 ? first_pointers : 1;
    } else { // they are both pointers can not add
      operator_applied2(add_node, add_node->childs[0], add_node->childs[1]);
    }
  } else if (add_node->childs[0]->an_type == add_node->childs[1]->an_type) {
    if (add_node->childs[0]->an_type == TYPE_CHAR) { // both are chars
      add_node->an_type = TYPE_INT;
    } else if (add_node->childs[0]->an_type == TYPE_VOID) {
      operator_applied2(add_node, add_node->childs[0], add_node->childs[1]);
    } else {
      add_node->an_type = add_node->childs[0]->an_type;
    }
  } else {
    if ((add_node->childs[0]->an_type == TYPE_CHAR && add_node->childs[1]->an_type == TYPE_INT) ||
        (add_node->childs[0]->an_type == TYPE_INT && add_node->childs[1]->an_type == TYPE_CHAR)) { // one is char, the other is int

      if (add_node->childs[0]->type == NODE_CHRLIT) {
        add_node->childs[0]->an_type = TYPE_INT;
      }

      if (add_node->childs[1]->type == NODE_CHRLIT) {
        add_node->childs[1]->an_type = TYPE_INT;
      }

      add_node->an_type = TYPE_INT;
    }
  }
}

void parse_mul_div_mod_node(sym_t *st, node_t *which_node, char *func_name) {
  sym_t *func_node1 = is_function(st, which_node->childs[0], func_name);
  sym_t *func_node2 = is_function(st, which_node->childs[1], func_name);

  which_node->an_type = TYPE_INT;

  if (func_node1 && !func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", which_node->loc.first_line, which_node->loc.first_column, node_types_err[which_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_node_array(which_node->childs[1]);
    printf("\n");
    which_node->an_type = TYPE_INT;
    return;
  } else if (!func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", which_node->loc.first_line, which_node->loc.first_column, node_types_err[which_node->type]);
    print_node_array(which_node->childs[0]);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    which_node->an_type = TYPE_INT;
    return;
  } else if (func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", which_node->loc.first_line, which_node->loc.first_column, node_types_err[which_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    which_node->an_type = TYPE_INT;
    return;
  }

  if ((which_node->childs[0]->an_type == TYPE_VOID && which_node->childs[0]->an_n_pointers == 0) ||
      (which_node->childs[1]->an_type == TYPE_VOID && which_node->childs[1]->an_n_pointers == 0)) { // first is void or second is void
    operator_applied2(which_node, which_node->childs[0], which_node->childs[1]);
  } else if ((which_node->childs[0]->an_n_pointers >= 1 || which_node->childs[0]->an_array_size >= 1) ||
             (which_node->childs[1]->an_n_pointers >= 1 || which_node->childs[1]->an_array_size >= 1)) { // first is pointer or second is pointer
    operator_applied2(which_node, which_node->childs[0], which_node->childs[1]);
  } else {
    which_node->an_type = TYPE_INT;
  }
}

void parse_comp_node2(sym_t *st, node_t *comp_node, char *func_name) {
  sym_t *func_node1 = is_function(st, comp_node->childs[0], func_name);
  sym_t *func_node2 = is_function(st, comp_node->childs[1], func_name);

  comp_node->an_type = TYPE_INT;

  int first_pointers = comp_node->childs[0]->an_n_pointers;
  int second_pointers = comp_node->childs[1]->an_n_pointers;

  if (comp_node->childs[0]->an_array_size >= 1) {
    first_pointers++;
  }

  if (comp_node->childs[1]->an_array_size >= 0) {
    second_pointers++;
  }

  int first_void = comp_node->childs[0]->an_type == TYPE_VOID && first_pointers == 0 && func_node1 == NULL;
  int second_void = comp_node->childs[1]->an_type == TYPE_VOID && second_pointers == 0 && func_node2 == NULL;

  if (first_void && second_void) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types void, void\n", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    return;
  } else if (first_void && !second_void) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types void, ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);

    if (func_node2 != NULL) {
      print_function_type(func_node2);
    } else {
      print_node_array(comp_node->childs[1]);
    }

    printf("\n");

    return;
  } else if (!first_void && second_void) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);

    if (func_node1 != NULL) {
      print_function_type(func_node1);
    } else {
      print_node_array(comp_node->childs[0]);
    }

    printf(", void\n");
    return;
  }

  if (func_node1 && !func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_node_array(comp_node->childs[1]);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  } else if (!func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_node_array(comp_node->childs[0]);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  } else if (func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  }

  int first_undef = comp_node->childs[0]->an_type == TYPE_UNDEF;
  int second_undef = comp_node->childs[1]->an_type == TYPE_UNDEF;

  if (first_undef && !second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    printf("undef");
    printf(", ");
    print_node_array(comp_node->childs[1]);
    printf("\n");
  } else if (!first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_node_array(comp_node->childs[0]);
    printf(", ");
    printf("undef");
    printf("\n");

    return;
  } else if (first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    printf("undef");
    printf(", ");
    printf("undef");
    printf("\n");

    return;
  }

  comp_node->an_type = TYPE_INT;
}

void parse_comp_node3(sym_t *st, node_t *comp_node, char *func_name) {
  sym_t *func_node1 = is_function(st, comp_node->childs[0], func_name);
  sym_t *func_node2 = is_function(st, comp_node->childs[1], func_name);

  if (func_node1 && !func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_node_array(comp_node->childs[1]);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  } else if (!func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_node_array(comp_node->childs[0]);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  } else if (func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  }

  int first_pointers = comp_node->childs[0]->an_n_pointers;
  int second_pointers = comp_node->childs[1]->an_n_pointers;

  if (comp_node->childs[0]->an_array_size >= 1) {
    first_pointers++;
  }

  if (comp_node->childs[1]->an_array_size >= 1) {
    second_pointers++;
  }

  if (((comp_node->childs[0]->an_type == TYPE_VOID && first_pointers == 0) ||
       (comp_node->childs[1]->an_type == TYPE_VOID && second_pointers == 0)) && first_pointers == 0 && second_pointers == 0) { // first is void or second is void
    operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]);
    comp_node->an_type = TYPE_INT;
  } else if (comp_node->childs[0]->an_type == TYPE_UNDEF || comp_node->childs[1]->an_type == TYPE_UNDEF) {
    operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]);
    comp_node->an_type = TYPE_INT;
  } else if (comp_node->childs[0]->an_type != comp_node->childs[1]->an_type) {
    if (((comp_node->childs[0]->an_type == TYPE_CHAR && comp_node->childs[1]->an_type == TYPE_INT) ||
         (comp_node->childs[0]->an_type == TYPE_INT && comp_node->childs[1]->an_type == TYPE_CHAR)) && first_pointers == 0 && second_pointers == 0) {

    } else if ((comp_node->childs[0]->an_type == TYPE_VOID && first_pointers >= 1) && second_pointers >= 1) {

    } else if ((comp_node->childs[1]->an_type == TYPE_VOID && second_pointers >= 1) && first_pointers >= 1) {

    } else {
      node_t* is_zero = NULL;

      if (first_pointers == 0 && comp_node->childs[0]->value != NULL && strlen(comp_node->childs[0]->value) >= 1) {
        if (second_pointers >= 1 && comp_node->childs[0]->value[0] == '0') {
          is_zero = comp_node->childs[0];
        }
      }

      if (second_pointers == 0 && comp_node->childs[1]->value != NULL && strlen(comp_node->childs[1]->value) >= 1) {
        if (first_pointers >= 1 && comp_node->childs[1]->value[0] == '0') {
          is_zero = comp_node->childs[1];
        }
      }

      if (is_zero == NULL) {
        operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]); // here
      }

      comp_node->an_type = TYPE_INT;
    }

    comp_node->an_type = TYPE_INT;
  } else if (comp_node->childs[0]->an_type == comp_node->childs[1]->an_type) {
    if (first_pointers != second_pointers) {
      node_t* is_zero = NULL;

      if (first_pointers == 0 && comp_node->childs[0]->value != NULL && strlen(comp_node->childs[0]->value) >= 1) {
        if (second_pointers >= 1 && comp_node->childs[0]->value[0] == '0') {
          is_zero = comp_node->childs[0];
        }
      }

      if (second_pointers == 0 && comp_node->childs[1]->value != NULL && strlen(comp_node->childs[1]->value) >= 1) {
        if (first_pointers >= 1 && comp_node->childs[1]->value[0] == '0') {
          is_zero = comp_node->childs[1];
        }
      }

      if (is_zero == NULL) {
        operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]); // here
      }

      comp_node->an_type = TYPE_INT;
    } else { // void
      if (first_pointers == second_pointers || first_pointers == 1 || second_pointers == 1) {

      } else {
        operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]);
      }
    }

    comp_node->an_type = TYPE_INT;
  } else {
    comp_node->an_type = TYPE_INT;
  }
}

void parse_comp_node(sym_t *st, node_t *comp_node, char *func_name) {
  sym_t *func_node1 = is_function(st, comp_node->childs[0], func_name);
  sym_t *func_node2 = is_function(st, comp_node->childs[1], func_name);

  if (func_node1 && !func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_node_array(comp_node->childs[1]);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  } else if (!func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_node_array(comp_node->childs[0]);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  } else if (func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comp_node->loc.first_line, comp_node->loc.first_column, node_types_err[comp_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");

    comp_node->an_type = TYPE_INT;

    return;
  }

  int first_pointers = comp_node->childs[0]->an_n_pointers;
  int second_pointers = comp_node->childs[1]->an_n_pointers;

  if (comp_node->childs[0]->an_array_size >= 1) {
    first_pointers++;
  }

  if (comp_node->childs[1]->an_array_size >= 1) {
    second_pointers++;
  }

  if (((comp_node->childs[0]->an_type == TYPE_VOID && first_pointers == 0) ||
       (comp_node->childs[1]->an_type == TYPE_VOID && second_pointers == 0)) && first_pointers == 0 && second_pointers == 0) { // first is void or second is void
    operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]);
    comp_node->an_type = TYPE_INT;
  } else if (comp_node->childs[0]->an_type == TYPE_UNDEF || comp_node->childs[1]->an_type == TYPE_UNDEF) {
    operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]);
    comp_node->an_type = TYPE_INT;
  } else if (comp_node->childs[0]->an_type != comp_node->childs[1]->an_type) {
    if (((comp_node->childs[0]->an_type == TYPE_CHAR && comp_node->childs[1]->an_type == TYPE_INT) ||
         (comp_node->childs[0]->an_type == TYPE_INT && comp_node->childs[1]->an_type == TYPE_CHAR)) && first_pointers == 0 && second_pointers == 0) {

    } else if ((comp_node->childs[0]->an_type == TYPE_VOID && first_pointers >= 1) && second_pointers >= 1) {

    } else if ((comp_node->childs[1]->an_type == TYPE_VOID && second_pointers >= 1) && first_pointers >= 1) {

    } else {
      node_t* is_zero = NULL;

      if (first_pointers == 0 && comp_node->childs[0]->value != NULL && strlen(comp_node->childs[0]->value) >= 1) {
        if (second_pointers >= 1 && comp_node->childs[0]->value[0] == '0') {
          is_zero = comp_node->childs[0];
        }
      }

      if (second_pointers == 0 && comp_node->childs[1]->value != NULL && strlen(comp_node->childs[1]->value) >= 1) {
        if (first_pointers >= 1 && comp_node->childs[1]->value[0] == '0') {
          is_zero = comp_node->childs[1];
        }
      }

      if (is_zero == NULL) {
        operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]); // here
      }

      comp_node->an_type = TYPE_INT;
    }

    comp_node->an_type = TYPE_INT;
  } else if (comp_node->childs[0]->an_type == comp_node->childs[1]->an_type) {
    if (first_pointers != second_pointers) {
      node_t* is_zero = NULL;

      if (first_pointers == 0 && comp_node->childs[0]->value != NULL && strlen(comp_node->childs[0]->value) >= 1) {
        if (second_pointers >= 1 && comp_node->childs[0]->value[0] == '0') {
          is_zero = comp_node->childs[0];
        }
      }

      if (second_pointers == 0 && comp_node->childs[1]->value != NULL && strlen(comp_node->childs[1]->value) >= 1) {
        if (first_pointers >= 1 && comp_node->childs[1]->value[0] == '0') {
          is_zero = comp_node->childs[1];
        }
      }

      if ((comp_node->childs[0]->an_type == TYPE_VOID && first_pointers >= 1) && second_pointers >= 1) {

      } else if ((comp_node->childs[1]->an_type == TYPE_VOID && second_pointers >= 1) && first_pointers >= 1) {

      } else {
        if (is_zero == NULL) {
          operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]); // here
        }
      }

      comp_node->an_type = TYPE_INT;
    } else { // void
      if (first_pointers == second_pointers || first_pointers == 1 || second_pointers == 1) {

      } else {
        operator_applied2(comp_node, comp_node->childs[0], comp_node->childs[1]);
      }
    }

    comp_node->an_type = TYPE_INT;
  } else {
    comp_node->an_type = TYPE_INT;
  }
}

void parse_comma_node(sym_t *st, node_t *comma_node, char *func_name) {
  sym_t *func_node1 = is_function(st, comma_node->childs[0], func_name);
  sym_t *func_node2 = is_function(st, comma_node->childs[1], func_name);

  int first_undef = comma_node->childs[0]->an_type == TYPE_UNDEF;
  int second_undef = comma_node->childs[1]->an_type == TYPE_UNDEF;

  if (func_node1 && !func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comma_node->loc.first_line, comma_node->loc.first_column, node_types_err[comma_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_node_array(comma_node->childs[1]);
    printf("\n");
  } else if (!func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comma_node->loc.first_line, comma_node->loc.first_column, node_types_err[comma_node->type]);
    print_node_array(comma_node->childs[0]);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    return;
  } else if (func_node1 && func_node2) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comma_node->loc.first_line, comma_node->loc.first_column, node_types_err[comma_node->type]);
    print_function_type(func_node1);
    printf(", ");
    print_function_type(func_node2);
    printf("\n");
    return;
  }

  if (first_undef && !second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comma_node->loc.first_line, comma_node->loc.first_column, node_types_err[comma_node->type]);
    printf("undef");
    printf(", ");
    print_node_array(comma_node->childs[1]);
    printf("\n");
  } else if (!first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comma_node->loc.first_line, comma_node->loc.first_column, node_types_err[comma_node->type]);
    print_node_array(comma_node->childs[0]);
    printf(", ");
    printf("undef");
    printf("\n");

    return;
  } else if (first_undef && second_undef) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", comma_node->loc.first_line, comma_node->loc.first_column, node_types_err[comma_node->type]);
    printf("undef");
    printf(", ");
    printf("undef");
    printf("\n");

    return;
  }

  int i;
  for (i = 0; i < comma_node->n_childs; i++) {
    if (comma_node->childs[i]->an_type != TYPE_UNKNOWN) {
      comma_node->an_type = comma_node->childs[i]->an_type;
      comma_node->an_n_pointers = comma_node->childs[i]->an_n_pointers;

      if (comma_node->childs[i]->an_array_size >= 1) {
        comma_node->an_n_pointers++;
      }
    }
  }
}

void parse_not_node(sym_t *st, node_t *not_node, char *func_name) {
  sym_t *func_node = is_function(st, not_node->childs[0], func_name);

  not_node->an_type = TYPE_INT;

  if (not_node->childs[0]->an_type == TYPE_UNDEF) {
    not_node->an_type = TYPE_INT;
    operator_applied1(not_node, not_node->childs[0]);
    return;
  }

  if (func_node != NULL) {
    operator_applied1_function(not_node, func_node);
    not_node->an_type = TYPE_INT;
    return;
  }

  if (not_node->childs[0]->an_type == TYPE_VOID && not_node->childs[0]->an_n_pointers == 0) {
    operator_applied1(not_node, not_node->childs[0]);
    not_node->an_type = TYPE_INT;
  } else {
    not_node->an_type = TYPE_INT;
  }
}

void parse_minus_plus_node(sym_t *st, node_t *which_node, char *func_name) {
  sym_t *func_node = is_function(st, which_node->childs[0], func_name);

  which_node->an_type = TYPE_INT;

  if (which_node->childs[0]->an_type == TYPE_UNDEF) {
    operator_applied1(which_node, which_node->childs[0]);
    which_node->an_type = TYPE_INT;
    return;
  }

  if (func_node != NULL) {
    operator_applied1_function(which_node, func_node);
    which_node->an_type = TYPE_INT;
    return;
  }

  if (which_node->childs[0]->type == NODE_ID) {
    sym_t *array_node = is_array(st, which_node->childs[0], func_name);

    if (array_node != NULL) {
      semantic_errors = 1;
      printf("Line %d, col %d: Operator %s cannot be applied to type ", which_node->loc.first_line, which_node->loc.first_column, node_types_err[which_node->type]);
      printf("%s[%d]\n", type_str[array_node->type], array_node->array_size);
      which_node->an_type = TYPE_INT;

      return;
    }
  }

  sym_t *array_node = is_array(st, which_node->childs[0], func_name);

  if (array_node) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to type ", which_node->loc.first_line, which_node->loc.first_column, node_types_err[which_node->type]);
    printf("%s[%d]\n", type_str[array_node->type], array_node->array_size);
    which_node->an_type = TYPE_INT;
    return;
  }

  if (which_node->childs[0]->an_n_pointers > 0) {
    operator_applied1(which_node, which_node->childs[0]);
    which_node->an_type = TYPE_INT;
    return;
  }

  if (which_node->childs[0]->an_type == TYPE_VOID) {
    operator_applied1(which_node, which_node->childs[0]);
  } else if (which_node->childs[0]->an_type == TYPE_CHAR) {
    which_node->an_type = TYPE_INT;
  } else {
    which_node->an_type = which_node->childs[0]->an_type;
  }
}

void parse_deref_node(sym_t *st, node_t *deref_node, char *func_name) {
  sym_t *func_node = is_function(st, deref_node->childs[0], func_name);

  if (func_node != NULL) {
    operator_applied1_function(deref_node, func_node);
    return;
  }

  if (deref_node->childs[0]->type == NODE_ADD && deref_node->childs[0]->an_type == TYPE_UNDEF) {
    return;
  }

  int child_pointers = deref_node->childs[0]->an_n_pointers;

  if (deref_node->childs[0]->an_array_size >= 1) {
    child_pointers++;
  }

  if (child_pointers == 0) {
    if (deref_node->childs[0]->type == NODE_ADD) {
      if (!deref_node->childs[0]->has_given_error) {
        semantic_errors = 1;
        printf("Line %d, col %d: Operator [ cannot be applied to types ", deref_node->childs[0]->loc2.first_line, deref_node->childs[0]->loc2.first_column);
        print_node_array(deref_node->childs[0]->childs[0]);
        printf(", ");
        print_node_array(deref_node->childs[0]->childs[1]);
        printf("\n");

        return;
      } else {
        return;
      }
    }
  }

  if (deref_node->childs[0]->an_type == TYPE_VOID && deref_node->childs[0]->an_n_pointers == 0) {
    operator_applied1(deref_node, deref_node->childs[0]);
  } else if (deref_node->childs[0]->an_n_pointers == 0 && ! (deref_node->childs[0]->an_array_size >= 1)) {
    operator_applied1(deref_node, deref_node->childs[0]);
  } else {
    if (deref_node->childs[0]->an_array_size >= 0) {
      deref_node->an_type = deref_node->childs[0]->an_type;
      deref_node->an_n_pointers = deref_node->childs[0]->an_n_pointers + 1 - 1;
    } else {
      deref_node->an_type = deref_node->childs[0]->an_type;
      deref_node->an_n_pointers = deref_node->childs[0]->an_n_pointers - 1;
      deref_node->an_array_size = deref_node->childs[0]->an_array_size;
    }
  }
}

void parse_addr_node(sym_t *st, node_t *addr_node, char *func_name) {
  sym_t *func_node = is_function(st, addr_node->childs[0], func_name);

  if (func_node != NULL) {
    operator_applied1_function(addr_node, func_node);
    return;
  }

  if (addr_node->childs[0]->an_type == TYPE_UNDEF) {
    operator_applied1(addr_node, addr_node->childs[0]);
    return;
  }

  if (addr_node->childs[0]->an_array_size >= 1) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to type ", addr_node->loc.first_line, addr_node->loc.first_column, node_types_err[addr_node->type]);
    print_node_array(addr_node->childs[0]);
    printf("\n");
    return;
  }

  int id_found = addr_node->childs[0]->type == NODE_ID ||
                 (addr_node->childs[0]->type == NODE_DEREF && addr_node->childs[0]->an_type != TYPE_UNDEF &&
                  addr_node->childs[0]->an_type != TYPE_UNKNOWN);

  if (!id_found) {
    semantic_errors = 1;
    printf("Line %d, col %d: Lvalue required\n", addr_node->childs[0]->loc.first_line, addr_node->childs[0]->loc.first_column);
    addr_node->an_type = addr_node->childs[0]->an_type;
    addr_node->an_n_pointers = addr_node->childs[0]->an_n_pointers + 1;
  } else {
    // ver se childs[0] for o Id de uma array tambem se da erro
    sym_t *array_node = NULL;

    if (addr_node->childs[0]->type == NODE_ID) {
      array_node = is_array(st, addr_node->childs[0], func_name);
    }

    if (array_node) {
      semantic_errors = 1;
      printf("Line %d, col %d: Operator %s cannot be applied to type ", addr_node->loc.first_line, addr_node->loc.first_column, node_types_err[addr_node->type]);
      printf("%s[%d]\n", type_str[array_node->type], array_node->array_size);
    } else {
      addr_node->an_type = addr_node->childs[0]->an_type;
      addr_node->an_n_pointers = addr_node->childs[0]->an_n_pointers + 1;
    }
  }
}

void parse_store_node(sym_t *st, node_t *store_node, char *func_name) {
  sym_t *func_node = is_function(st, store_node->childs[1], func_name);

  int id_found = store_node->childs[0]->type == NODE_ID ||
                 (store_node->childs[0]->type == NODE_DEREF && store_node->childs[0]->an_type != TYPE_UNDEF &&
                 store_node->childs[0]->an_type != TYPE_UNKNOWN);

  store_node->an_type = store_node->childs[0]->an_type;
  store_node->an_n_pointers = store_node->childs[0]->an_n_pointers;
  store_node->an_array_size = store_node->childs[0]->an_array_size;

  if (!id_found) {
    semantic_errors = 1;
    printf("Line %d, col %d: Lvalue required\n", store_node->childs[0]->loc.first_line, store_node->childs[0]->loc.first_column);
    store_node->an_type = store_node->childs[0]->an_type;
    store_node->an_n_pointers = store_node->childs[0]->an_n_pointers;
    store_node->an_array_size = store_node->childs[0]->an_array_size;
    return;
  }

  if (func_node != NULL) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", store_node->loc.first_line, store_node->loc.first_column, node_types_err[store_node->type]);
    print_node(store_node->childs[0]);
    printf(", ");
    print_function_type(func_node);
    printf("\n");

    return;
  }

  if (store_node->childs[1]->an_type == TYPE_VOID && store_node->childs[1]->an_n_pointers == 0) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", store_node->loc.first_line, store_node->loc.first_column, node_types_err[store_node->type]);
    print_node_array(store_node->childs[0]);
    printf(", ");
    print_node_array(store_node->childs[1]);
    printf("\n");

    return;
  }

  // verify if node can be stored or not

  if (store_node->childs[1]->an_type == TYPE_UNDEF) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", store_node->loc.first_line, store_node->loc.first_column, node_types_err[store_node->type]);
    print_node_array(store_node->childs[0]);
    printf(", ");
    print_node_array(store_node->childs[1]);
    printf("\n");

    return;
  }

  if (store_node->childs[0]->an_array_size >= 1) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", store_node->loc.first_line, store_node->loc.first_column, node_types_err[store_node->type]);
    print_node_array(store_node->childs[0]);
    printf(", ");
    print_node_array(store_node->childs[1]);
    printf("\n");

    return;
  }

  int first_pointers = store_node->childs[0]->an_n_pointers;
  int second_pointers = store_node->childs[1]->an_n_pointers;

  if (store_node->childs[0]->an_array_size >= 1) {
    first_pointers++;
  }

  if (store_node->childs[1]->an_array_size >= 1) {
    second_pointers++;
  }

  int error = 1;

  if (((store_node->childs[0]->an_type == TYPE_INT && store_node->childs[1]->an_type == TYPE_CHAR) ||
       (store_node->childs[0]->an_type == TYPE_CHAR && store_node->childs[1]->an_type == TYPE_INT)) &&
      first_pointers == 0 && second_pointers == 0) { // one is int, other is char
    error = 0;

  } else if (store_node->childs[0]->an_type == store_node->childs[1]->an_type) {
    if (first_pointers == second_pointers) { // same type, same pointer
      error = 0;
    }

    if ((store_node->childs[0]->an_type == TYPE_VOID && first_pointers == 1) ||
        (store_node->childs[1]->an_type == TYPE_VOID && second_pointers == 1)) {
      error = 0;
    }

    if (first_pointers >= 1 && store_node->childs[1]->value != NULL && strlen(store_node->childs[1]->value) >= 1 && second_pointers == 0 && store_node->childs[1]->value[0] == '0') {
      error = 0;
    }
  } else if (store_node->childs[0]->an_type == TYPE_VOID && first_pointers == 1) {
    if (second_pointers >= 1) { // one is void* and the other is some sort of pointer
      error = 0;
    }

    if (first_pointers >= 1 && store_node->childs[1]->value != NULL && strlen(store_node->childs[1]->value) >= 1 && second_pointers == 0 && store_node->childs[1]->value[0] == '0') {
      error = 0;
    }
  } else if (store_node->childs[1]->an_type == TYPE_VOID && second_pointers == 1) {
    if (first_pointers >= 1) { // one is void* and the other is some sort of pointer
      error = 0;
    }
  } else {
    if (first_pointers >= 1 && store_node->childs[1]->value != NULL && strlen(store_node->childs[1]->value) >= 1 && second_pointers == 0 && store_node->childs[1]->value[0] == '0') {
      error = 0;
    }
  }

  if (error) {
    semantic_errors = 1;
    printf("Line %d, col %d: Operator %s cannot be applied to types ", store_node->loc.first_line, store_node->loc.first_column, node_types_err[store_node->type]);
    print_node_array(store_node->childs[0]);
    printf(", ");
    print_node_array(store_node->childs[1]);
    printf("\n");
  } else {
    store_node->an_type = store_node->childs[0]->an_type;
    store_node->an_n_pointers = store_node->childs[0]->an_n_pointers;
    store_node->an_array_size = store_node->childs[0]->an_array_size;
  }
}

void parse_call_node(sym_t *st, node_t *call_node, int an, char *func_name) {
  if (an) {
    set_function_type(st, call_node);
  }

  int args_sent_in = call_node->n_childs - 1;

  int expected_args;

  sym_t *cur_st_node = st;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, call_node->childs[0]->value)) {
      expected_args = cur_st_node->n_params;

      if (expected_args == 1) {
        if (cur_st_node->params[0]->type == TYPE_VOID && cur_st_node->params[0]->n_pointers == 0) {
          expected_args = 0;
        }
      }

      break;
    }

    cur_st_node = cur_st_node->next;
  }

  if (cur_st_node == NULL) {
    semantic_errors = 1;
    printf("Line %d, col %d: Symbol %s is not a function\n", call_node->loc.first_line, call_node->loc.first_column, call_node->childs[0]->value);
    return;
  }

  if (args_sent_in != expected_args) {
    semantic_errors = 1;
    printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n", call_node->loc.first_line, call_node->loc.first_column, call_node->childs[0]->value, args_sent_in, expected_args);
    return;
  }

  int i;
  for (i = 0; i < args_sent_in && i < expected_args; i++) {
    int child_pointers = call_node->childs[1 + i]->an_n_pointers;

    if (call_node->childs[1 + i]->an_array_size >= 1) {
      child_pointers++;
    }

    sym_t *func_node = is_function(st, call_node->childs[1 + i], func_name);

    if (func_node != NULL) {
      semantic_errors = 1;
      printf("Line %d, col %d: Conflicting types (got ", call_node->childs[i + 1]->loc.first_line, call_node->childs[i + 1]->loc.first_column);
      print_function_type(func_node);
      printf(", expected ");
      printf("%s", type_str[cur_st_node->params[i]->type]);
      print_asterisks2(cur_st_node->params[i]->n_pointers);
      printf(")\n");

      continue;
    }

    int expected_pointers = cur_st_node->params[i]->n_pointers;

    if ((expected_pointers != child_pointers || call_node->childs[1 + i]->an_type != cur_st_node->params[i]->type) || (call_node->childs[1 + i]->an_type == TYPE_VOID && child_pointers == 0)) {
      if (call_node->childs[i + 1]->value != NULL && strlen(call_node->childs[i + 1]->value) >= 1) {
        if (expected_pointers >= 1 && call_node->childs[i + 1]->value[0] == '0') {
          continue;
        }
      }

      if ((call_node->childs[1 + i]->an_type == TYPE_VOID && child_pointers == 1) || (cur_st_node->params[i]->type == TYPE_VOID && expected_pointers == 1 && child_pointers >= 1)) {
        continue;
      }

      if (((call_node->childs[1 + i]->an_type == TYPE_INT && cur_st_node->params[i]->type == TYPE_CHAR) ||
           (call_node->childs[1 + i]->an_type == TYPE_CHAR && cur_st_node->params[i]->type == TYPE_INT)) && expected_pointers == 0 && child_pointers == 0) {
        continue;
      }

      semantic_errors = 1;
      printf("Line %d, col %d: Conflicting types (got ", call_node->childs[i + 1]->loc.first_line, call_node->childs[i + 1]->loc.first_column);
      print_node(call_node->childs[i + 1]);
      printf(", expected ");
      printf("%s", type_str[cur_st_node->params[i]->type]);
      print_asterisks2(cur_st_node->params[i]->n_pointers);
      printf(")\n");
    }
  }
}

void parse_return_node(sym_t *st, node_t *return_node, char *func_name) { // TODO accept return int[] when expects int*
  sym_t *cur_st_node = st;
  type_t expected_type = TYPE_UNKNOWN;
  int expected_pointers;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, func_name)) {
      expected_type = cur_st_node->type;
      expected_pointers = cur_st_node->n_pointers;
      break;
    }

    cur_st_node = cur_st_node->next;
  }

  sym_t *func_node = is_function(st, return_node->childs[0], func_name);

  if (func_node != NULL) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", return_node->loc.first_line, return_node->loc.first_column);

    printf("%s", type_str[return_node->childs[0]->an_type]);
    print_asterisks2(return_node->childs[0]->an_n_pointers);
    printf("(");

    int i;
    for (i = 0; i < return_node->childs[0]->an_n_params; i++) {
      sym_t *arg = return_node->childs[0]->an_params[i];

      printf("%s", type_str[arg->type]);
      print_asterisks2(arg->n_pointers);

      if (i != return_node->childs[0]->an_n_params - 1) printf(",");
    }

    printf(")");

    printf(", expected ");
    printf("%s", type_str[expected_type]);
    print_asterisks2(expected_pointers);
    printf(")\n");
    return;
  }

  if (expected_type != return_node->childs[0]->an_type ||
      (expected_type == return_node->childs[0]->an_type &&
       expected_pointers != return_node->childs[0]->an_n_pointers)) {

    if (expected_pointers == 0 && return_node->childs[0]->an_n_pointers == 0) { // none is pointer
      if ((expected_type == TYPE_CHAR && return_node->childs[0]->an_type == TYPE_INT) ||
          (expected_type == TYPE_INT && return_node->childs[0]->an_type == TYPE_CHAR)) { // if one is int and the other is char no problem
        return;
      }
    }

    if (expected_type == TYPE_VOID && return_node->childs[0]->type == NODE_NULL) {
      return;
    }

    if (expected_type == TYPE_CHAR && expected_pointers == 1 && return_node->childs[0]->an_array_size >= 1) {
      return;
    }

    if (expected_type == TYPE_VOID && expected_pointers == 1 && return_node->childs[0]->an_n_pointers >= 1) {
      return;
    }

    if (expected_pointers >= 1 && return_node->childs[0]->an_n_pointers == 1 && return_node->childs[0]->an_type == TYPE_VOID) {
      return;
    }

    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", return_node->loc.first_line, return_node->loc.first_column);

    if (return_node->childs[0]->an_type == TYPE_UNKNOWN) {
      printf("undef");
    } else {
      printf("%s", type_str[return_node->childs[0]->an_type]);
    }

    print_asterisks2(return_node->childs[0]->an_n_pointers);
    printf(", expected ");
    printf("%s", type_str[expected_type]);
    print_asterisks2(expected_pointers);
    printf(")\n");
  }
}

void parse_decl(sym_t *st, node_t *decl_node, char *func_name) {
  sym_t *cur_st_node = st;
  sym_t *func_node;
  int duplicate = 0;
  sym_t *new_node = create_variable_node(decl_node);

  func_node = is_function(st, decl_node->childs[1], func_name);

  if (func_node != NULL && func_name == NULL) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", decl_node->loc.first_line, decl_node->loc.first_column);
    print_sym_array(new_node);
    printf(", expected ");
    print_function_type(func_node);
    printf(")\n");
    return;
  }

  if (new_node->type == TYPE_VOID && new_node->n_pointers == 0) {
    semantic_errors = 1;
    printf("Line %d, col %d: Invalid use of void type in declaration\n", decl_node->childs[0]->loc.first_line, decl_node->childs[0]->loc.first_column);
    return;
  }

  if (func_name != NULL) { // variaveis globais podem ser declaradas duas vezes
    while (cur_st_node != NULL) {
      if (!strcmp(cur_st_node->id, func_name)) {
        func_node = cur_st_node;
      }

      cur_st_node = cur_st_node->next;
    }

    cur_st_node = func_node->definition->next;

    while (cur_st_node != NULL) {
      if (cur_st_node->id != NULL && !strcmp(cur_st_node->id, decl_node->childs[decl_node->n_childs - 1]->value)) {
        semantic_errors = 1;
        printf("Line %d, col %d: Symbol %s already defined\n", decl_node->loc.first_line, decl_node->loc.first_column, decl_node->childs[decl_node->n_childs - 1]->value);
        duplicate = 1;
        break;
      }

      cur_st_node = cur_st_node->next;
    }
  } else {
    while (cur_st_node != NULL) {
      if (!strcmp(cur_st_node->id, new_node->id)) { // check if preivous decl was something else
        if (cur_st_node->type != new_node->type || cur_st_node->n_pointers != new_node->n_pointers || cur_st_node->array_size != new_node->array_size) {
          semantic_errors = 1;
          printf("Line %d, col %d: Conflicting types (got %s", decl_node->loc.first_line, decl_node->loc.first_column, type_str[new_node->type]);
          print_asterisks2(new_node->n_pointers);

          if (new_node->array_size >= 1) {
            printf("[%d]", new_node->array_size);
          }

          printf(", expected %s", type_str[cur_st_node->type]);
          print_asterisks2(cur_st_node->n_pointers);

          if (cur_st_node->array_size >= 1) {
            printf("[%d]", cur_st_node->array_size);
          }

          printf(")\n");
        }

        duplicate = 1;
      }

      cur_st_node = cur_st_node->next;
    }
  }

  if (!func_name) {
    if (!duplicate) {
      if (add_to_top(st, new_node) == 1) {
        last = new_node;
      }
    }
  } else {
    if (!duplicate) {
      cur_st_node = st;

      while (cur_st_node != NULL) {
        if (!strcmp(cur_st_node->id, func_name)) {
          func_node = cur_st_node;
        }

        cur_st_node = cur_st_node->next;
      }

      cur_st_node = func_node->definition;
      while (cur_st_node->next != NULL) {
        cur_st_node = cur_st_node->next;
      }

      cur_st_node->next = new_node;
    }
  }
}

void parse_array_decl(sym_t *st, node_t *decl_node, char *func_name) {
  sym_t *cur_st_node = st;
  sym_t *func_node;
  int duplicate = 0;
  sym_t *new_node = create_array_node(decl_node);

  func_node = is_function(st, decl_node->childs[1], func_name);

  if (func_node != NULL && func_name == NULL) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", decl_node->loc.first_line, decl_node->loc.first_column);
    print_sym_array(new_node);
    printf(", expected ");
    print_function_type(func_node);
    printf(")\n");
    return;
  }

  if (decl_node->childs[0]->type == NODE_VOID && new_node->n_pointers == 0) {
    semantic_errors = 1;
    printf("Line %d, col %d: Invalid use of void type in declaration\n", decl_node->loc.first_line, decl_node->loc.first_column);
  }

  if (func_name != NULL) { // variaveis globais podem ser declaradas duas vezes
    while (cur_st_node != NULL) {
      if (!strcmp(cur_st_node->id, func_name)) {
        func_node = cur_st_node;
      }

      cur_st_node = cur_st_node->next;
    }

    cur_st_node = func_node->definition->next;

    while (cur_st_node != NULL) {
      if (cur_st_node->id != NULL && !strcmp(cur_st_node->id, decl_node->childs[decl_node->n_childs - 2]->value)) {
        semantic_errors = 1;
        printf("Line %d, col %d: Symbol %s already defined\n", decl_node->loc.first_line, decl_node->loc.first_column, decl_node->childs[decl_node->n_childs - 2]->value);
        duplicate = 1;
        break;
      }

      cur_st_node = cur_st_node->next;
    }
  } else {
    while (cur_st_node != NULL) {
      if (!strcmp(cur_st_node->id, new_node->id)) { // TODO check if previous decl was actually a function and give error
        if (cur_st_node->array_size != new_node->array_size || cur_st_node->type != new_node->type || cur_st_node->n_pointers != new_node->n_pointers) {
          semantic_errors = 1;
          printf("Line %d, col %d: Conflicting types (got ", decl_node->loc.first_line, decl_node->loc.first_column);
          print_sym_array(new_node);
          printf(", expected ");

          if (is_sym_function(st, cur_st_node)) {
            print_function_type(cur_st_node);
          } else {
            print_sym_array(cur_st_node);
          }

          printf(")\n");
        }

        duplicate = 1;
      }

      cur_st_node = cur_st_node->next;
    }
  }

  if (!func_name) {
    if (!duplicate) {
      if (add_to_top(st, new_node) == 1) {
        last = new_node;
      }
    }
  } else {
    if (!duplicate) {
      cur_st_node = st;

      while (cur_st_node != NULL) {
        if (!strcmp(cur_st_node->id, func_name)) {
          func_node = cur_st_node;
        }

        cur_st_node = cur_st_node->next;
      }

      cur_st_node = func_node->definition;
      while (cur_st_node->next != NULL) {
        cur_st_node = cur_st_node->next;
      }

      cur_st_node->next = new_node;
    }
  }
}

void parse_func_declaration(sym_t *st, node_t *func_decl_node, char *func_name) {
  sym_t *declaration_node = create_declaration_node(func_decl_node);
  node_t* param_list = func_decl_node->childs[declaration_node->n_pointers + 2];
  int to_insert = 1;

  int i;
  for (i = 0; i < param_list->n_childs; i++) {
    node_t* param_declaration = param_list->childs[i];

    sym_t *new_node = create_variable_node(param_declaration);
    new_node->is_parameter = 1;
    declaration_node->params[declaration_node->n_params++] = new_node;

    if (new_node->type == TYPE_VOID && new_node->n_pointers == 0 && (new_node->id != NULL || param_list->n_childs > 1)) {
      semantic_errors = 1;
      printf("Line %d, col %d: Invalid use of void type in declaration\n", param_list->childs[i]->childs[0]->loc.first_line, param_list->childs[i]->childs[0]->loc.first_column);
      to_insert = 0;
      break;
    }

    if (i >= 1) {
      int u;
      for (u = 0; u < i; u++) {
        if (new_node->id != NULL && declaration_node->params[u]->id != NULL) {
          if (!strcmp(new_node->id, declaration_node->params[u]->id)) {
            semantic_errors = 1;
            printf("Line %d, col %d: Symbol %s already defined\n", param_declaration->loc.first_line, param_declaration->loc.first_column, declaration_node->params[i]->id);
            break;
          }
        }
      }
    }
  }

  // if duplicate, we don't put it in symbol table
  sym_t *cur_st_node = st;
  int arg_mismatch = 0;
  int duplicate = 0;

  if (to_insert) {
    while (cur_st_node != NULL) {
      if (!strcmp(cur_st_node->id, declaration_node->id)) {
        if (cur_st_node->node_type != FUNC_DECLARATION) {
          semantic_errors = 1;
          printf("Line %d, col %d: Conflicting types (got ", func_decl_node->loc.first_line, func_decl_node->loc.first_column + declaration_node->n_pointers);
          print_function_type(declaration_node);
          printf(", expected ");
          print_sym_array(cur_st_node);
          printf(")\n");
          return;
        } else {
          // todo iterate each arg and check if it's different in which case give a conflicting type on the argument itself

          if (cur_st_node->type != declaration_node->type || cur_st_node->n_pointers != declaration_node->n_pointers) {
            arg_mismatch = 1;
          }

          if (cur_st_node->n_params != declaration_node->n_params) {
            arg_mismatch = 1;
          }

          int i;

          for (i = 0; i < cur_st_node->n_params && i < declaration_node->n_params; i++) {
            if (cur_st_node->params[i]->type != declaration_node->params[i]->type || cur_st_node->params[i]->n_pointers != declaration_node->params[i]->n_pointers) {
              arg_mismatch = 1;
            }
          }

          duplicate = 1;
        }

        break;
      }

      cur_st_node = cur_st_node->next;
    }
  }

  if (arg_mismatch) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", func_decl_node->loc.first_line, func_decl_node->loc.first_column + declaration_node->n_pointers);
    print_function_type(declaration_node);
    printf(", expected ");
    print_function_type(cur_st_node);
    printf(")\n");
  } else {
    if (!duplicate && to_insert) {
      if (add_to_top(st, declaration_node) == 1) {
        last = declaration_node;
      }
    }
  }
}

int mystrlen(const char *str) {
  int val = 0;
  int i = 0;
  int u;

  while (i < strlen(str)) {
    if (str[i] == '\\') {
      if (str[i + 1] - '0' >= 0 && str[i + 1] - '0' <= 7) {
        int increment_i = 0;
        for (u = 2; u <= 3; u++) {
          if (str[i + u] - '0' >= 0 && str[i + u] - '0' <= 7) {
            increment_i++;
          } else {
            break;
          }
        }

        i += 2 + increment_i;
        val++;
      } else {
        i += 2;
        val++;
      }
    } else {
      val++;
      i += 1;
    }
  }

  return val;
}

int parse_func_definition(sym_t *st, node_t *func_def_node) {
  sym_t *table_node = create_func_table_node(func_def_node);

  sym_t *cur_st_node = st;
  sym_t *declaration_node = NULL;

  int print_conflicting_type = 0;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, table_node->id)) {
      if (cur_st_node->node_type != FUNC_DECLARATION || (cur_st_node->node_type == FUNC_DECLARATION && cur_st_node->definition != NULL)) {
        sym_t *func_node = is_sym_function(st, cur_st_node);

        if (func_node) {
          semantic_errors = 1;
          printf("Line %d, col %d: Symbol %s already defined\n", func_def_node->loc.first_line, func_def_node->childs[table_node->n_pointers + 1]->loc.first_column, cur_st_node->id);
          return 1;
        } else {
          print_conflicting_type = 1;
        }

        break;
      }

      declaration_node = cur_st_node;
    }

    cur_st_node = cur_st_node->next;
  }

  sym_t *temp = last;
  if (declaration_node == NULL) {
    declaration_node = create_node(FUNC_DECLARATION, table_node->id, table_node->type);
    declaration_node->n_pointers = table_node->n_pointers;
    add_to_top(st, declaration_node);
    last = declaration_node;
  }

  int error_given = st_add_definition(st, table_node, func_def_node, declaration_node);

  if (print_conflicting_type) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", func_def_node->loc.first_line, func_def_node->childs[table_node->n_pointers + 1]->loc.first_column);
    print_function_type(declaration_node);
    printf(", expected ");
    print_sym_array(cur_st_node);
    printf(")\n");
    error_given = 1;
  }

  if (error_given) {
    last = temp;
    temp->next = NULL;
  }

  return error_given;
}

void parse_if_node(sym_t *st, node_t *if_node, char *func_name) {
  sym_t *func_node = is_function(st, if_node->childs[0], func_name);

  if (func_node != NULL) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", if_node->childs[0]->loc.first_line, if_node->childs[0]->loc.first_column);
    print_function_type(func_node);
    printf(", expected int)\n");
    return;
  }

  if (if_node->childs[0]->an_type == TYPE_UNDEF) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got undef, expected int)\n", if_node->childs[0]->loc.first_line, if_node->childs[0]->loc.first_column);
  }

  if (if_node->childs[0]->an_type == TYPE_VOID && if_node->childs[0]->an_n_pointers == 0 && if_node->childs[0]->an_array_size < 1) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got void, expected int)\n", if_node->childs[0]->loc.first_line, if_node->childs[0]->loc.first_column);
  }
}

void parse_for_node(sym_t *st, node_t *for_node, char *func_name) {
  sym_t *func_node = is_function(st, for_node->childs[1], func_name);

  if (func_node != NULL) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got ", for_node->childs[1]->loc.first_line, for_node->childs[1]->loc.first_column);
    print_function_type(func_node);
    printf(", expected int)\n");
    return;
  }

  if (for_node->childs[1]->an_type == TYPE_UNDEF) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got undef, expected int)\n", for_node->childs[1]->loc.first_line, for_node->childs[1]->loc.first_column);
    return;
  }

  if (for_node->childs[1]->an_type == TYPE_VOID && for_node->childs[1]->an_n_pointers == 0 && for_node->childs[1]->an_array_size < 1) {
    semantic_errors = 1;
    printf("Line %d, col %d: Conflicting types (got void, expected int)\n", for_node->childs[1]->loc.first_line, for_node->childs[1]->loc.first_column);
  }
}

void an_tree(node_t *where, sym_t *st, char *func_name, int an, int bad) {
  if (where->type == NODE_ARRAYDECLARATION || where->type == NODE_FUNCDECLARATION ||
      where->type == NODE_DECLARATION) {
    an = 0;
  }

  int i;
  if (where->type == NODE_FUNCDEFINITION) {
    bad = parse_func_definition(st, where);

    func_name = (char *) strdup(get_function_name(where));

    int i;

    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->type == NODE_FUNCBODY) {
        an_tree(where->childs[i], st, func_name, an, bad);
      }
    }
  } else {
    for (i = 0; i < where->n_childs; i++) {
      an_tree(where->childs[i], st, func_name, an, bad);
    }
  }

  if (where->type == NODE_RETURN) {
    parse_return_node(st, where, func_name);
  } else if (where->type == NODE_DECLARATION) {
    if (!bad) {
      parse_decl(st, where, func_name);
    }
  } else if (where->type == NODE_FUNCDECLARATION) {
    parse_func_declaration(st, where, func_name);
  } else if (where->type == NODE_ARRAYDECLARATION) {
    if (!bad) {
      parse_array_decl(st, where, func_name);
    }
  } else if (where->type == NODE_FUNCDEFINITION) {

  } else if (where->type == NODE_INTLIT || where->type == NODE_CHRLIT) {
    if (an) {
      where->an_type = TYPE_INT;
    }
  } else if (where->type == NODE_STRLIT) {
    if (an) {
      where->an_type = TYPE_CHAR;
      where->an_array_size = mystrlen(where->value) - 2 + 1; // remove the two quotes ("") and add the null byte
    }
  } else if (where->type == NODE_ID) {
    parse_id_node(st, where, func_name, an);
  } else if (where->type == NODE_ADD) {
    parse_add_node(st, where, func_name);
  } else if (where->type == NODE_SUB) {
    parse_sub_node(st, where, func_name);
  } else if (where->type == NODE_COMMA) { // comma gets last child's value
    parse_comma_node(st, where, func_name);
  } else if (where->type == NODE_MUL || where->type == NODE_DIV || where->type == NODE_MOD) {
    parse_mul_div_mod_node(st, where, func_name);
  } else if (where->type == NODE_EQ || where->type == NODE_NE) {
    parse_comp_node(st, where, func_name);
  } else if (where->type == NODE_AND || where->type == NODE_OR) {
    parse_comp_node2(st, where, func_name);
  } else if (where->type == NODE_GE || where->type == NODE_GT || where->type == NODE_LE || where->type == NODE_LT) {
    parse_comp_node3(st, where, func_name);
  } else if (where->type == NODE_MINUS || where->type == NODE_PLUS) {
    parse_minus_plus_node(st, where, func_name);
  } else if (where->type == NODE_NOT) {
    parse_not_node(st, where, func_name);
  } else if (where->type == NODE_ADDR) {
    parse_addr_node(st, where, func_name);
  } else if (where->type == NODE_STORE) {
    parse_store_node(st, where, func_name);
  } else if (where->type == NODE_CALL) {
    parse_call_node(st, where, an, func_name);
  } else if (where->type == NODE_DEREF) {
    parse_deref_node(st, where, func_name);
  } else if (where->type == NODE_IF) {
    parse_if_node(st, where, func_name);
  } else if (where->type == NODE_FOR) {
    parse_for_node(st, where, func_name);
  }

  if (where->type == NODE_FUNCDEFINITION || where->type == NODE_PROGRAM || where->type == NODE_FUNCBODY ||
      where->type == NODE_FOR || where->type == NODE_RETURN || where->type == NODE_STATLIST || where->type == NODE_IF ||
      where->type == NODE_NULL) {
    return;
  }

  if (where->an_type == TYPE_UNKNOWN && an) {
    where->an_type = TYPE_UNDEF;
  }
}
