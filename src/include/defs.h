#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure and enum definitions

// Token types
enum {
  EOF_T, PLUS_T, MINUS_T, STAR_T, SLASH_T, INTLIT_T
};

// Token structure
typedef struct token {
  int token;				  // Token type, from the enum list above
  int intvalue;				// For T_INTLIT, the integer value
} token_T;

// AST node types
enum {
  ADD_A, SUB_A, MUL_A, DIV_A, INTLIT_A
};

// Abstract Syntax Tree structure
typedef struct ASTnode {
  int op;				            // "Operation" to be performed on this tree
  struct ASTnode *left;			// Left and right child trees
  struct ASTnode *right;
  int intvalue;				      // For INTLIT_A, the integer value
} AST_T;
