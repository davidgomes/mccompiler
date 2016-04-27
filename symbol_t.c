#include "symbol_t.h"

char *node_type_to_str[] = {
  "global",
  "func_table",
  "func_declaration",
  "array",
  "variable",
  "return_node"
};

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
  node->definition = NULL;
  node->defined = 0;
  node->params = (sym_t**) malloc(1000 * sizeof(sym_t*));

  int i;
  for (i = 0; i < 100; i++) {
    node->params[i] = NULL;
  }

  node->node_type = node_type;

  return node;
}

sym_t* create_variable_node(node_t *cur_node) {
  int n_pointers = 0;
  int cur_pointer = 1;
  sym_t *new_node;

  while (cur_pointer < cur_node->n_childs && cur_node->childs[cur_pointer]->type == NODE_POINTER) {
    n_pointers++;
    cur_pointer++;
  }

  if (n_pointers + 1 >= cur_node->n_childs) {
    new_node = create_node(VARIABLE, NULL, node_type_to_sym_type(cur_node->childs[0]->type));
  } else {
    new_node = create_node(VARIABLE, cur_node->childs[n_pointers + 1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
  }

  new_node->n_pointers = n_pointers;

  return new_node;
}

int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

int octal_decimal(int n) /* Function to convert octal to decimal */
{
    int decimal=0, i=0, rem;
    while (n!=0)
    {
        rem = n%10;
        n/=10;
        decimal += rem*ipow(8,i);
        ++i;
    }
    return decimal;
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

  if (cur_node->childs[n_pointers + 2]->value[0] == '0') {
    new_node->array_size = octal_decimal(new_node->array_size);
  }

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

void st_add_definition(sym_t *st, sym_t *table_node, node_t *cur_node, sym_t *declaration_node) {
  sym_t *new_node, *last_node;

  declaration_node->definition = table_node;
  last_node = table_node;

  new_node = create_node(RETURN_NODE, NULL, node_type_to_sym_type(cur_node->childs[0]->type));
  new_node->n_pointers = table_node->n_pointers;
  declaration_node->definition->next = new_node;
  last_node = new_node;

  node_t* param_list = cur_node->childs[new_node->n_pointers + 2];

  int declaration_node_was_defined = declaration_node->n_params > 0;

  // TODO Ao ler os parâmetros, detetar parâmetros duplicados e parâmetros que não estejam
  // de acordo com a prévia declaração
  int i;
  for (i = 0; i < param_list->n_childs; i++) {
    node_t* param_declaration = param_list->childs[i];

    if (param_declaration->n_childs == 1) { // int main(void) { for instance
      new_node = create_node(VARIABLE, NULL, TYPE_VOID);
      new_node->is_parameter = 1;

      if (declaration_node_was_defined == 0) {
        declaration_node->params[declaration_node->n_params++] = new_node;
      }

      break;
    }

    new_node = create_variable_node(param_declaration);
    new_node->is_parameter = 1;

    if (declaration_node_was_defined == 0) {
      declaration_node->params[declaration_node->n_params++] = new_node;
    }

    last_node->next = new_node;
    last_node = new_node;
  }
}

int add_to_top(sym_t *st, sym_t *node) { // returns 1 if last has to be changed
  if (st->next == NULL) {
    st->next = node;
    return 1;
  }

  sym_t *cur_st_node = st;

  while (cur_st_node->next != NULL) {
    /*if (!strcmp(cur_st_node->id, node->id)) {
      return 0;
      }*/

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

void init_st() {
  st = create_node(GLOBAL, "Global", TYPE_UNKNOWN);
  last = st;

  last->next = create_node(FUNC_DECLARATION, "atoi", TYPE_INT);
  last->next->defined = 1;
  last->next->n_params = 1;
  last->next->params[0] = create_node(VARIABLE, NULL, TYPE_CHAR);
  last->next->params[0]->n_pointers = 1;
  last = last->next;

  last->next = create_node(FUNC_DECLARATION, "itoa", TYPE_CHAR);
  last->next->defined = 1;
  last->next->n_pointers = 1;
  last->next->n_params = 2;
  last->next->params[0] = create_node(VARIABLE, NULL, TYPE_INT);
  last->next->params[1] = create_node(VARIABLE, NULL, TYPE_CHAR);
  last->next->params[1]->n_pointers = 1;
  last = last->next;

  last->next = create_node(FUNC_DECLARATION, "puts", TYPE_INT);
  last->next->defined = 1;
  last->next->n_params = 1;
  last->next->params[0] = create_node(VARIABLE, NULL, TYPE_CHAR);
  last->next->params[0]->n_pointers = 1;
  last = last->next;
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
  sym_t* cur_st_node = table;

  while (cur_st_node != NULL) {
    st_print_table_element(cur_st_node);
    cur_st_node = cur_st_node->next;
  }

  // print function tables
  cur_st_node = table->next;
  while (cur_st_node != NULL) {
    if (cur_st_node->definition != NULL) {
      st_print_table_element(cur_st_node->definition);

      sym_t *cur_st_node2 = cur_st_node->definition->next;

      while (cur_st_node2 != NULL) {
        st_print_table_element(cur_st_node2);
        cur_st_node2 = cur_st_node2->next;
      }
    }

    cur_st_node = cur_st_node->next;
  }
}
