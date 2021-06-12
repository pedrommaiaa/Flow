#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure and enum definitions

#define TEXTLEN     512     // legnth of symbols in input
#define NSYMBOLS    1024    // Number of symbol table entrie

// Token structure
typedef struct token 
{
  enum 
  {
    EOF_T, 
    PLUS_T, MINUS_T, 
    STAR_T, SLASH_T,
    EQUAL_T, NOT_EQUAL_T,
    LESS_THAN_T, GREATER_THAN_T, LESS_OR_EQUAL_T, GREATER_OR_EQUAL_T,
    INTLIT_T, SEMI_T, ASSIGN_T, IDENT_T,
    LBRACE_T, RBRACE_T, LPAREN_T, RPAREN_T,
    // Keywords
    PRINT_T, INT_T, IF_T, ELSE_T, WHILE_T,
    FOR_T, VOID_T
  } token;            // Token types
  int intvalue;				// For T_INTLIT, the integer value
} token_T;


// Abstract Syntax Tree structure
typedef struct ASTnode 
{ 
  enum 
  { // AST node types
    ADD_A = 1, SUB_A, MUL_A, DIV_A,
    EQUAL_A, NOT_EQUAL_A, LESS_THAN_A, GREATER_THAN_A, LESS_OR_EQUAL_A, GREATER_OR_EQUAL_A, 
    INTLIT_A,
    IDENT_A, LVIDENT_A, ASSIGN_A, PRINT_A, GLUE_A, 
    IF_A, WHILE_A, FUNCTION_A
  } op;                       // "Operation" to be performed on this tree
  struct ASTnode *left;			  // Left, middle right child trees
  struct ASTnode *mid;
  struct ASTnode *right;
  union 
  {
    int intvalue;				      // For INTLIT_A, the integer value
    int id;				            // For IDENT_A, the symbol slot number
  } v;
} AST_T;


#define NOREG -1              // Use NOREG when the AST generation
                              // functions hve no register to return


// Symbol table structure
typedef struct symtable 
{
  char *name;                 // Name of a symbol
} SymTable;
