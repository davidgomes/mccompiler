#include "code_gen.h"

int current_str_id = 1;
int r_count = 1;

char* type2llvm(type_t type) {
  if (type == TYPE_INT) {
    return "i32";
  } else if (type == TYPE_CHAR) {
    return "i8";
  } else if (type == TYPE_VOID) {
    return "void";
  } else {
    return "undefined";
  }
}

char *get_var(node_t* which) {
  char res[100] = "";

  // TODO Implement is_global

  //if (is_global(p)) {
  //	sprintf(res, "@_%s", p->value);
  //} else {
  sprintf(res, "%%%s", which->value);
  //}

  return strdup(res);
}


void node_llvm_type(node_t *which, char *res, char *func_name) {
  if (which->an_type != TYPE_UNKNOWN) {
    strcat(res, type2llvm(which->an_type));

    int n_pointers = which->an_n_pointers;

    if (which->an_array_size >= 1) {
      n_pointers++;
    }

    for (int i = 0; i < n_pointers; i++) {
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

void sym_t_llvm_type(sym_t *which, char *res, char *func_name) {
  if (which->type != TYPE_UNKNOWN) {
    strcat(res, type2llvm(which->type));

    int n_pointers = which->n_pointers;

    if (which->array_size >= 1) {
      n_pointers++;
    }

    for (int i = 0; i < n_pointers; i++) {
      strcat(res, "*");
    }
  }
}

void find_and_save_strings(node_t *which) {
  if (which->type == NODE_STRLIT) {
    // remove first character of ->value (the opening quote)
    which->value = which->value++;
    // remove last character of ->value (the closing quote) and put in a null byte
    which->value[mystrlen(which->value) - 2 + 1] = 0;

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
  printf("declare i32 @atoi(i8*) nounwind readonly\n");
  printf("declare i32 @__sprintf_chk(i8*, i32, i64, i8*, ...) #1\n");
  printf("@.str = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
  printf("declare i64 @llvm.objectsize.i64.p0i8(i8*, i1) #2\n");

  printf("define i8* @itoa(i32 %%n, i8* %%buf) #0 {\n");
    printf("%%1 = alloca i32, align 4\n");
    printf("%%2 = alloca i8*, align 8\n");
    printf("store i32 %%n, i32* %%1, align 4\n");
    printf("store i8* %%buf, i8** %%2, align 8\n");
    printf("%%3 = load i8** %%2, align 8\n");
    printf("%%4 = load i8** %%2, align 8\n");
    printf("%%5 = call i64 @llvm.objectsize.i64.p0i8(i8* %%4, i1 false)\n");
    printf("%%6 = load i32* %%1, align 4\n");
    printf("%%7 = call i32 (i8*, i32, i64, i8*, ...)* @__sprintf_chk(i8* %%3, i32 0, i64 %%5, i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %%6)\n");
    printf("%%8 = load i8** %%2, align 8\n");
    printf("ret i8* %%8\n");
  printf("}\n");

  find_and_save_strings(program_node);

  int i;
  for (i = 0; i < program_node->n_childs; i++) {
    code_gen(program_node->childs[i], func_name);
  }
}

void code_gen_func_declaration(node_t *func_decl_node, char *func_name) {

}

void code_gen_func_definition(node_t *func_def_node, char *func_name) {
  sym_t *table_node = create_func_table_node(func_def_node);
  int i;

  printf("define %s @%s(", type2llvm(table_node->type), table_node->id);

  sym_t *cur_st_node = st;
  sym_t *func_node = NULL;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, table_node->id)) {
      func_node = cur_st_node;
      break;
    }

    cur_st_node = cur_st_node->next;
  }


  for (i = 0; i < func_node->n_params; i++) {
    char arg_res[100] = "";
    sym_t_llvm_type(func_node->params[i], arg_res, func_name);

    printf("%s %%%s", arg_res, func_node->params[i]->id);

    if (i != func_node->n_params - 1) {
      printf(",");
    }
  }

  printf(") {\n");

  r_count = 1;

  for (i = 0; i < func_def_node->n_childs; i++) {
    code_gen(func_def_node->childs[i], func_name);
  }

  printf("}\n");
}

void code_gen_id(node_t *node_id, char *func_name) {
  if (node_id->an_type != TYPE_UNKNOWN) {
    int new_reg = r_count++;
    char res[100] = "";
    node_llvm_type(node_id, res, func_name);

    printf("%%%d = load %s* %s\n", new_reg, res, get_var(node_id));
    node_id->reg = new_reg;
  }
}

void code_gen_call(node_t *call_node, char *func_name) {
  int i;
  for (i = 1; i < call_node->n_childs; i++) {
    code_gen(call_node->childs[i], func_name);
  }

  char res[100] = "";
  node_llvm_type(call_node, res, func_name);

  int new_reg = r_count++;
  printf("%%%d = call %s (", new_reg, res);

  for (i = 1; i < call_node->n_childs; i++) {
    char arg_res[100] = "";
    node_llvm_type(call_node->childs[i], arg_res, func_name);

    printf("%s", arg_res);

    if (i != call_node->n_childs - 1) {
      printf(",");
    }
  }

  printf(")* ");

  printf("@%s(", call_node->childs[0]->value);
  call_node->reg = new_reg;

  for (i = 1; i < call_node->n_childs; i++) {
    char arg_res[100] = "";
    node_llvm_type(call_node->childs[i], arg_res, func_name);

    printf("%s %%%d", arg_res, call_node->childs[i]->reg);

    if (i != call_node->n_childs - 1) {
      printf(",");
    }
  }

  printf(")\n");
}

void code_gen_declaration(node_t *decl_node, char *func_name) {
  char res[100] = "";
  node_llvm_type(decl_node->childs[decl_node->n_childs - 1], res, func_name);

  printf("%%%s = alloca %s, align 4\n", decl_node->childs[decl_node->n_childs - 1]->value, res);

  int n_pointers = decl_node->childs[decl_node->n_childs - 1]->an_n_pointers;

  if (decl_node->childs[decl_node->n_childs - 1]->an_array_size >= 1) {
    n_pointers++;
  }

  if (n_pointers == 0) {
    printf("store %s 0, %s* %%%s\n", res, res, decl_node->childs[decl_node->n_childs - 1]->value);
  }
}

void code_gen_store(node_t *store_node, char *func_name) {
  code_gen(store_node->childs[1], func_name);
  int which_reg = store_node->childs[1]->reg;

  char res[100] = "";
  node_llvm_type(store_node->childs[0], res, func_name);

  printf("store %s %%%d, %s* %%%s\n", res, which_reg, res, store_node->childs[0]->value);
}

void code_gen_strlit(node_t *strlit_node, char *func_name) {
  int new_reg = r_count++;
  printf("%%%d = getelementptr [%d x i8]* @.str.%d, i64 0, i64 0\n", new_reg, (int) strlen(strlit_node->value), strlit_node->str_id);
  strlit_node->reg = new_reg;
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

void code_gen_return(node_t *return_node, char *func_name) {
  code_gen(return_node->childs[0], func_name);
  printf("ret i32 %%%d\n", return_node->childs[0]->reg);
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
    code_gen_return(which, func_name);
  } else if (which->type == NODE_FUNCBODY) {
    int i;
    for (i = 0; i < which->n_childs; i++) {
      code_gen(which->childs[i], func_name);
    }
  } else if (which->type == NODE_STRLIT) {
    code_gen_strlit(which, func_name);
  } else if (which->type == NODE_INTLIT) {
    code_gen_intlit(which, func_name);
  } else if (which->type == NODE_CHRLIT) {
    code_gen_chrlit(which, func_name);
  } else if (which->type == NODE_STORE) {
    code_gen_store(which, func_name);
  } else if (which->type == NODE_PARAMLIST) {
    int i;
    for (i = 0; i < which->n_childs; i++) {
      code_gen(which->childs[i], func_name);
    }
  } else if (which->type == NODE_PARAMDECLARATION) {
    //code_gen_declaration(which, func_name);
  } else if (which->type == NODE_ID) {
    code_gen_id(which, func_name);
  }
}