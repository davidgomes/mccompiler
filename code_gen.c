#include "code_gen.h"

int current_str_id = 1;
int r_count = 1;
int returned = 0;

char* type2llvm(type_t type, int n_pointers) {
  if (type == TYPE_INT) {
    if (n_pointers == 0) {
      return "i32";
    } else {
      return "i8";
    }
  } else if (type == TYPE_CHAR) {
    return "i8";
  } else if (type == TYPE_VOID) {
    if (n_pointers == 0) {
      return "void";
    } else {
      return "i8";
    }
  } else {
    return "undefined";
  }
}

int is_global(char *id, char *func_name) {
  sym_t *cur_st_node = st;
  int global = 0;

  while (cur_st_node != NULL) {
    if (!strcmp(id, cur_st_node->id) && cur_st_node->node_type == VARIABLE) {
      global = 1;
      break;
    }

    cur_st_node = cur_st_node->next;
  }

  if (func_name != NULL) {
    cur_st_node = st;

    while (cur_st_node != NULL) {
      if (!strcmp(func_name, cur_st_node->id)) {
        break;
      }

      cur_st_node = cur_st_node->next;
    }

    cur_st_node = cur_st_node->definition->next;

    while (cur_st_node != NULL) {
      if (cur_st_node->id != NULL) {
        if (!strcmp(id, cur_st_node->id) && cur_st_node->node_type == VARIABLE) {
          global = 0;
          break;
        }
      }

      cur_st_node = cur_st_node->next;
    }
  }

  return global;
}

char *get_var(node_t* which, char *func_name) {
  char res[100] = "";

  if (is_global(which->value, func_name)) {
  	sprintf(res, "@%s", which->value);
  } else {
    sprintf(res, "%%%s", which->value);
  }

  return strdup(res);
}

void node_llvm_type(node_t *which, char *res, char *func_name) {
  if (which->an_type != TYPE_UNKNOWN) {
    int n_pointers = which->an_n_pointers;

    if (which->an_array_size >= 1) {
      n_pointers++;
    }

    strcat(res, type2llvm(which->an_type, n_pointers));

    for (int i = 0; i < n_pointers; i++) {
      strcat(res, "*");
    }
  } else { // declarations and such
    parse_id_node(st, which, func_name, 1);
    strcat(res, type2llvm(which->an_type, which->an_n_pointers));

    for (int i = 0; i < which->an_n_pointers; i++) {
      strcat(res, "*");
    }
  }
}

void sym_t_llvm_type(sym_t *which, char *res, char *func_name) {
  if (which->type != TYPE_UNKNOWN) {
    int n_pointers = which->n_pointers;

    if (which->array_size >= 1) {
      n_pointers++;
    }

    strcat(res, type2llvm(which->type, n_pointers));

    for (int i = 0; i < n_pointers; i++) {
      strcat(res, "*");
    }
  }
}

void find_and_save_strings(node_t *which) {
  if (which->type == NODE_STRLIT) {
    // remove first character of ->value (the opening quote)
    which->value++;
    // remove last character of ->value (the closing quote) and put in a null byte
    which->value[strlen(which->value) - 1] = 0;

    printf("@.str.%d = private unnamed_addr constant [%d x i8] c\"", current_str_id, mystrlen(which->value) + 1);

    int i = 0;
    int previous_slash = 0;
    int slash_block = 0;
    while (i < strlen(which->value)) {
      if (slash_block) {
        if (which->value[i] >= '0' && which->value[i] <= '7') {
          if (previous_slash) {
            printf("0");
          }

        } else if (which->value[i] >= '7' && which->value[i] <= '9') {
          slash_block = 0;
        } else {
          if (previous_slash) {
            if (which->value[i] == 'n') {
              printf("0A");
            } else if (which->value[i] == '\'') {
              printf("27");
            } else if (which->value[i] == '\\') {
              printf("5C");
            } else if (which->value[i] == 't') {
              printf("09");
            } else if (which->value[i] == '\"') {
              printf("22");
            }
          } else {
            printf("%c", which->value[i]);
          }

          slash_block = 0;
          previous_slash = 0;

          i++;
          continue;
        }

        previous_slash = 0;
      }

      if (which->value[i] == '\\') {
        previous_slash = 1;
        slash_block = 1;
      }

      printf("%c", which->value[i]);

      i++;
    }

    printf("\\00\"\n");

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
  sym_t *declaration_node_temp = create_declaration_node(func_decl_node);

  sym_t *cur_st_node = st;
  sym_t *actual_decl_node = NULL;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, declaration_node_temp->id)) {
      actual_decl_node = cur_st_node;
      break;
    }

    cur_st_node = cur_st_node->next;
  }

  if (actual_decl_node->definition != NULL) { // review this with Professor
    return;
  }

  char res[100] = "";
  sym_t_llvm_type(actual_decl_node, res, func_name);

  printf("declare %s @%s(", res, actual_decl_node->id);

  int i;
  for (i = 0; i < actual_decl_node->n_params; i++) {
    char arg_res[100] = "";
    sym_t_llvm_type(actual_decl_node->params[i], arg_res, func_name);
    printf("%s", arg_res);

    if (i != actual_decl_node->n_params - 1) {
      printf(",");
    }
  }

  printf(")\n");
}

void code_gen_func_definition(node_t *func_def_node, char *func_name) {
  sym_t *table_node = create_func_table_node(func_def_node);
  int i;

  char res[100] = "";
  sym_t_llvm_type(table_node, res, func_name);

  printf("define %s @%s(", res, table_node->id);

  sym_t *cur_st_node = st;
  sym_t *func_node = NULL;

  while (cur_st_node != NULL) {
    if (!strcmp(cur_st_node->id, table_node->id)) {
      func_node = cur_st_node;
      break;
    }

    cur_st_node = cur_st_node->next;
  }

  if (func_node->n_params != 0) {
    cur_st_node = func_node->definition->next->next; // first parameter
  }

  for (i = 0; i < func_node->n_params; i++) {
    char arg_res[100] = "";
    sym_t_llvm_type(cur_st_node, arg_res, func_name);

    if (cur_st_node->type == TYPE_VOID && cur_st_node->n_pointers == 0) {
      continue;
    }

    printf("%s %%.%s", arg_res, cur_st_node->id);

    if (i != func_node->n_params - 1) {
      printf(",");
    }

    cur_st_node = cur_st_node->next;
  }

  printf(") {\n");

  r_count = 1;

  returned = 0;
  for (i = 0; i < func_def_node->n_childs; i++) {
    code_gen(func_def_node->childs[i], func_name);
  }

  if (!returned) {
    printf("ret void\n");
  }

  printf("}\n");
}

void code_gen_id(node_t *node_id, char *func_name) {
  if (node_id->an_type != TYPE_UNKNOWN) {
    int new_reg = r_count++;
    char res[100] = "";
    node_llvm_type(node_id, res, func_name);

    printf("%%%d = load %s* %s\n", new_reg, res, get_var(node_id, func_name));
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

  int new_reg = -1;

  if (call_node->an_type == TYPE_VOID && call_node->an_n_pointers == 0) {
    printf("call %s (", res);
  } else {
    new_reg = r_count++;
    printf("%%%d = call %s (", new_reg, res);
  }

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

  if (new_reg != -1) {
    call_node->reg = new_reg;
  }

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

  if (is_global(decl_node->childs[decl_node->n_childs - 1]->value, func_name)) {
    int n_pointers = decl_node->childs[decl_node->n_childs - 1]->an_n_pointers;

    if (n_pointers == 0) {
      printf("@%s = global %s %s\n", decl_node->childs[decl_node->n_childs - 1]->value, res, "0");
    } else {
      printf("@%s = global %s null\n", decl_node->childs[decl_node->n_childs - 1]->value, res);
    }
  } else {
    printf("%%%s = alloca %s, align 4\n", decl_node->childs[decl_node->n_childs - 1]->value, res);

    int n_pointers = decl_node->childs[decl_node->n_childs - 1]->an_n_pointers;

    if (decl_node->childs[decl_node->n_childs - 1]->an_array_size >= 1) {
      n_pointers++;
    }

    if (n_pointers == 0) {
      printf("store %s 0, %s* %%%s\n", res, res, decl_node->childs[decl_node->n_childs - 1]->value);
    }
  }
}

void code_gen_store(node_t *store_node, char *func_name) {
  code_gen(store_node->childs[1], func_name);
  int which_reg = store_node->childs[1]->reg;

  char res[100] = "";
  node_llvm_type(store_node->childs[0], res, func_name);

  printf("store %s %%%d, %s* %s\n", res, which_reg, res, get_var(store_node->childs[0], func_name));
}

void code_gen_strlit(node_t *strlit_node, char *func_name) {
  int new_reg = r_count++;
  printf("%%%d = getelementptr [%d x i8]* @.str.%d, i64 0, i64 0\n", new_reg, (int) mystrlen(strlit_node->value) + 1, strlit_node->str_id);
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
  returned = 1;

  int n_pointers = return_node->childs[0]->an_n_pointers;

  if (return_node->an_array_size >= 1) {
    n_pointers++;
  }

  if ((return_node->childs[0]->an_type == TYPE_VOID && n_pointers == 0) || return_node->childs[0]->type == NODE_NULL) {
    printf("ret void\n");
  } else {
    code_gen(return_node->childs[0], func_name);
    printf("ret i32 %%%d\n", return_node->childs[0]->reg);
  }
}

void code_gen(node_t *which, char *func_name) {
  if (which->type == NODE_PROGRAM) {
    code_gen_program(which, func_name);
  } else if (which->type == NODE_FUNCDEFINITION) {
    func_name = (char *) strdup(get_function_name(which));
    code_gen_func_definition(which, func_name);
  } else if (which->type == NODE_DECLARATION) {
    code_gen_declaration(which, func_name);
  } else if (which->type == NODE_FUNCDECLARATION) {
    code_gen_func_declaration(which, func_name);
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
    char res[100] = "";

    sym_t *param_temp = create_variable_node(which);
    sym_t_llvm_type(param_temp, res, func_name);

    int n_pointers = param_temp->n_pointers;

    if (param_temp->type == TYPE_VOID && param_temp->n_pointers == 0) {
      return;
    }

    printf("%%%s = alloca %s, align 4\n", param_temp->id, res);

    if (n_pointers == 0) {
      printf("store %s 0, %s* %%%s\n", res, res, param_temp->id);
    }

    printf("store %s %%.%s, %s* %%%s\n", res, param_temp->id, res, param_temp->id);
  } else if (which->type == NODE_ID) {
    code_gen_id(which, func_name);
  }
}