#include "semantics.h"

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
  print_asterisks2(which->an_n_pointers);
}

void operator_applied2(node_t *operator, node_t *node1, node_t *node2) {
  printf("Line %d, col %d: Operator %s cannot be applied to types ", operator->loc.first_line, operator->loc.first_column, node_types[operator->type]);
  print_node(node1);
  printf(", ");
  print_node(node2);
  printf("\n");
}

void conflicting_types(node_t *node1, node_t *node2) {
  printf("Line %d, col %d: Conflicting types (got ", node1->loc.first_line, node2->loc.first_column);
  print_node(node1);
  printf(", expected ");
  print_node(node2);
  printf(")\n");
}

void parse_id_node(sym_t *st, node_t *node_id, char* func_name, int an) { // ler também nas declarations para chamada de funcoes nao definidas, no fim.
  if (!an) { // todo: check for duplicate but not annotate
    return;
  }

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
        operator_applied2(add_node, add_node->childs[0], add_node->childs[1]);
      }
  } else if (add_node->childs[0]->an_type == add_node->childs[1]->an_type) {
    add_node->an_type = add_node->childs[0]->an_type;
  }
}

void parse_store_node(sym_t *st, node_t *store_node) {
  if (store_node->childs[1]->an_type == TYPE_VOID) {
    conflicting_types(store_node->childs[1], store_node->childs[0]);
  }

  store_node->an_type = store_node->childs[0]->an_type;
  store_node->an_n_pointers = store_node->childs[0]->an_n_pointers;
  store_node->an_array_size = store_node->childs[0]->an_array_size;
}

void parse_call_node(sym_t *st, node_t *call_node, int an) {
  if (an) {
    set_function_type(st, call_node);
  }

  int args_sent_in = call_node->n_childs - 1;
  int expected_args;

  sym_t *cur_st_node = st;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, call_node->childs[0]->value)) {
      expected_args = cur_st_node->n_params;
      break;
    }

    cur_st_node = cur_st_node->next;
  }

  if (cur_st_node == NULL) {
    printf("Line %d, col %d: Symbol %s is not a function\n", call_node->loc.first_line, call_node->loc.first_column, call_node->childs[0]->value);
    return;
  }

  if (args_sent_in != expected_args) {
    if (cur_st_node->params[0]->type == TYPE_VOID) {
      return;
    }

    printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n", call_node->loc.first_line, call_node->loc.first_column, call_node->childs[0]->value, args_sent_in, expected_args);
  }
}

void parse_return_node(sym_t *st, node_t *return_node, char *func_name) {
  sym_t *cur_st_node = st;
  type_t expected_type;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, func_name)) {
      expected_type = cur_st_node->type;
    }

    cur_st_node = cur_st_node->next;
  }

  if (expected_type == TYPE_VOID && return_node->childs[0]->type != NODE_NULL) {
    printf("Line %d, col %d: Invalid use of void type in declaration\n", return_node->loc.first_line, return_node->loc.first_column);
    return;
  }

  if (expected_type != return_node->childs[0]->an_type) { // todo check that return int and return char is same thing
    printf("Line %d, col %d: Conflicting types (got %s, expected ", return_node->loc.first_line, return_node->loc.first_column, type_str[return_node->childs[0]->an_type]);
    printf("%s", type_str[expected_type]);
    printf(")\n");
  }
}

void parse_decl(sym_t *st, node_t *decl_node, char *func_name) {
  sym_t *cur_st_node = st;
  sym_t *func_node;

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
        printf("Line %d, col %d: Symbol %s already defined\n", decl_node->loc.first_line, decl_node->loc.first_column, decl_node->childs[decl_node->n_childs - 1]->value);

        break;
      }

      cur_st_node = cur_st_node->next;
    }
  }

  int n_pointers;
  if (!func_name) {
    sym_t *new_node = create_variable_node(decl_node);
    n_pointers = new_node->n_pointers;

    if (add_to_top(st, new_node) == 1) {
      last = new_node;
    }
  } else {
    sym_t *new_node = create_variable_node(decl_node);
    n_pointers = new_node->n_pointers;

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

  if (decl_node->childs[0]->type == NODE_VOID && n_pointers == 0) {
    printf("Line %d, col %d: Invalid use of void type in declaration\n", decl_node->loc.first_line, decl_node->loc.first_column);
  }
}

void parse_array_decl(sym_t *st, node_t *decl_node, char *func_name) {
  sym_t *cur_st_node = st;
  sym_t *func_node;

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
        printf("Line %d, col %d: Symbol %s already defined\n", decl_node->loc.first_line, decl_node->loc.first_column, decl_node->childs[decl_node->n_childs - 2]->value);

        break;
      }

      cur_st_node = cur_st_node->next;
    }
  }

  int n_pointers;
  if (!func_name) {
    sym_t *new_node = create_array_node(decl_node);
    n_pointers = new_node->n_pointers;

    if (add_to_top(st, new_node) == 1) {
      last = new_node;
    }
  } else {
    sym_t *new_node = create_array_node(decl_node);
    n_pointers = new_node->n_pointers;

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

  if (decl_node->childs[0]->type == NODE_VOID && n_pointers == 0) {
    printf("Line %d, col %d: Invalid use of void type in declaration\n", decl_node->loc.first_line, decl_node->loc.first_column);
  }
}

void parse_func_declaration(sym_t *st, node_t *func_decl_node, char *func_name) {
  sym_t *declaration_node = create_declaration_node(func_decl_node);

  node_t* param_list = func_decl_node->childs[declaration_node->n_pointers + 2];

  int i;
  for (i = 0; i < param_list->n_childs; i++) {
    node_t* param_declaration = param_list->childs[i];

    sym_t *new_node = create_variable_node(param_declaration);
    new_node->is_parameter = 1;
    declaration_node->params[declaration_node->n_params++] = new_node;
  }

  if (add_to_top(st, declaration_node) == 1) {
    last = declaration_node;
  }
}

void parse_func_definition(sym_t *st, node_t *func_def_node) {
  sym_t *table_node = create_func_table_node(func_def_node);

  sym_t *cur_st_node = st->next->next->next->next; // point to "puts->next"
  sym_t *declaration_node = NULL;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, table_node->id)) {
      declaration_node = cur_st_node;
    }

    cur_st_node = cur_st_node->next;
  }

  if (declaration_node == NULL) {
    declaration_node = create_node(FUNC_DECLARATION, table_node->id, table_node->type);
    declaration_node->n_pointers = table_node->n_pointers;
    add_to_top(st, declaration_node);
    last = declaration_node;
  }

  st_add_definition(st, table_node, func_def_node, declaration_node);
}

void an_tree(node_t *where, sym_t *st, char *func_name, int an) {
  if (where->type == NODE_ARRAYDECLARATION || where->type == NODE_FUNCDECLARATION ||
      where->type == NODE_DECLARATION) {
    an = 0;
  }

  int i;
  if (where->type == NODE_FUNCDEFINITION) {
    parse_func_definition(st, where);

    func_name = (char *) strdup(get_function_name(where));

    int i;

    for (i = 0; i < where->n_childs; i++) {
      if (where->childs[i]->type == NODE_FUNCBODY) {
        an_tree(where->childs[i], st, func_name, an);
      }
    }
  } else {
    for (i = 0; i < where->n_childs; i++) {
      an_tree(where->childs[i], st, func_name, an);
    }
  }

  if (where->type == NODE_RETURN) {
    parse_return_node(st, where, func_name);
  } else if (where->type == NODE_DECLARATION) {
    parse_decl(st, where, func_name);
  } else if (where->type == NODE_FUNCDECLARATION) {
    parse_func_declaration(st, where, func_name);
  } else if (where->type == NODE_ARRAYDECLARATION) {
    parse_array_decl(st, where, func_name);
  } else if (where->type == NODE_FUNCDEFINITION) {

  } else if (where->type == NODE_INTLIT || where->type == NODE_CHRLIT) {
    if (an) {
      where->an_type = node_type_to_sym_type(where->type);
    }
  } else if (where->type == NODE_STRLIT) {
    if (an) {
      where->an_type = TYPE_CHAR;
      where->an_array_size = strlen(where->value) - 2 + 1; // remove the two quotes ("") and add the null byte
    }
  } else if (where->type == NODE_ID) {
    parse_id_node(st, where, func_name, an);
  } else if (where->type == NODE_ADD) {
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
    parse_store_node(st, where);
  } else if (where->type == NODE_CALL) {
    parse_call_node(st, where, an);
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

  if (where->an_type == TYPE_UNKNOWN && an) {
    where->an_type = TYPE_UNDEF;
  }
}