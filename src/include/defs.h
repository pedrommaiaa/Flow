#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure and enum definitions

#define TEXTLEN		      512	  // Length of symbols in input
#define NSYMBOLS        1024	// Number of symbol table entries


// Token structure
typedef struct token {
  enum {
    EOF_T,
    // Operators
    PLUS_T, MINUS_T,
    STAR_T, SLASH_T,
    EQUAL_T, NOT_EQUAL_T,
    LESS_THAN_T, GREATER_THAN_T, LESS_OR_EQUAL_T, GREATER_OR_EQUAL_T,
    // Type keywords
    VOID_T, CHAR_T, INT_T, LONG_T,
    // Structural tokens
    INTLIT_T, SEMI_T, ASSIGN_T, IDENT_T,
    LBRACE_T, RBRACE_T, LPAREN_T, RPAREN_T,
    AMPER_T, COMMA_T,
    // Other keywords
    PRINT_T, IF_T, ELSE_T, WHILE_T, FOR_T, RETURN_T,
    AND_T
  } token;          // Token type, from the enum list above			
  int intvalue;			// For INTLIT_T, the integer value
} token_T;



// Primitive types
enum {
  NONE_P, VOID_P, CHAR_P, INT_P, LONG_P,
  VOIDPTR_P, CHARPTR_P, INTPTR_P, LONGPTR_P
};

// Abstract Syntax Tree structure
typedef struct ASTnode {
  // AST node types. The first few line up
  // with the related tokens
  enum {
    ADD_A = 1, SUB_A, MUL_A, DIV_A,
    EQUAL_A, NOT_EQUAL_A, LESS_THAN_A, GREATER_THAN_A, LESS_OR_EQUAL_A, GREATER_OR_EQUAL_A,
    INTLIT_A,
    IDENT_A, LVIDENT_A, ASSIGN_A, PRINT_A, GLUE_A,
    IF_A, WHILE_A, FUNCTION_A, WIDEN_A, RETURN_A,
    FUNCCALL_A, DEREF_A, ADDR_A
  }op;			              // "Operation" to be performed on this tree
  int type;			          // Type of any expression this tree generates
  struct ASTnode *left;		// Left, middle and right child trees
  struct ASTnode *mid;
  struct ASTnode *right;
  union {			            // For INTLIT, the integer value
    int intvalue;		      // For IDENT, the symbol slot number
    int id;			          // For FUNCTION, the symbol slot number
  } v;				            // For FUNCCALL, the symbol slot number
} AST_T;


#define NOREG	-1		// Use NOREG when the AST generation
				            // functions have no register to return

// Symbol table structure
typedef struct symtable {
  char *name;			          // Name of a symbol
  int type;			            // Primitive type for the symbol
  enum {
    VARIABLE_S, FUNCTION_S
  }stype;                   // Structural type for the symbol		
  int endlabel;			        // For S_FUNCTIONs, the end label
} SymTable;
