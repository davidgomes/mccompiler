//gcc -o ast_tests ast_tests.c ast.c
#include "ast.h"

int main(){
  node_t* ast;

  node_t* aux_int = ast_insert_node(NODE_INT, 1, 0);
  node_t* aux_id = ast_insert_node(NODE_ID, 1, 0);
  aux_id->value = "hello";

  node_t* aux_decl = ast_insert_node(NODE_DECLARATION, 1, 2, aux_int, aux_id);

  ast = ast_insert_node(NODE_PROGRAM, 1, 1, aux_decl);
  ast_print_tree(ast, 0);
  return 0;
}
