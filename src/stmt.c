#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of statements

// compound_statement:          // empty, i.e. no statement
//      |      statement
//      |      statement statements
//      ;
//
// statement: print_statement
//      |     declaration
//      |     assignment_statement
//      |     if_statement
//      ;
//
// print_statement: 'print' expression ';'  ;
//
// declaration: 'int' identifier ';'  ;
//
// assignment_statement: identifier '=' expression ';'   ;
//
// if_statement: if_head
//      |        if_head 'else' compound_statement
//      ;
//
// if_head: 'if' '(' true_false_expression ')' compound_statement  ;
//
// identifier: T_IDENT ;


static AST_T *print_statement(void)
{
  AST_T *tree;
  int reg;

  // Match 'print' as the first token
  match(PRINT_T, "print");

  // Parse the following expression
  tree = binexpr(0);

  // Make an print AST tree
  tree = mkastunary(PRINT_A, tree, 0);

  // Match the following semiclon
  semi();
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

  // Match the following semicolon
  // and return the AST
  semi();
  return (tree);
} 
  
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
    switch (Token.token)
    {
      case PRINT_T: tree = print_statement(); break;
      case INT_T: var_declaration(); tree = NULL; break;
      case IDENT_T: tree = assignment_statement(); break;
      case IF_T: tree = if_statement(); break;
      case RBRACE_T: rbrace(); return (left);
      default: fatald("Syntax error, token", Token.token); 
    }
    // For each new tree, either save it in left
    // if left is empty, or glue the left and the
    // new tree together
    if (tree)
    {
      if (left == NULL)
        left = tree;
      else
        left = mkastnode(GLUE_A, left, NULL, tree, 0);
    }
  }
}
