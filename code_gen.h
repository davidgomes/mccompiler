#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>

#include "symbol_t.h"
#include "semantics.h"
#include "structures.h"

void code_gen(node_t *which, char *func_name);

#endif