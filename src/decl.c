#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"


// Parsing of declarations

// Parse the current token and
// return a primitive type enum value
int parse_type(void) 
{
  int type;
  switch (Token.token)
  {
    case VOID_T: type = VOID_P; break;
    case CHAR_T: type = CHAR_P; break;
    case INT_T:  type = INT_P;  break;
    case LONG_T: type = LONG_P; break;
    default: fatald("Illegal type, token", Token.token);
  }

  // Scan in one or more further '*' tokens
  // and determine the correct pointer type
  while (1)
  {
    scan(&Token);
    if (Token.token != STAR_T)
      break;
    type = pointer_to(type);
  }

  // We leave with the next token already scanned
  return (type);
}

// Parse the declaration of a variable
void var_declaration(void) 
{
  int id, type;

  // Get the type of the variable, 
  // which also scans in the identifier
  type = parse_type();
  ident();

  // Text now has the identifier's name.
  // Add it as a known identifier
  // and generate its space in assembly
  id = addglob(Text, type, VARIABLE_S, 0);
  genglobsym(id);
  // Get the trailing semicolon
  semi();
}

// Parse the declaration of a simplistic function
AST_T *function_declaration(void) 
{
  AST_T *tree, *finalstmt;
  int nameslot, type, endlabel;

  // Get the type of the variable, then the identifier
  type = parse_type();
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

  // Get the AST tree for the compound statement
  tree = compound_statement();

  // If the function type isn't VOID_P...
  if (type != VOID_P) 
  {
    // Error ifno statemetns in the function
    if (tree == NULL)
      fatal("No statements in function with non-void type");

    // Check that the last AST operation in the
    // compound statementwas a return statement
    finalstmt = (tree->op == GLUE_A) ? tree->right : tree;
    if (finalstmt == NULL || finalstmt->op != RETURN_A)
      fatal("No return for function with non-void type");
  }
  // Return an A_FUNCTION node which has the function's nameslot
  // and the compound statement sub-tree
  return (mkastunary(FUNCTION_A, type, tree, nameslot));
}
