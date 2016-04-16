#include "symbol_t.h"

type_t node_type_to_sym_type(nodetype_t type) {
  if (type == NODE_INT) {
    return TYPE_INT;
  } else if (type == NODE_CHAR) {
    return TYPE_CHAR;
  }

  return TYPE_UNKNOWN;
}

sym_t *create_node(table_type_t node_type, char *name, type_t type) {
  sym_t* node = (sym_t*) malloc(sizeof(sym_t));

  memset(node, 0, sizeof(sym_t));

  node->type = type;
  node->is_parameter = 0;
  node->id = name;
  node->array_size = -1;
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
    } else if (cur_node->type == NODE_ARRAYDECLARATION) {
      new_node = create_node(ARRAY, cur_node->childs[1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
      new_node->array_size = atoi(cur_node->childs[2]->value);
    } else if (cur_node->type == NODE_FUNCDECLARATION) {

    } else if (cur_node->type == NODE_FUNCDEFINITION) {
      new_node = create_node(FUNC_TABLE, cur_node->childs[1]->value, node_type_to_sym_type(cur_node->childs[0]->type));
      last->next = new_node;
      last = new_node;

      new_node = create_node(RETURN_NODE, NULL, node_type_to_sym_type(cur_node->childs[0]->type));

      // todo paramlist
      node_t *param_list = cur_node->childs[2];

      node_t *func_body = cur_node->childs[cur_node->n_childs - 1];
      // todo body
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
  } else if (element->node_type == ARRAY) {
    printf("%s\t%s[%d]\n", element->id, type_str[element->type], element->array_size);
  } else if (element->node_type == FUNC_DEFINITION) {

  } else if (element->node_type == FUNC_TABLE) {
    printf("\n===== Function %s Symbol Table =====\n", element->id);
  } else if (element->node_type == RETURN_NODE) {
    printf("return\t%s\n", type_str[element->type]);
  }
}

void st_print_table(sym_t* table) {
  sym_t* cur_node = table;

  while (cur_node != NULL) {
    st_print_table_element(cur_node);
    cur_node = cur_node->next;
  }
}