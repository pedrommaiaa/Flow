#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"


// Parsing of declarations

// Parse the current token and
// return a primitive type enum value
int parse_type(int t) {
  if (t == CHAR_T) return (CHAR_P);
  if (t == INT_T)  return (INT_P);
  if (t == LONG_T) return (LONG_P);
  if (t == VOID_T) return (VOID_P);
  fatald("Illegal type, token", t);
}

// Parse the declaration of a variable
void var_declaration(void) {
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

// Parse the declaration of a simplistic function
struct ASTnode *function_declaration(void) {
  struct ASTnode *tree, *finalstmt;
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

  // Get the AST tree for the compound statement
  tree = compound_statement();

  // If the function type isn't P_VOID, check that
  // the last AST operation in the compound statement
  // was a return statement
  if (type != VOID_P) {
    finalstmt = (tree->op == GLUE_A) ? tree->right : tree;
    if (finalstmt == NULL || finalstmt->op != RETURN_A)
      fatal("No return for function with non-void type");
  }
  // Return an A_FUNCTION node which has the function's nameslot
  // and the compound statement sub-tree
  return (mkastunary(FUNCTION_A, type, tree, nameslot));
}
