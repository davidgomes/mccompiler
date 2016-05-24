#include "code_gen.h"

int current_str_id = 1;
int r_count = 1;

char* type2llvm(type_t type) {
  if (type == TYPE_INT) {
    return "i32";
  } else if (type == TYPE_CHAR) {
    return "i32";
  } else {
    return "undefined";
  }
}

void node_llvm_type(node_t *which, char *res, char *func_name) {
  if (which->an_type != TYPE_UNKNOWN) {
    strcat(res, type2llvm(which->an_type));

    for (int i = 0; i < which->an_n_pointers; i++) {
      strcat(res, "*");
    }
  } else { // declarations and such
    parse_id_node(st, which, func_name, 1);
    strcat(res, type2llvm(which->an_type));

    for (int i = 0; i < which->an_n_pointers; i++) {
      strcat(res, "*");
    }
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

void code_gen_program(node_t *program_node, char *func_name) {
  printf("declare i32 @puts(i8* nocapture) nounwind\n");

  find_and_save_strings(program_node);

  int i;
  for (i = 0; i < program_node->n_childs; i++) {
    code_gen(program_node->childs[i], func_name);
  }
}

void code_gen_func_definition(node_t *func_def_node, char *func_name) {
  sym_t *table_node = create_func_table_node(func_def_node);

  printf("define %s @%s() {\n", type2llvm(table_node->type), table_node->id);

  int i;
  for (i = 0; i < func_def_node->n_childs; i++) {
    code_gen(func_def_node->childs[i], func_name);
  }

  printf("}\n");
}

void code_gen_call(node_t *call_node, char *func_name) {
  int i;
  for (i = 1; i < call_node->n_childs; i++) {
    code_gen(call_node->childs[i], func_name);
  }
}

void code_gen_declaration(node_t *decl_node, char *func_name) {
  char res[100] = "";
  node_llvm_type(decl_node->childs[decl_node->n_childs - 1], res, func_name);

  printf("%%%s = alloca %s, align 4\n", decl_node->childs[decl_node->n_childs - 1]->value, res);
  printf("store %s 0, %s* %%%s\n", res, res, decl_node->childs[decl_node->n_childs - 1]->value);
}

void code_gen_store(node_t *store_node, char *func_name) {
  code_gen(store_node->childs[1], func_name);
  int which_reg = store_node->childs[1]->reg;
  printf("store %s %%%d, %s* %%%s\n", type2llvm(store_node->childs[0]->an_type), which_reg, type2llvm(store_node->childs[0]->an_type), store_node->childs[0]->value);
}

void code_gen_intlit(node_t *intlit_node, char *func_name) {
  int new_reg = r_count++;
  printf("%%%d = add i32 %s, 0\n", new_reg, intlit_node->value);
  intlit_node->reg = new_reg;
}

void code_gen_chrlit(node_t *chrlit_node, char *func_name) {
  int new_reg = r_count++;
  printf("%%%d = add i32 %d, 0\n", new_reg, atoi(chrlit_node->value));
  chrlit_node->reg = new_reg;
}

void code_gen_return(node_t *return_node) {
  printf("ret i32 %s\n", return_node->childs[0]->value);
}

void code_gen(node_t *which, char *func_name) {
  if (which->type == NODE_PROGRAM) {
    code_gen_program(which, func_name);
  } else if (which->type == NODE_FUNCDEFINITION) {
    func_name = (char *) strdup(get_function_name(which));
    code_gen_func_definition(which, func_name);
  } else if (which->type == NODE_DECLARATION) {
    code_gen_declaration(which, func_name);
  } else if (which->type == NODE_CALL) {
    code_gen_call(which, func_name);
  } else if (which->type == NODE_RETURN) {
    code_gen_return(which);
  } else if (which->type == NODE_FUNCBODY) {
    int i;
    for (i = 0; i < which->n_childs; i++) {
      code_gen(which->childs[i], func_name);
    }
  } else if (which->type == NODE_STRLIT) {
    //int new_reg = r_count++;
    //printf("%%%d = add [%d x i8]* %s\", 0\n", new_reg, (int) strlen(which->value), which->value);
    //printf("i8* getelementptr inbounds ([%d x i8]* @.str.%d, i32 0, i32 0)", (int) strlen(which->value), which->str_id);
  } else if (which->type == NODE_INTLIT) {
    code_gen_intlit(which, func_name);
  } else if (which->type == NODE_CHRLIT) {
    code_gen_chrlit(which, func_name);
  } else if (which->type == NODE_STORE) {
    code_gen_store(which, func_name);
  }
}