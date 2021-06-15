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
    // Other Keywords
    PRINT_T, IF_T, ELSE_T, WHILE_T,
    FOR_T, RETURN_T
  } token;            // Token types
  int intvalue;				// For T_INTLIT, the integer value
} token_T;


// Primitive types
enum
{
  NONE_P, VOID_P, CHAR_P, INT_P, LONG_P 
};


// Abstract Syntax Tree structure
typedef struct ASTnode 
{ 
  enum 
  {                           // AST node types. The first few line up with the related tokens.
    ADD_A = 1, SUB_A, MUL_A, DIV_A,
    EQUAL_A, NOT_EQUAL_A, LESS_THAN_A, GREATER_THAN_A, LESS_OR_EQUAL_A, GREATER_OR_EQUAL_A, 
    INTLIT_A,
    IDENT_A, LVIDENT_A, ASSIGN_A, PRINT_A, GLUE_A, 
    IF_A, WHILE_A, FUNCTION_A, WIDEN_A, RETURN_A,
    FUNCCALL_A
  } op;                       // "Operation" to be performed on this tree.
  int type;                   // Type of any expression this tree generates.
  struct ASTnode *left;			  // Left, middle right child trees
  struct ASTnode *mid;
  struct ASTnode *right;
  union                       // For INTLIT_A, the symbol slot number 
  {                           // For IDENT_A, the symbol slot number
    int intvalue;				      // For FUNCTION_A, the integer value
    int id;				            // For FUNCCALL_A, the symbol slot number
  } v;                        
} AST_T;


#define NOREG -1              // Use NOREG when the AST generation
                              // functions hve no register to return


// Symbol table structure
typedef struct symtable 
{
  char *name;                 // Name of a symbol
  int type;                   // Primitive type for the symbol
  enum                        // Structural type got the symbol
  {
    VARIABLE_S, FUNCTION_S
  } stype;
  int endlabel;               // For FUNCTION_Ss, the end label
} SymTable;
