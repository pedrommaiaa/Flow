#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of expressions

// Parse a function call with a single expression
// argument and return its AST
AST_T *funccall(void) 
{
  AST_T *tree;
  int id;

  // Check that the identifier has been defined,
  // then make a leaf node for it. XXX Add structural type test
  if ((id = findglob(Text)) == -1) 
  {
    fatals("Undeclared function", Text);
  }
  // Get the '('
  lparen();

  // Parse the following expression
  tree = binexpr(0);

  // Build the function call AST node. Store the
  // function's return type as this node's type.
  // Also record the function's symbol-id
  tree = mkastunary(FUNCCALL_A, Gsym[id].type, tree, id);

  // Get the ')'
  rparen();
  return (tree);
}

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
      // Make it a CHAR_P if it's within the CHAR_P range
      if ((Token.intvalue) >= 0 && (Token.intvalue < 256))
	      n = mkastleaf(INTLIT_A, CHAR_P, Token.intvalue);
      else
	      n = mkastleaf(INTLIT_A, INT_P, Token.intvalue);
      break;

    case IDENT_T:
      // This could be a variable or a function call.
      // Scan in the next token to find out
      scan(&Token);

      // It's a '(', so a function call
      if (Token.token == LPAREN_T)
	      return (funccall());

      // Not a function call, so reject the new token
      reject_token(&Token);

      // Check that the variable exists. XXX Add structural type test
      id = findglob(Text);
      if (id == -1)
	      fatals("Unknown variable", Text);

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
// We rely on a 1:1 mapping from token to AST operation
static int arithop(int tokentype) 
{
  if (tokentype > EOF_T && tokentype < INTLIT_T)
    return (tokentype);
  fatald("Syntax error, token", tokentype);
  return (0); // Keep -Wall happy
}

// Operator precedence for each token. Must
// match up with the order of tokens in defs.h
static int OpPrec[] = 
{
  0, 10, 10,			  // EOF_T, PLUS_T, MINUS_T
  20, 20,			      // STAR_T, SLASH_T
  30, 30,			      // EQUAL_T, NOT_EQUAL_T
  40, 40, 40, 40		// LESS_THAN_T, GREATER_THAN_T, LESS_OR_EQUAL_T, GREATER_OR_EQUAL_T
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


// Parse a prefix expression and return
// a sub-tree representing it.
AST_T *prefix(void)
{
  AST_T *tree;
  switch (Token.token)
  {
    case AMPER_T:
      // Get the next token and parse it
      // recursively as a prefix expression
      scan(&Token);
      tree = prefix();

      // Ensure that it's an identifier
      if (tree->op != IDENT_A)
        fatal("& operator must be followed by an identifier");

      // Now change the operator to ADDR_A and the type 
      // to a pointer to the original type
      tree->op = ADDR_A; 
      tree->type = pointer_to(tree->type);
      break;
    
    case STAR_T:
      // Get the next token and parse it
      // recursively as a prefix expression
      scan(&Token);
      tree = prefix();

      // For now, ensure it's either another deref or
      // an identifier
      if (tree->op != IDENT_A && tree->op != DEREF_A)
        fatal("* operator must be followed by an identifier or *");

      // Prepend an DEREF_A operation to the tree
      tree = mkastunary(DEREF_A, value_at(tree->type), tree, 0);
      break;
    
    default: tree = primary();
  }
  return (tree);
}



// Return an AST tree whose root is a binary operator.
// Parameter ptp is the previous token's precedence.
AST_T *binexpr(int ptp) 
{
  AST_T *left, *right;
  AST_T *ltemp, *rtemp;
  int ASTop;
  int tokentype;

  // Get the tree on the left.
  // Fetch the next token at the same time.
  left = prefix();

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

    // Ensure the two types are compatible by trying
    // to modify each tree to match the other's type.
    ASTop = arithop(tokentype);
    ltemp = modify_type(left, right->type, ASTop);
    rtemp = modify_type(right, left->type, ASTop);
    if (ltemp == NULL && rtemp == NULL)
      fatal("Incompatible types in binary expression");
    if (ltemp != NULL)
      left = ltemp;
    if (rtemp != NULL)
      right = rtemp;

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
