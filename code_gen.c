#include "code_gen.h"

void code_gen_program(node_t *program_node) {
  printf("@.str = private unnamed_addr constant [13 x i8] c\"hello world\\0A\\00\"\n");

  printf("declare i32 @puts(i8* nocapture) nounwind\n");

  printf("define i32 @main() {\n");
    printf("%%cast210 = getelementptr [13 x i8]* @.str, i64 0, i64 0\n");
    printf("call i32 @puts(i8* %%cast210)\n");
    printf("ret i32 0\n");
  printf("}\n");
}

void code_gen(node_t *which) {
  if (which->type == NODE_PROGRAM) {
    code_gen_program(which);
  }
}