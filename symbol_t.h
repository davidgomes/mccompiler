#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "structures.h"

extern char* node_types[];
extern char* type_str[];

sym_t* st;

sym_t* st_analyze_ast(node_t *root);
void st_print_table_element(sym_t* element);
void st_print_table(sym_t* table);
