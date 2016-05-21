#include "code_gen.h"

int current_str_id = 0;

char* type2llvm(type_t type) {
  if (type == TYPE_INT) {
    return "i32";
  } else if (type == TYPE_CHAR) {
    return "i32";
  } else {
    return "undefined";
  }
}

void find_and_save_strings(node_t *which) {
  if (which->type == NODE_STRLIT) {
    // remove first character of ->value (the opening quote)
    which->value = which->value++;
    // remove last character of ->value (the closing quote) and put in a null byte
    which->value[strlen(which->value) - 1] = 0;

    printf("@.str.%d = private unnamed_addr constant [%d x i8] c%s\\00\"\n", current_str_id, (int) strlen(which->value), which->value);
    which->str_id = current_str_id;
    current_str_id++;
  }

  int i;
  for (i = 0; i < which->n_childs; i++) {
    find_and_save_strings(which->childs[i]);
  }
}

void code_gen_program(node_t *program_node) {
  printf("declare i32 @puts(i8* nocapture) nounwind\n");

  find_and_save_strings(program_node);

  int i;
  for (i = 0; i < program_node->n_childs; i++) {
    code_gen(program_node->childs[i]);
  }
}

void code_gen_func_definition(node_t *func_def_node) {
  sym_t *table_node = create_func_table_node(func_def_node);

  printf("define %s @%s() {\n", type2llvm(table_node->type), table_node->id);

  int i;
  for (i = 0; i < func_def_node->n_childs; i++) {
    printf("  ");
    code_gen(func_def_node->childs[i]);
  }

  printf("}\n");
}

void code_gen_call(node_t *call_node) {
  //%cast210 = getelementptr [13 x i8]* @.str, i64 0, i64 0

  int i;
  for (i = 1; i < call_node->n_childs)

  printf("%%cast210 = getelementptr [13 x i8]* @.str, i64 0, i64 0");

  //call i32 @puts(i8* %cast210)
}

void code_gen_return(node_t *return_node) {
  printf("ret i32 %s\n", return_node->childs[0]->value);
}

void code_gen(node_t *which) {
  if (which->type == NODE_PROGRAM) {
    code_gen_program(which);
  } else if (which->type == NODE_FUNCDEFINITION) {
    code_gen_func_definition(which);
  } else if (which->type == NODE_CALL) {
    code_gen_call(which);
  } else if (which->type == NODE_RETURN) {
    code_gen_return(which);
  } else if (which->type == NODE_FUNCBODY) {
    int i;
    for (i = 0; i < which->n_childs; i++) {
      code_gen(which->childs[i]);
    }
  }
}