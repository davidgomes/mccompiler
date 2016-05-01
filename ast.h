#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "semantics.h"

node_t *ast;
int were_there_errors;

type_t node_type_to_sym_type(nodetype_t type);
node_t* ast_create_node(nodetype_t nodetype, int to_use);
node_t* ast_insert_node(nodetype_t nodetype, int to_use, int node_operands, ...);
node_t* ast_insert_terminal(nodetype_t nodetype, char* s);
void ast_destroy(node_t *where);
void ast_print_tree(node_t* n, int d);
void ast_print_node(node_t* n);
void ast_print_an_node(node_t* n);
node_t *ast_fix_to_null(node_t *which);
int ast_count_not_nulls(node_t *list, node_t *st);
void ast_add_typespec(node_t *typespec, node_t *declarator);
int ast_count_statement_childs(node_t* which);

#endif
