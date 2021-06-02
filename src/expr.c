#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of expressions

// Parse a primary factor and return an
// AST node representing it.
static struct ASTnode *primary(void) {
  struct ASTnode *n;

  // For an INTLIT token, make a leaf AST node for it
  // and scan in the next token. Otherwise, a syntax error
  // for any other token type.
  switch (Token.token) {
    case INTLIT_T:
      n = mkastleaf(INTLIT_A, Token.intvalue);
      scan(&Token);
      return (n);
    default:
      fprintf(stderr, "[Line %d] Syntax error\n", Line);
      exit(1);
  }
}


// Convert a token into an AST operation.
int arithop(int tok) {
  switch (tok) {
    case PLUS_T: return (ADD_A);
    case MINUS_T: return (SUB_A);
    case STAR_T: return (MUL_A);
    case SLASH_T: return (DIV_A);
    default:
      fprintf(stderr, "[Line %d] Unknown token in arithop()\n", Line);
      exit(1);
  }
}

// Return an AST tree whose root is a binary operator
struct ASTnode *binexpr(void) {
  struct ASTnode *n, *left, *right;
  int nodetype;

  // Get the integer literal on the left.
  // Fetch the next token at the same time.
  left = primary();

  // If no tokens left, return just the left node
  if (Token.token == EOF_T)
    return (left);

  // Convert the token into a node type
  nodetype = arithop(Token.token);

  // Get the next token in
  scan(&Token);

  // Recursively get the right-hand tree
  right = binexpr();

  // Now build a tree with both sub-trees
  n = mkastnode(nodetype, left, right, 0);
  return (n);
}
