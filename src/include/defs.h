#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure and enum definitions

#define TEXTLEN		      512	  // Length of symbols in input
#define NSYMBOLS        1024	// Number of symbol table entries

// Commands and default filenames
#define AOUT "a.out"
#define ASCMD "as -o "
#define LDCMD "cc -o "


// Token types
enum {
  T_EOF,
  
  // Binary Operators
  T_ASSIGN, T_LOGOR, T_LOGAND,                  // '=', 'or', 'and',
  T_OR, T_XOR, T_AMPER,                         // '|',  '^',  '&',
  T_EQ, T_NE,                                   // '==', '!=',
  T_LT, T_GT, T_LE, T_GE,                       // '<', '>', '<=', '>=',
  T_LSHIFT, T_RSHIFT,                           // '<<', '>>',
  T_PLUS, T_MINUS, T_STAR, T_SLASH,             // '+', '-', '*', '/',
  
  // Other operators
  T_INC, T_DEC, T_INVERT, T_LOGNOT,             // '++', '--', '~', 'not',
  
  // Type keywords
  T_VOID, T_CHAR, T_INT, T_LONG,                // 'void', 'char', 'int', 'long',
  
  // Other keywords
  T_IF, T_ELSE, T_WHILE, T_FOR, T_RETURN,       // 'if', 'else', 'while', 'for', 'return',

  // Structural tokens
  T_INTLIT, T_STRLIT, T_SEMI, T_IDENT,
  T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN,       // '{', '}', '(', ')',
  T_LBRACKET, T_RBRACKET, T_COMMA               // '[', ']', ','
};

// Token structure
typedef struct token {
  int token;			        // Token type, from the enum list above
  int intvalue;			      // For T_INTLIT, the integer value
}token_T;

// AST node types. The first few line up
// with the related tokens
enum {
  A_ASSIGN= 1, A_LOGOR, A_LOGAND, A_OR, A_XOR, A_AND, 
  A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE, A_LSHIFT, A_RSHIFT,
  A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE,
  A_INTLIT, A_STRLIT, A_IDENT, A_GLUE,
  A_IF, A_WHILE, A_FUNCTION, A_WIDEN, A_RETURN,
  A_FUNCCALL, A_DEREF, A_ADDR, A_SCALE,
  A_PREINC, A_PREDEC, A_POSTINC, A_POSTDEC,
  A_NEGATE, A_INVERT, A_LOGNOT, A_TOBOOL
};

// Primitive types
enum {
  P_NONE, P_VOID, P_CHAR, P_INT, P_LONG,
  P_VOIDPTR, P_CHARPTR, P_INTPTR, P_LONGPTR
};

// Abstract Syntax Tree structure
typedef struct ASTnode {
  int op;			            // "Operation" to be performed on this tree
  int type;			          // Type of any expression this tree generates
  int rvalue;			        // True if the node is an rvalue
  struct ASTnode *left;	  // Left, middle and right child trees
  struct ASTnode *mid;
  struct ASTnode *right;
  union {			            // For A_INTLIT, the integer value
    int intvalue;		      // For A_IDENT, the symbol slot number
    int id;			          // For A_FUNCTION, the symbol slot number
    int size;			        // For A_SCALE, the size to scale by
  } v;				            // For A_FUNCCALL, the symbol slot number
} AST_T;

#define NOREG	-1		      // Use NOREG when the AST generation
				                  // functions have no register to return
#define NOLABEL	 0		    // Use NOLABEL when we have no label to
				                  // pass to genAST()

// Structural types
enum {
  S_VARIABLE, S_FUNCTION, S_ARRAY
};


// Storate classes
enum {
  C_GLOBAL = 1,           // Globally visible symbol
  C_LOCAL,                // Locally visible symbol
  C_PARAM                 // Locally visible function parameter
};


// Symbol table structure
typedef struct symtable {
  char *name;			        // Name of a symbol
  int type;			          // Primitive type for the symbol
  int stype;			        // Structural type for the symbol
  int class;              // Storate class for the symbol
  int endlabel;			      // For functions, the end label  
  int size;               // Number of elements in the symbol
  int posn;               // For locals, the negative offset
                          // from the stack base pointer
  #define nelems posn     // For functions, # of params
                          // For structs, # of fields
}SymTable_T;
