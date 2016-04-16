#include <stdio.h>
#include <string.h>

#include "ast.h"

typedef enum {
  GLOBAL,
	FUNC_TABLE,
  FUNC_DECLARATION,
  ARRAY,
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

  /* size of the array */
  int array_size;

	/*The type of the element - String[]; int[]; etc*/
	type_t type;

  /* number of asterisks to show after type */
  int n_pointers;

  /* function definitions need this */
  int n_params;
  struct sym_t** params;

	/*In case the current table corresponds to a method, then this field tells us if the element is a parameter or not*/
	int is_parameter;

	/*Points to the next item on the table*/
	struct sym_t* next;
} sym_t;

sym_t* st;

type_t node_type_to_sym_type(nodetype_t type);
sym_t* st_analyze_ast(node_t *root);
void st_print_table_element(sym_t* element);
void st_print_table(sym_t* table);
