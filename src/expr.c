#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of expressions

// Parse a primary factor and return an
// AST node representing it.
static AST_T *primary(void) 
{
  AST_T *n;
  int id;

  switch (Token.token) 
  {
    // For an INTLIT token, make a leaf AST node for it
    case INTLIT_T: 
          n = mkastleaf(INTLIT_A, Token.intvalue); 
          break;
    case IDENT_T:
          // Check that this identifier exists
          id = findglob(Text);
          if (id == -1)
            fatals("Unkown variable", Text);

          // Make a leaf AST node for it
          n = mkastleaf(IDENT_A, id);
          break;
    
    default:
      fatald("Syntax error, token", Token.token);
  }

  // Scan in the next token and return the leaf node
  scan(&Token);
  return (n);
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
      fatald("Syntax error, token", tokentype);
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
    fatald("Syntax error, token", tokentype);
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
    return (left); 

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
      return (left);
  }

  // Return the tree we have when the precedence
  // is the same or lower
  return (left);
}
