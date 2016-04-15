#include "symbol_t.h"

sym_t* st_analyze_ast(node_t *root) {
  printf("%s\n", node_types[root->type]);

  node_t* cur_node = root->childs[0];

  while (cur_node != 0) {
    if (cur_node->type == NODE_DECLARATION) {
      //cur_node = cur_node->
    }
  }

  return NULL;
}

void st_print_table_element(sym_t* element) {

}

void st_print_table(sym_t* table) {
  //printf("%s\n", node_types[root->type]);
}