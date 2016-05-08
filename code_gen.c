#include "code_gen.h"

void code_gen_program(node_t *program_node) {
  printf("declare i32 @puts(i8* nocapture) nounwind\n");

  int i;
  for (i = 0; i < program_node->n_childs; i++) {
    code_gen(program_node->childs[i]);
  }
}

void code_gen_func_definition(node_t *func_def_node) {
  printf("define i32 @main() {\n");

  int i;
  for (i = 0; i < func_def_node->n_childs; i++) {
    code_gen(func_def_node->childs[i]);
  }

  printf("}\n");
}

void code_gen_return(node_t *return_node) {
  printf("ret i32 %s\n", return_node->childs[0]->value);
}

void code_gen(node_t *which) {
  if (which->type == NODE_PROGRAM) {
    code_gen_program(which);
  } else if (which->type == NODE_FUNCDEFINITION) {
    code_gen_func_definition(which);
  } else if (which->type == NODE_RETURN) {
    code_gen_return(which);
  } else if (which->type == NODE_FUNCBODY) {
    int i;
    for (i = 0; i < which->n_childs; i++) {
      code_gen(which->childs[i]);
    }
  }
}