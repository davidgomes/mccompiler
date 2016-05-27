#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "structures.h"

extern char* node_types[];
extern char* type_str[];

sym_t *st;
sym_t *last;

extern int semantic_errors;
int octal_decimal(int n);
sym_t *create_node(table_type_t node_type, char *name, type_t type);
sym_t* create_variable_node(node_t *cur_node);
sym_t* create_array_node(node_t *cur_node);
sym_t* create_declaration_node(node_t *cur_node);
sym_t* create_func_table_node(node_t *cur_node);
int st_add_definition(sym_t *st, sym_t *table_node, node_t *cur_node, sym_t *declaration_node);
int add_to_top(sym_t *st, sym_t *node);

void init_st();
void st_print_table_element(sym_t* element);
void st_print_table(sym_t* table);
int add_to_top(sym_t *st, sym_t *node);