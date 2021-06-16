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
    fatals("Expected", what);
  }
}

// Match a semicolon and fetch the next token
void semi(void) 
{
  match(SEMI_T, ";"); 
}

// Match a left brace and fetch the next token
void lbrace(void) 
{
  match(LBRACE_T, "{");
}

// Match a right brace and fetch the next token
void rbrace(void) 
{
  match(RBRACE_T, "}");
}

// Match a left parenthesis and fetch the next token
void lparen(void) 
{
  match(LPAREN_T, "(");
}

// Match a right parenthesis and fetch the next token
void rparen(void) 
{
  match(RPAREN_T, ")");
}

// Match an identifier and fetch the next token
void ident(void) 
{ 
  match(IDENT_T, "identifier");
}

// Print out fatal messages
void fatal(char *s)
{
  fprintf(stderr, "[Line %d] %s\n", Line, s); exit(1);
}

void fatals(char *s1, char *s2)
{
  fprintf(stderr, "[Line %d] %s:%s\n", Line, s1, s2); exit(1);
}

void fatald(char *s, int d)
{
  fprintf(stderr, "[Line %d] %s:%d\n", Line, s, d); exit(1);
}

void fatalc(char *s, int c)
{
  fprintf(stderr, "[Line %d] %s:%c\n", Line, s, c); exit(1);
}
