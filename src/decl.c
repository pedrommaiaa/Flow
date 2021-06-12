#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"


// Parsing of declarations


// Parse the declaration of a variable
void var_declaration(void)
{
  // Ensure we have an 'int' token followed by an identifier
  // and a semicolon. Text now has the identifier's name.
  // Add it as a known identifier.
  match(INT_T, "int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}

// For now we have a very simplistic function definition grammar
//
// function_declaration: 'void' identifier '(' ')' compound_statement
//      ;
//Parse the declaration of a simplistic function
AST_T *function_declaration(void)
{
  AST_T *tree;
  int nameslot;

  // Find the 'void', the identifier, and the '(' ')'.
  // For now, do nothing with them
  match(VOID_T, "void");
  ident();
  nameslot = addglob(Text);
  lparen();
  rparen();

  // Get the AST tree for the compound statement
  tree = compound_statement();

  // Return an FUNCTION_A node wich has the function's nameslot
  // and the compound statemetn sub-tree
  return (mkastunary(FUNCTION_A, tree, nameslot));
}
