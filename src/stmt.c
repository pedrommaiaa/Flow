#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Parsing of statements


// statements: statement
//      | statement statements
//      ;
//
// statement: 'print' expression ';'
//      |     'int'   identifier ';'
//      |     identifier '=' expression ';'
//      ;
//
// identifier: IDENT_T
//      ;


void print_statement(void)
{
  AST_T *tree;
  int reg;

  // Match 'print' as the first token
  match(PRINT_T, "print");

  // Parse the following expression and
  // generate the assembly code
  tree = binexpr(0);
  reg = genAST(tree, -1);
  genprintint(reg);
  genfreeregs();

  // Match the following semiclon
  semi();
}


void assignment_statement(void)
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
  match(EQUALS_T, "=");

  // Parse the following expression
  left = binexpr(0);

  // Make an assignment AST tree
  tree = mkastnode(ASSIGN_A, left, right, 0);

  // Generate the assembly code for the assignment
  genAST(tree, -1);
  genfreeregs();

  // Match the following semicolon
  semi();
} 
  

// Parse one or more statements
void statements(void)
{

  while (1)
  {
    switch (Token.token)
    {
      case PRINT_T: print_statement(); break;
      case INT_T: var_declaration(); break;
      case IDENT_T: assignment_statement(); break;
      case EOF_T: return;
      default: fatald("Syntax error, token", Token.token);
    }
  }
}
