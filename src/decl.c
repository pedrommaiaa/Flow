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
  if (LONG_T == t) return (LONG_P);
  if (VOID_T == t) return (VOID_P);
  fatald("Ilegal type, token", t);
}


// Parse the declaration of a variable
void var_declaration(void)
{
  int id, type;

  // Get the type of the variable, then the identifier
  type = parse_type(Token.token);
  scan(&Token);
  ident();
  // Text now has the identifier's name.
  // Add it as a known identifier
  // and generate its space in assembly
  id = addglob(Text, type, VARIABLE_S, 0);
  genglobsym(id);
  // Get the trailing semicolon 
  semi();
}

// For now we have a very simplistic function definition grammar
// Parse the declaration of a simplistic function
AST_T *function_declaration(void)
{
  AST_T *tree, *finalstmt;
  int nameslot, type, endlabel;

  // Get the type of the variable, then the identifier
  type = parse_type(Token.token);
  scan(&Token);
  ident();

  // Get a label-id for the end label, add the function
  // to the symbol table, and set the Functionid global
  // to the function's symbol-id
  endlabel = genlabel();
  nameslot = addglob(Text, type, FUNCTION_S, endlabel);
  Functionid = nameslot;

  // Scan in the parentheses
  lparen();
  rparen();

  // Get the AST tree for compound statement
  tree = compound_statement();

  // If the function type isn't VOID_P, check that
  // the last AST operation in the compound statement
  // was a return statement
  if (type != VOID_P)
  {
    finalstmt = (tree->op == GLUE_A) ? tree->right : tree;
    if (finalstmt == NULL || finalstmt->op != RETURN_A)
      fatal("No return for function with non-void type");
  }
  // Return an FUNCTION_A node wich has the function's nameslot
  // and the compound statemetn sub-tree
  return (mkastunary(FUNCTION_A, type, tree, nameslot));
}
