#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of statements

// Prototypes
static AST_T *single_statement(void);


static AST_T *print_statement(void)
{
  AST_T *tree;
  int lefttype, righttype;
  int reg;

  // Match 'print' as the first token
  match(PRINT_T, "print");
  lparen();

  // Parse the following expression
  tree = binexpr(0);
  rparen();

  // Ensure the two types are compatible.
  lefttype = INT_P;
  righttype = tree->type;
  if (!type_compatible(&lefttype, &righttype, 0))
    fatal("Incompatible types");

  // Widen the tree if required.
  if (righttype)
    tree = mkastunary(righttype, INT_P, tree, 0);

  // Make an print AST tree
  tree = mkastunary(PRINT_A, NONE_P, tree, 0);

  // Return the AST
  return (tree);
}

static AST_T *assignment_statement(void)
{
  AST_T *left, *right, *tree;
  int lefttype, righttype;
  int id;

  // Ensure we have an identifier
  ident();

  // This could be a variable or a function call.
  // If next token is '(', it's a function call.
  if (Token.token = LPAREN_T)
    return (funccall());

  // Not a function call, on with an assignment then
  // Check the identifier has benn defined then make a leaf node for it
  // XXX Add structural type test
  if ((id = findglob(Text)) == -1)
  {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(LVIDENT_A, Gsym[id].type, id);

  // Ensure we have an equals sign
  match(ASSIGN_T, "=");

  // Parse the following expression
  left = binexpr(0);

  // Ensure the two types are compatible.
  lefttype = left->type;
  righttype = right->type;
  if (!type_compatible(&lefttype, &righttype, 1))
    fatal("Incompatible types");

  // Widen the left if required.
  if (lefttype)
    left = mkastunary(lefttype, right->type, left, 0);

  // Make an assignment AST tree
  tree = mkastnode(ASSIGN_A, INT_P, left, NULL, right, 0);

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
  return (mkastnode(IF_A, NONE_P, condAST, trueAST, falseAST, 0));
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
  return (mkastnode(WHILE_A, NONE_P, condAST, NULL, bodyAST, 0));
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
  tree = mkastnode(GLUE_A, NONE_P, bodyAST, NULL, postopAST, 0);

  // Make a WHILE loop with the condition and this new body
  tree = mkastnode(WHILE_A, NONE_P, condAST, NULL, tree, 0);

  // And glue the preop tree to the WHILE_A tree
  return (mkastnode(GLUE_A, NONE_P, preopAST, NULL, tree, 0));
}


// Parse a return statement and return its AST
static AST_T *return_statement(void)
{
  AST_T *tree;
  int returntype, functype;

  // Can't return a value if function returns VOID_P
  if (Gsym[Functionid].type == VOID_P)
    fatal("Can't return from a void function");

  // Ensure we have 'return' '('
  match(RETURN_T, "return");
  lparen();

  // Parse the following expression
  tree = binexpr(0);

  // Ensure this is compatible with the function's type
  returntype = tree->type;
  functype = Gsym[Functionid].type;
  if (!type_compatible(&returntype, &functype, 1))
    fatal("Incompatible types");

  // Widen the left if required
  if (returntype)
    tree = mkastunary(returntype, functype, tree, 0);

  // Add on the RETURN_A node
  tree = mkastunary(RETURN_A, NONE_P, tree, 0); 

  // Get the ')'
  rparen();
  return (tree);
}


// Parse a single statement
// and return its AST
static AST_T *single_statement(void)
{
  switch(Token.token)
  {
    case PRINT_T: return (print_statement());
    case CHAR_T:
    case INT_T:
    case LONG_T: var_declaration(); return (NULL); // No AST generated here
    case IDENT_T: return (assignment_statement());
    case IF_T: return (if_statement());
    case WHILE_T: return (while_statement());
    case FOR_T: return (for_statement());
    case RETURN_T: return (return_statement());
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
    if (tree != NULL && (tree->op == PRINT_A || tree->op == ASSIGN_A ||
                         tree->op == RETURN_A || tree->op == FUNCCALL_A))
      semi();

    // For each new tree, either save it in left
    // if left is empty, or glue the left and the
    // new tree together
    if (tree != NULL) 
    {
      if (left == NULL)
	      left = tree;
      else
	      left = mkastnode(GLUE_A, NONE_P, left, NULL, tree, 0);
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
