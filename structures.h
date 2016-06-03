#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "y.tab.h"

typedef enum { NODE_PROGRAM, NODE_DECLARATION, NODE_ARRAYDECLARATION, NODE_FUNCDECLARATION,
               NODE_FUNCDEFINITION, NODE_PARAMLIST, NODE_FUNCBODY, NODE_PARAMDECLARATION, NODE_STATLIST, NODE_IF, NODE_FOR, NODE_RETURN, NODE_OR,
               NODE_AND, NODE_EQ, NODE_NE, NODE_LT, NODE_GT, NODE_LE, NODE_GE, NODE_ADD, NODE_SUB, NODE_MUL,
               NODE_DIV, NODE_MOD, NODE_NOT, NODE_MINUS, NODE_PLUS, NODE_ADDR, NODE_DEREF, NODE_STORE,
               NODE_COMMA, NODE_CALL, NODE_CHAR, NODE_CHRLIT, NODE_ID, NODE_INT, NODE_INTLIT, NODE_POINTER,
               NODE_STRLIT, NODE_VOID, NODE_NULL, NODE_DECLARATOR, NODE_FUNCDECLARATOR, NODE_FUNCBODYDECLARATION,
               NODE_FUNCTIONBODYSTATEMENT, NODE_STATEMENT, NODE_ARRAYDECLARATOR, NODE_BLOCK, NODE_PROGRAM_BLOCK,
               NODE_EXPRESSION } nodetype_t;

typedef enum {
  TYPE_INT,
  TYPE_CHAR,
  TYPE_VOID,

  TYPE_CHARRAY,
  TYPE_INTARRAY,

  TYPE_UNKNOWN, /* Used internally */
  TYPE_UNDEF
} type_t;

typedef enum {
  GLOBAL,
  FUNC_TABLE,
  FUNC_DECLARATION,
  ARRAY,
  VARIABLE,
  RETURN_NODE
} table_type_t;

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
  struct sym_t* definition;
  int defined;
} sym_t;

typedef struct node {
  struct node *sibling_right;
  nodetype_t type;
  char* value;
  int to_use;
  int n_childs;
  struct node **childs;

  int is_array_access;

  type_t an_type;
  int an_n_pointers;
  int an_array_size;

  /* useful for array access */
  int has_given_error;

  // for function ids in calls
  int an_n_params;
  sym_t **an_params;

  YYLTYPE loc;
  YYLTYPE loc2;

  int str_id; // we hold the string id (0, 1, 2, 3, ...) to tell LLVM which string to print
  int reg;
} node_t;

#endif
