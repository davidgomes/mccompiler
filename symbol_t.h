#include <stdio.h>
#include <string.h>

#include "ast.h"

typedef enum {
  GLOBAL,
	FUNC_TABLE,
  VARIABLE,
  RETURN_NODE
} table_type_t;

extern char* node_types[];
extern char* type_str[];

typedef struct sym_t {
	/*Tells us if the current element is the table of a class, the table of a method, a variable, or a method*/
	table_type_t node_type;

	/*Name of the element (main, x, etc)*/
	char* id;

	/*The type of the element - String[]; int[]; etc*/
	type_t type;

	/*In case the current table corresponds to a method, then this field tells us if the element is a parameter or not*/
	int is_parameter;

	/*Points to the next item on the table*/
	struct sym_t* next;

	/*In case we have a method point to the method symbolic table*/
	struct sym_t* table_method;

	/* Pointer to the node which contains the first statements of this method,
	this is used later on for error checking, and might be used later on for code generation.
	It should thus point to a MethodBody node, or NULL */
	node_t* method_start;
} sym_t;

sym_t* st;

sym_t* st_analyze_ast(node_t *root);
void st_print_table_element(sym_t* element);
void st_print_table(sym_t* table);
