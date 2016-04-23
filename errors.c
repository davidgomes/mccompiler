#include "errors.h"

void print_asterisks3(int n_pointers) {
  int i;

  for (i = 0; i < n_pointers; i++) {
    printf("*");
  }
}

void print_node(node_t *which) {
  printf("%s", type_str[which->an_type]);
  print_asterisks3(which->an_n_pointers);
}

void operator_applied2(nodetype_t operator, node_t *node1, node_t *node2) {
  printf("Operator %s cannot be applied to types ", node_types[operator]);
  print_node(node1);
  printf(", ");
  print_node(node2);
  printf("\n");
}