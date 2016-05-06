#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structures.h"
#include "ast.h"
#include "symbol_t.h"

extern char* node_types[];
extern char* node_types_err[];
extern char* type_str[];

void an_tree(node_t *where, sym_t *st, char *func_name, int an, int bad);
void print_an_tree(node_t* n, int d, int an);
type_t node_type_to_sym_type(nodetype_t type);

#endif