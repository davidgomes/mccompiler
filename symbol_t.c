#include "symbol_t.h"

sym_t *create_node(table_type_t node_type, char *name, type_t type) {
  sym_t* node = (sym_t*) malloc(sizeof(sym_t));

  memset(node, 0, sizeof(sym_t));

  node->type = type;
  node->n_pointers = 0;
  node->is_parameter = 0;
  node->id = name;
  node->array_size = -1;
  node->next = NULL;
  node->n_params = 0;
  node->params = (sym_t**) malloc(sizeof(sym_t*));
  node->node_type = node_type;

  return node;
}

sym_t* create_variable_node(node_t *cur_node) {
  int n_pointers = 0;

  int cur_pointer = 1;
  while (cur_node->childs[cur_pointer]->type == NODE_POINTER) {
    n_pointers++;
    cur_pointer++;
  }

  sym_t *new_node = create_node(VARIABLE, cur_node->childs[n_pointers + 1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
  new_node->n_pointers = n_pointers;

  return new_node;
}

sym_t* create_array_node(node_t *cur_node) {
  int n_pointers = 0;

  int cur_pointer = 1;
  while (cur_node->childs[cur_pointer]->type == NODE_POINTER) {
    n_pointers++;
    cur_pointer++;
  }

  sym_t *new_node = create_node(ARRAY, cur_node->childs[n_pointers + 1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
  new_node->n_pointers = n_pointers;
  new_node->array_size = atoi(cur_node->childs[n_pointers + 2]->value);
  return new_node;
}

sym_t* create_declaration_node(node_t *cur_node) {
  int n_pointers = 0;

  int cur_pointer = 1;
  while (cur_node->childs[cur_pointer]->type == NODE_POINTER) {
    n_pointers++;
    cur_pointer++;
  }

  sym_t *new_node = create_node(FUNC_DECLARATION, cur_node->childs[n_pointers + 1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
  new_node->n_pointers = n_pointers;
  return new_node;
}

sym_t* create_func_table_node(node_t *cur_node) {
  int n_pointers = 0;

  int cur_pointer = 1;
  while (cur_node->childs[cur_pointer]->type == NODE_POINTER) {
    n_pointers++;
    cur_pointer++;
  }

  sym_t *new_node = create_node(FUNC_TABLE, cur_node->childs[n_pointers + 1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
  new_node->n_pointers = n_pointers;
  return new_node;
}

void st_add_declaration_to_top(sym_t *st, sym_t *definition, node_t *param_list) {
  sym_t *cur_st_node = st;

  while (1) {
    if (cur_st_node->next->node_type == FUNC_TABLE) {
      sym_t *tmp = cur_st_node->next;

      sym_t *declaration_node = create_node(FUNC_DECLARATION, definition->id, definition->type);
      declaration_node->n_pointers = definition->n_pointers;

      int i;
      for (i = 0; i < param_list->n_childs; i++) {
        node_t* param_declaration = param_list->childs[i];

        if (param_declaration->n_childs == 1) { // int main(void); for instance
          break;
        }

        sym_t *new_node = create_variable_node(param_declaration);
        new_node->is_parameter = 1;
        declaration_node->params[declaration_node->n_params++] = new_node;
      }

      cur_st_node->next = declaration_node;
      declaration_node->next = tmp;

      break;
    }

    cur_st_node = cur_st_node->next;
  }
}

int add_to_top(sym_t *st, sym_t *node) { // returns 1 if last has to be changed
  if (st->next == NULL) {
    st->next = node;
    return 1;
  }

  sym_t *cur_st_node = st;

  while (cur_st_node->next != NULL) {
    if (cur_st_node->next->node_type == FUNC_TABLE) {
      sym_t *tmp = cur_st_node->next;
      cur_st_node->next = node;
      node->next = tmp;
      return 0;
    }

    cur_st_node = cur_st_node->next;
  }

  cur_st_node->next = node;
  return 1;
}

sym_t* st_analyze_ast(node_t *root) {
  if (!root) { return NULL; }

  int cur = 0;
  node_t* cur_node = root->childs[cur];
  sym_t* st = create_node(GLOBAL, "Global", TYPE_UNKNOWN);
  sym_t* last = st;

  last->next = create_node(FUNC_DECLARATION, "atoi", TYPE_INT);
  last->next->n_params = 1;
  last->next->params[0] = create_node(VARIABLE, NULL, TYPE_CHAR);
  last->next->params[0]->n_pointers = 1;
  last = last->next;

  last->next = create_node(FUNC_DECLARATION, "itoa", TYPE_CHAR);
  last->next->n_pointers = 1;
  last->next->n_params = 2;
  last->next->params[0] = create_node(VARIABLE, NULL, TYPE_INT);
  last->next->params[1] = create_node(VARIABLE, NULL, TYPE_CHAR);
  last->next->params[1]->n_pointers = 1;
  last = last->next;

  last->next = create_node(FUNC_DECLARATION, "puts", TYPE_INT);
  last->next->n_params = 1;
  last->next->params[0] = create_node(VARIABLE, NULL, TYPE_CHAR);
  last->next->params[0]->n_pointers = 1;
  last = last->next;

  while (cur < root->n_childs) {
    if (cur_node->type == NODE_DECLARATION) {
      sym_t *new_node = create_variable_node(cur_node);
      if (add_to_top(st, new_node) == 1) last = new_node;
    } else if (cur_node->type == NODE_ARRAYDECLARATION) {
      sym_t *new_node = create_array_node(cur_node);
      if (add_to_top(st, new_node) == 1) last = new_node;
    } else if (cur_node->type == NODE_FUNCDECLARATION) {
      sym_t *declaration_node = create_declaration_node(cur_node);

      node_t* param_list = cur_node->childs[declaration_node->n_pointers + 2];

      int i;
      for (i = 0; i < param_list->n_childs; i++) {
        node_t* param_declaration = param_list->childs[i];

        if (param_declaration->n_childs == 1) { // int main(void); for instance
          break;
        }

        sym_t *new_node = create_variable_node(param_declaration);
        new_node->is_parameter = 1;
        last->params[last->n_params++] = new_node;
      }

      if (add_to_top(st, declaration_node) == 1) last = declaration_node;
    } else if (cur_node->type == NODE_FUNCDEFINITION) {
      sym_t *new_node = create_func_table_node(cur_node);
      sym_t *definition_node = new_node;
      last->next = new_node;
      last = new_node;

      new_node = create_node(RETURN_NODE, NULL, node_type_to_sym_type(cur_node->childs[0]->type));
      new_node->n_pointers = definition_node->n_pointers;
      last->next = new_node;
      last = new_node;

      node_t* param_list = cur_node->childs[new_node->n_pointers + 2];

      int i;
      for (i = 0; i < param_list->n_childs; i++) {
        node_t* param_declaration = param_list->childs[i];

        if (param_declaration->n_childs == 1) { // int main(void) { for instance
          break;
        }

        new_node = create_variable_node(param_declaration);
        new_node->is_parameter = 1;
        last->next = new_node;
        last = new_node;
      }

      node_t *func_body = cur_node->childs[cur_node->n_childs - 1];

      for (int i = 0; i < func_body->n_childs; i++) {
        if (func_body->childs[i]->type == NODE_RETURN) break;

        node_t *func_body_decl = func_body->childs[i];

        if (func_body_decl->type == NODE_DECLARATION) {
          new_node = create_variable_node(func_body_decl);

          last->next = new_node;
          last = new_node;
        } else if (func_body_decl->type == NODE_ARRAYDECLARATION) {
          new_node = create_array_node(func_body_decl);

          last->next = new_node;
          last = new_node;
        } else {
          break;
        }
      }

      st_add_declaration_to_top(st, definition_node, param_list);
    }

    cur++;
    cur_node = root->childs[cur];
  }

  return st;
}

void print_asterisks(int n_pointers) {
  int i;
  for (i = 0; i < n_pointers; i++) {
    printf("*");
  }
}

void st_print_table_element(sym_t* element) {
  if (element->node_type == GLOBAL) {
    printf("===== Global Symbol Table =====\n");
  } else if (element->node_type == VARIABLE) {
    if (element->is_parameter) {
      if (element->n_pointers == 0) {
        printf("%s\t%s\tparam\n", element->id, type_str[element->type]);
      } else {
        printf("%s\t%s", element->id, type_str[element->type]);
        print_asterisks(element->n_pointers);
        printf("\tparam\n");
      }
    } else {
      if (element->n_pointers == 0) {
        printf("%s\t%s\n", element->id, type_str[element->type]);
      } else {
        printf("%s\t%s", element->id, type_str[element->type]);
        print_asterisks(element->n_pointers);
        printf("\n");
      }
    }
  } else if (element->node_type == ARRAY) {
    if (element->n_pointers == 0) {
      printf("%s\t%s[%d]\n", element->id, type_str[element->type], element->array_size);
    } else {
      printf("%s\t%s", element->id, type_str[element->type]);
      print_asterisks(element->n_pointers);
      printf("[%d]\n", element->array_size);
    }
  } else if (element->node_type == FUNC_DECLARATION) {
    if (element->n_pointers == 0) {
      printf("%s\t%s(", element->id, type_str[element->type]);
    } else {
      printf("%s\t%s", element->id, type_str[element->type]);
      print_asterisks(element->n_pointers);
      printf("(");
    }

    int i;
    for (i = 0; i < element->n_params; i++) {
      sym_t *arg = element->params[i];

      if (arg->n_pointers == 0) {
        printf("%s", type_str[arg->type]);
      } else {
        printf("%s", type_str[arg->type]);
        print_asterisks(arg->n_pointers);
      }

      if (i != element->n_params - 1) {
        printf(",");
      }
    }

    printf(")\n");
  } else if (element->node_type == FUNC_TABLE) {
    printf("\n===== Function %s Symbol Table =====\n", element->id);
  } else if (element->node_type == RETURN_NODE) {
    printf("return\t%s", type_str[element->type]);
    print_asterisks(element->n_pointers);
    printf("\n");
  }
}

void st_print_table(sym_t* table) {
  sym_t* cur_node = table;

  while (cur_node != NULL) {
    st_print_table_element(cur_node);
    cur_node = cur_node->next;
  }
}