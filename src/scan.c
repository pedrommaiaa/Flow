#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Lexical scanning

// Return the position of character c
// in string s, or -1 if c not found
static int chrpos(char *s, int c) 
{
  char *p;

  p = strchr(s, c);
  return (p ? p - s : -1);
}

// Get the next character from the input file.
static int next(void) 
{
  int c;

  if (Putback) 
  {		              // Use the character put
    c = Putback;		// back if there is one
    Putback = 0;
    return c;
  }

  c = fgetc(Infile);		// Read from input file
  if (c == '\n')
  { 
    Line++;			// Increment line count
  }
  return c;
}

// Put back an unwanted character
static void putback(int c) 
{
  Putback = c;
}

// Skip past input that we don't need to deal with, 
// i.e. whitespace, newlines. Return the first
// character we do need to deal with.
static int skip(void) 
{
  int c;

  c = next();
  while (c == ' ' ||c == '\t' ||c == '\n' ||c == '\r' ||c == '\f') 
  {
    c = next();
  }
  return (c);
}

// Scan and return an integer literal
// value from the input file. Store
// the value as a string in Text.
static int scanint(int c) 
{
  int k, val = 0;

  // Convert each character into an int value
  while ((k = chrpos("0123456789", c)) >= 0) 
  {
    val = val * 10 + k;
    c = next();
  }

  // We hit a non-integer character, put it back.
  putback(c);
  return val;
}

// Scan and return the next token found in the input.
// Return 1 if token valid, 0 if no tokens left.
int scan(token_T *t) 
{
  int c;

  // Skip whitespace
  c = skip();

  // Determine the token based on
  // the input character
  switch (c) 
  {
    case EOF: t->token = EOF_T; return (0);
    case '+': t->token = PLUS_T; break;
    case '-': t->token = MINUS_T; break;
    case '*': t->token = STAR_T; break;
    case '/': t->token = SLASH_T; break;
    default:

      // If it's a digit, scan the
      // literal integer value in
      if (isdigit(c)) {
        t->intvalue = scanint(c);
        t->token = INTLIT_T;
        break;
      }

      printf("[Line %d] Unrecognised character %c\n", Line, c);
      exit(1);
  }

  // We found a token
  return (1);
}
