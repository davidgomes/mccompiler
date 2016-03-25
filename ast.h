#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>

char* NODE_PROGRAM = "Program";
char* NODE_DECLARATION = "Declaration";
char* NODE_ARRAYDECLARATION = "ArrayDeclaration";
char* NODE_FUNCDECLARATION = "FuncDeclaration";
char* NODE_FUNC_EFINITION = "FuncDefinition";
char* NODE_PARAMLIST = "ParamList";
char* NODE_FUNCBODY = "FuncBody";
char* NODE_PARAMDECLARATION = "ParamDeclaration";
char* NODE_STATLIST = "StatList";
char* NODE_IF = "If";
char* NODE_FOR = "For";
char* NODE_RETURN = "Return";
char* NODE_OR = "Or";
char* NODE_AND = "And";
char* NODE_EQ = "Eq";
char* NODE_NE = "Ne";
char* NODE_LT = "Lt";
char* NODE_GT = "Gt";
char* NODE_LE = "Le";
char* NODE_GE = "Ge";
char* NODE_ADD = "Add"
char* NODE_SUB = "Sub";
char* NODE_MUL = "Mul";
char* NODE_DIV = "Div";
char* NODE_MOD = "Mod";
char* NODE_NOT = "Not";
char* NODE_MINUS = "Minus";
char* NODE_PLUS = "Plus";
char* NODE_ADDR = "Addr";
char* NODE_DEREF = "Deref";
char* NODE_STORE = "Store";
char* NODE_COMMA = "Comma";
char* NODE_CALL = "Call";
char* NODE_CHAR = "Char";
char* NODE_CHRLIT = "Chrlit";
char* NODE_ID = "Id";
char* NODE_INT = "Int";
char* NODE_INTLIT = "Intlit";
char* NODE_POINTER = "Pointer";
char* NODE_STRLIT = "Strlit";
char* NODE_VOID = "Void";
char* NODE_NULL = "Null";

typedef enum { NODE_PROGRAM, NODE_DECLARATION, NODE_ARRAYDECLARATION, NODE_FUNCDECLARATION,
  NODE_FUNC_EFINITION, NODE_PARAMLIST, NODE_FUNCBODY, NODE_PARAMDECLARATION, NODE_STATLIST, NODE_IF, NODE_FOR, NODE_RETURN, NODE_OR,
  NODE_AND, NODE_EQ, NODE_NE, NODE_LT, NODE_GT, NODE_LE, NODE_GE, NODE_ADD, NODE_SUB, NODE_MUL,
  NODE_DIV, NODE_MOD, NODE_NOT, NODE_MINUS, NODE_PLUS, NODE_ADDR, NODE_DEREF, NODE_STORE,
  NODE_COMMA, NODE_CALL, NODE_CHAR, NODE_CHRLIT, NODE_ID, NODE_INT, NODE_INTLIT, NODE_POINTER,
  NODE_STRLIT, NODE_VOID, NODE_NULL } nodetype_t;

struct {
  struct node_t *sibling_right;
  nodetype_t type;

  int n_childs;
  struct node_t **childs;
} typedef node_t;

node_t* ast_create_node(nodetype_t nodetype);
node_t* ast_insert_node(nodetype_t nodetype, int to_use, int node_operands, ...);
void ast_print(node_t* ast);

#endif
