#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of expressions

// Parse a primary factor and return an
// AST node representing it.
static AST_T *primary(void) 
{
  AST_T *n;

  // For an INTLIT token, make a leaf AST node for it
  // and scan in the next token. Otherwise, a syntax error
  // for any other token type.
  switch (Token.token) 
  {
    case INTLIT_T:
      n = mkastleaf(INTLIT_A, Token.intvalue);
      scan(&Token);
      return (n);
    default:
      fprintf(stderr, "[Line %d] Syntax error, token '%d'\n", Line, Token.token);
      exit(1);
  }
}


// Convert a binary operator token into an AST operation.
static int arithop(int tokentype) 
{
  switch (tokentype) 
  {
    case PLUS_T: return (ADD_A);
    case MINUS_T: return (SUB_A);
    case STAR_T: return (MUL_A);
    case SLASH_T: return (DIV_A);
    default:
      fprintf(stderr, "[Line %d] syntax error, token '%d'\n", Line, tokentype);
      exit(1);
  }
}

// Operator precedence for each token
// *same order as the token types*
static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };
//                     EOF  +   -   *   / INTLIT

// Check that we have a binary operator and
// return its precedence.
static int op_precedence(int tokentype) 
{
  int prec = OpPrec[tokentype];
  if (prec == 0) 
  {
    fprintf(stderr, "[Line %d] Syntax error, token '%d'\n", Line, tokentype);
    exit(1);
  }
  return (prec);
}

// Return an AST tree whose root is a binary operator.
// Parameter ptp is the previous token's precedence.
AST_T *binexpr(int ptp) 
{
  AST_T *left, *right;
  int tokentype;

  // Get the integer literal on the left.
  // Fetch the next token at the same time.
  left = primary();

  // If no tokens left, return just the left node
  tokentype = Token.token;
  if (tokentype == SEMI_T)
  {
    return (left); 
  }

  // While the precedence of this token is
  // more than that of the previous token precedence
  while (op_precedence(tokentype) > ptp) 
  {
    // Fetch in the next integer literal
    scan(&Token);

    // Recursively call binexpr() with the
    // precedence of our token to build a sub-tree
    right = binexpr(OpPrec[tokentype]);

    // Join that sub-tree with ours. Convert the token
    // into an AST operation at the same time.
    left = mkastnode(arithop(tokentype), left, right, 0);

    // Update the details of the current token.
    // If no tokens left, return just the left node
    tokentype = Token.token;
    if (tokentype == SEMI_T)
    { 
      return (left);
    }
  }

  // Return the tree we have when the precedence
  // is the same or lower
  return (left);
}
