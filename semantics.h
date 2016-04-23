#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structures.h"

extern char* node_types[];
extern char* type_str[];

void operator_applied2(nodetype_t, node_t*, node_t*);
void an_tree(node_t *where, sym_t *st, char *func_name);
void print_an_tree(node_t* n, int d);
type_t node_type_to_sym_type(nodetype_t type);