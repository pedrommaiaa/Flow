#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of statements

// Prototypes
static AST_T *single_statement(void);


static AST_T *print_statement(void)
{
  AST_T *tree;
  int reg;

  // Match 'print' as the first token
  match(PRINT_T, "print");
  lparen();

  // Parse the following expression
  tree = binexpr(0);
  rparen();

  // Make an print AST tree
  tree = mkastunary(PRINT_A, tree, 0);

  // Return the AST
  return (tree);
}

static AST_T *assignment_statement(void)
{
  AST_T *left, *right, *tree;
  int id;

  // Ensure we have an identifier
  ident();

  // Check it's been defined then make a leaf node for it
  if ((id = findglob(Text)) == -1)
  {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(LVIDENT_A, id);

  // Ensure we have an equals sign
  match(ASSIGN_T, "=");

  // Parse the following expression
  left = binexpr(0);

  // Make an assignment AST tree
  tree = mkastnode(ASSIGN_A, left, NULL, right, 0);

  // Return the AST
  return (tree);
} 


// Parse an IF statement including
// any optional ELSE clause
// and return its AST
AST_T *if_statement(void)
{
  AST_T *condAST, *trueAST, *falseAST = NULL;

  // Ensure we have 'if' '('
  match(IF_T, "if");
  lparen();


  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison
  condAST = binexpr(0);

  if (condAST->op < EQUAL_A || condAST->op > GREATER_OR_EQUAL_A)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  trueAST = compound_statement();

  // If we have an 'else', skip it
  // and get the AST for the compound statement
  if (Token.token == ELSE_T)
  {
    scan(&Token);
    falseAST = compound_statement();
  } 
  // Build and return the AST for this statement
  return (mkastnode(IF_A, condAST, trueAST, falseAST, 0));
}


// Parse a WHILE statement
// and return its AST
AST_T *while_statement(void)
{
  AST_T *condAST, *bodyAST;

  // Ensure we have 'while' '('
  match(WHILE_T, "while");
  lparen();


  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);
  if (condAST->op < EQUAL_A || condAST->op > GREATER_OR_EQUAL_A)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  bodyAST = compound_statement();

  // Build and return the AST for this statement
  return (mkastnode(WHILE_A, condAST, NULL, bodyAST, 0));
}


// Parse a FOR statement
// and return its AST
static AST_T *for_statement(void)
{
  AST_T *condAST, *bodyAST;
  AST_T *preopAST, *postopAST;
  AST_T *tree;

  // Ensure we have 'for' '('
  match(FOR_T, "for");
  lparen();

  // Get the pre_op statement and the ';'
  preopAST = single_statement();
  semi();

  // Get the condition and the ';'
  condAST = binexpr(0);
  if (condAST->op < EQUAL_A || condAST->op > GREATER_OR_EQUAL_A)
    fatal("Bad comparison operator");
  semi();

  // Get the post_op statemetn and the ')'
  postopAST = single_statement();
  rparen();

  // Get the compound statement which is the body
  bodyAST = compound_statement();

  // For now, all four sub-trees have to be non-NULL.
  // Later on, I'll change the semantics for when some are missing
  
  // Glue the compound statement and the postop tree
  tree = mkastnode(GLUE_A, bodyAST, NULL, postopAST, 0);

  // Make a WHILE loop with the condition and this new body
  tree = mkastnode(WHILE_A, condAST, NULL, tree, 0);

  // And glue the preop tree to the WHILE_A tree
  return (mkastnode(GLUE_A, preopAST, NULL, tree, 0));
}


// Parse a single statement
// and return its AST
static AST_T *single_statement(void)
{
  switch(Token.token)
  {
    case PRINT_T: return (print_statement());
    case INT_T: var_declaration(); return (NULL); // No AST generated here
    case IDENT_T: return (assignment_statement());
    case IF_T: return (if_statement());
    case WHILE_T: return (while_statement());
    case FOR_T: return (for_statement());
    default: fatald("Syntax error, token", Token.token);
  }
}

// Parse a compound statement
// and return its AST
AST_T *compound_statement(void) 
{
  AST_T *left = NULL;
  AST_T *tree;

  // Require a left curly bracket
  lbrace();

  while (1) 
  {
    // Parse a single statement
    tree = single_statement();

    // Some statements must be followed by a semicolon
    if (tree != NULL && (tree->op == PRINT_A || tree->op == ASSIGN_A))
      semi();

    // For each new tree, either save it in left
    // if left is empty, or glue the left and the
    // new tree together
    if (tree != NULL) 
    {
      if (left == NULL)
	      left = tree;
      else
	      left = mkastnode(GLUE_A, left, NULL, tree, 0);
    }
    // When we hit a right curly bracket,
    // skip past it and return the AST
    if (Token.token == RBRACE_T) 
    {
      rbrace();
      return (left);
    }
  }
}
