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
