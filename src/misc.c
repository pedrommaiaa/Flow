#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Miscellaneous functions



// Ensure that the current token is t,
// and fetch the next token. Otherwise
// throw an error
void match(int t, char *what)
{
  if (t == Token.token)
  {
    scan(&Token);
  }
  else 
  {
    printf("[Line %d] expected '%s'\n", Line, what);
    exit(1);
  }
}


// Match a semicolon and fetch the next token
void semi(void)
{
  match(SEMI_T, ";");
}
