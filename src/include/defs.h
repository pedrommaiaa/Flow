#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure and enum definitions

#define TEXTLEN 512 // legnth of symbols in input

// Token structure
typedef struct token {
  enum {
    EOF_T, 
    PLUS_T, MINUS_T, 
    STAR_T, SLASH_T, 
    INTLIT_T, SEMI_T,
    // Keywords
    PRINT_T
  } token;            // Token types
  int intvalue;				// For T_INTLIT, the integer value
} token_T;


// Abstract Syntax Tree structure
typedef struct ASTnode { 
  enum { // AST node types
    ADD_A, 
    SUB_A, 
    MUL_A, 
    DIV_A, 
    INTLIT_A
  } op;                     // "Operation" to be performed on this tree
  struct ASTnode *left;			// Left and right child trees
  struct ASTnode *right;
  int intvalue;				      // For INTLIT_A, the integer value
} AST_T;
