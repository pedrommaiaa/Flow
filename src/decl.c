#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"


// Parsing of declarations

// Parse the current token and
// return a primitive type enum value
int parse_type(int t)
{
  if (CHAR_T == t) return (CHAR_P);
  if (INT_T == t)  return (INT_P);
  if (VOID_T == t) return (VOID_P);
  fatald("Ilegal type, token", t);
}


// Parse the declaration of a variable
void var_declaration(void)
{
  int id, type;
  type = parse_type(Token.token);
  scan(&Token);
  ident();
  // Text now has the identifier's name.
  // Add it as a known identifier
  // and generate its space in assembly
  id = addglob(Text, type, VARIABLE_S);
  genglobsym(id);
  // Get the trailing semicolon 
  semi();
}

// For now we have a very simplistic function definition grammar
// Parse the declaration of a simplistic function
AST_T *function_declaration(void)
{
  AST_T *tree;
  int nameslot;

  // Find the 'void', the identifier, and the '(' ')'.
  // For now, do nothing with them
  match(VOID_T, "void");
  ident();
  nameslot = addglob(Text, VOID_P, FUNCTION_S);
  lparen();
  rparen();

  // Get the AST tree for the compound statement
  tree = compound_statement();

  // Return an FUNCTION_A node wich has the function's nameslot
  // and the compound statemetn sub-tree
  return (mkastunary(FUNCTION_A, VOID_P, tree, nameslot));
}
