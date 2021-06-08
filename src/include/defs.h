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
    INTLIT_T, SEMI_T,
    EQUALS_T, IDENT_T,
    // Keywords
    PRINT_T, INT_T
  } token;            // Token types
  int intvalue;				// For T_INTLIT, the integer value
} token_T;


// Abstract Syntax Tree structure
typedef struct ASTnode 
{ 
  enum 
  { // AST node types
    ADD_A, 
    SUB_A, 
    MUL_A, 
    DIV_A, 
    INTLIT_A,
    IDENT_A,
    LVIDENT_A,
    ASSIGN_A
  } op;                       // "Operation" to be performed on this tree
  struct ASTnode *left;			  // Left and right child trees
  struct ASTnode *right;
  union 
  {
    int intvalue;				      // For INTLIT_A, the integer value
    int id;				            // For IDENT_A, the symbol slot number
  } v;
} AST_T;


// Symbol table structure
typedef struct symtable 
{
  char *name;                 // Name of a symbol
} SymTable;
