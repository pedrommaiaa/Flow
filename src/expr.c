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
    case INTLIT_T:
      // For an INTLIT token, make a leaf AST node for it.
      // Make it a CHAR_P if it's within the CHAR_P range.
      if ((Token.intvalue) >= 0 && (Token.intvalue < 256))
        n = mkastleaf(INTLIT_A, CHAR_P, Token.intvalue);
      else
        n = mkastleaf(INTLIT_A, INT_P, Token.intvalue);
      break;

    case IDENT_T:
      // Check that this identifier exists
      id = findglob(Text);
      if (id == -1)
        fatals("Unkown variable", Text);

      // Make a leaf AST node for it
      n = mkastleaf(IDENT_A, Gsym[id].type, id);
      break;
    
    default:
      fatald("Syntax error, token", Token.token);
  }

  // Scan in the next token and return the leaf node
  scan(&Token);
  return (n);
}


// Convert a binary operator token into an AST operation.
// We rely on a 1:1 mapping from token to AST operation.
static int arithop(int tokentype) 
{
  if (tokentype > EOF_T && tokentype < INTLIT_T)
    return (tokentype);
  fatald("Syntax error, token", tokentype);
}


// Operator precedence for each token
// *same order as the token types*
static int OpPrec[] = { 
  0, 10, 10,          // EOF_T, PLUS_T, MINUS_T 
  20, 20,             // STAR_T, SLASH_T
  30, 30,             // EQUAL_T, NOT_EQUAL_T 
  40, 40, 40, 40      // LESS_THAN_T, GREATER_THAN_T, LESS_OR_EQUAL_T, GREATER_OR_EQUAL_T 
};

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
  int lefttype, righttype;
  int tokentype;

  // Get the integer literal on the left.
  // Fetch the next token at the same time.
  left = primary();

  // If we hit a semicolon or ')', return just the left node
  tokentype = Token.token;
  if (tokentype == SEMI_T || tokentype == RPAREN_T)
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

    // Ensure the two types are compatible.
    lefttype = left->type;
    righttype = right->type;
    if (!type_compatible(&lefttype, &righttype, 0))
      fatal("Incompatible types");

    // Widen either side if required. type vars are WIDEN_A now
    if (lefttype)
      left = mkastunary(lefttype, right->type, left, 0);
    if (righttype)
      right = mkastunary(righttype, left->type, right, 0);

    // Join that sub-tree with ours. Convert the token
    // into an AST operation at the same time.
    left = mkastnode(arithop(tokentype), left->type, left, NULL, right, 0);

    // Update the details of the current token.
    // If we hit a semicolon or ')', return just the left node
    tokentype = Token.token;
    if (tokentype == SEMI_T || tokentype == RPAREN_T)
      return (left);
  }

  // Return the tree we have when the precedence
  // is the same or lower
  return (left);
}
