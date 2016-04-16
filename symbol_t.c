#include "symbol_t.h"

type_t node_type_to_sym_type(nodetype_t type) {
  if (type == NODE_INT) {
    return TYPE_INT;
  }

  return TYPE_UNKNOWN;
}

sym_t *create_node(table_type_t node_type, char *name, type_t type) {
  sym_t* node = (sym_t*) malloc(sizeof(sym_t));

  memset(node, 0, sizeof(sym_t));

  node->type = type;
  node->is_parameter = 0;
  node->id = name;
  node->next = NULL;
  node->table_method = NULL;
  node->node_type = node_type;
  node->method_start = NULL;

  return node;
}

sym_t* st_analyze_ast(node_t *root) {
  if (!root) { return NULL; }

  int cur = 0;
  node_t* cur_node = root->childs[cur];
  sym_t* st = create_node(GLOBAL, "Global", TYPE_UNKNOWN);
  sym_t* last = st;

  while (cur < root->n_childs) {
    sym_t* new_node;

    if (cur_node->type == NODE_DECLARATION) {
      new_node = create_node(VARIABLE, cur_node->childs[1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
    }

    last->next = new_node;
    last = new_node;
    cur_node = root->childs[++cur];
  }

  return st;
}

void st_print_table_element(sym_t* element) {
  if (element->node_type == GLOBAL) {
    printf("===== Global Symbol Table =====\n");
    printf("atoi\tint(char*)\n");
    printf("itoa\tchar*(int,char*)\n");
    printf("puts\tint(char*)\n");
  } else if (element->node_type == VARIABLE) {
    printf("%s\t%s\n", element->id, type_str[element->type]);
  }
}

void st_print_table(sym_t* table) {
  sym_t* cur_node = table;

  while (cur_node != NULL) {
    st_print_table_element(cur_node);
    cur_node = cur_node->next;
  }
}