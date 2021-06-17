#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of statements


// Prototypes
static AST_T *single_statement(void);


// Parse an IF statement including any
// optional ELSE clause and return its AST
static AST_T *if_statement(void) {
  AST_T *condAST, *trueAST, *falseAST = NULL;

  // Ensure we have 'if' '('
  match(T_IF, "if");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  trueAST = compound_statement();

  // If we have an 'else', skip it
  // and get the AST for the compound statement
  if (Token.token == T_ELSE) {
    scan(&Token);
    falseAST = compound_statement();
  }
  // Build and return the AST for this statement
  return (mkastnode(A_IF, P_NONE, condAST, trueAST, falseAST, 0));
}


// Parse a WHILE statement and return its AST
static AST_T *while_statement(void) {
  AST_T *condAST, *bodyAST;

  // Ensure we have 'while' '('
  match(T_WHILE, "while");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  bodyAST = compound_statement();

  // Build and return the AST for this statement
  return (mkastnode(A_WHILE, P_NONE, condAST, NULL, bodyAST, 0));
}


// Parse a FOR statement and return its AST
static AST_T *for_statement(void) {
  AST_T *condAST, *bodyAST;
  AST_T *preopAST, *postopAST;
  AST_T *tree;

  // Ensure we have 'for' '('
  match(T_FOR, "for");
  lparen();

  // Get the pre_op statement and the ';'
  preopAST = single_statement();
  semi();

  // Get the condition and the ';'
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  semi();

  // Get the post_op statement and the ')'
  postopAST = single_statement();
  rparen();

  // Get the compound statement which is the body
  bodyAST = compound_statement();

  // For now, all four sub-trees have to be non-NULL.
  // Later on, we'll change the semantics for when some are missing

  // Glue the compound statement and the postop tree
  tree = mkastnode(A_GLUE, P_NONE, bodyAST, NULL, postopAST, 0);

  // Make a WHILE loop with the condition and this new body
  tree = mkastnode(A_WHILE, P_NONE, condAST, NULL, tree, 0);

  // And glue the preop tree to the A_WHILE tree
  return (mkastnode(A_GLUE, P_NONE, preopAST, NULL, tree, 0));
}


// Parse a return statement and return its AST
static AST_T *return_statement(void) {
  AST_T *tree;

  // Can't return a value if function returns P_VOID
  if (Gsym[Functionid].type == P_VOID)
    fatal("Can't return from a void function");

  // Ensure we have 'return' '('
  match(T_RETURN, "return");
  lparen();

  // Parse the following expression
  tree = binexpr(0);

  // Ensure this is compatible with the function's type
  tree = modify_type(tree, Gsym[Functionid].type, 0);
  if (tree == NULL)
    fatal("Incompatible type to return");

  // Add on the A_RETURN node
  tree = mkastunary(A_RETURN, P_NONE, tree, 0);

  // Get the ')'
  rparen();
  return (tree);
}

// Parse a single statement and return its AST
static AST_T *single_statement(void) {
  int type;

  switch (Token.token) {
    case T_CHAR:
    case T_INT:
    case T_LONG:

      // The beginning of a variable declaration.
      // Parse the type and get the identifier.
      // Then parse the rest of the declaration.
      // XXX: These are globals at present.
      type = parse_type();
      ident();
      var_declaration(type);
      return (NULL);		// No AST generated here
    case T_IF: return (if_statement());
    case T_WHILE: return (while_statement());
    case T_FOR: return (for_statement());
    case T_RETURN: return (return_statement());
    default:
      // For now, see if this is an expression.
      // This catches assignment statements.
      return (binexpr(0));
  }
  return (NULL);		// Keep -Wall happy
}

// Parse a compound statement
// and return its AST
AST_T *compound_statement(void) {
  AST_T *left = NULL;
  AST_T *tree;

  // Require a left curly bracket
  lbrace();

  while (1) {
    // Parse a single statement
    tree = single_statement();

    // Some statements must be followed by a semicolon
    if (tree != NULL && (tree->op == A_ASSIGN ||
			 tree->op == A_RETURN || tree->op == A_FUNCCALL))
      semi();

    // For each new tree, either save it in left
    // if left is empty, or glue the left and the
    // new tree together
    if (tree != NULL) {
      if (left == NULL)
	left = tree;
      else
	left = mkastnode(A_GLUE, P_NONE, left, NULL, tree, 0);
    }

    // When we hit a right curly bracket,
    // skip past it and return the AST
    if (Token.token == T_RBRACE) {
      rbrace();
      return (left);
    }
  }
}
