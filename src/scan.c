#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Lexical scanning

// Get the next character from the input file.
static int next(void) 
{
  int c;

  if (Putback) 
  {		                  // Use the character put
    c = Putback;		    // back if there is one
    Putback = 0;
    return c;
  }

  c = fgetc(Infile);		// Read from input file
  if (c == '\n') 
    Line++;			        // Increment line count
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
  int val = c - '0';
  // printf("val: %d\n", val);
  int k = (c = next());
  // Convert c from ASCII digit to integer.
  while (k >= 0 && k <= 9) 
  {
    val = val * 10 + (c - '0');
    // printf("val: %d\n", val);
    c = next();
  }

  // We hit a non-integer character, put it back.
  putback(c);
  return (val);
}

// Scan an identifier from the input file and
// store it in buf[]. Return the identifier's length
static int scanident(int c, char *buf, int lim)
{
  int i = 0;

  // Allow digits, alpha and underscorees
  while (isalpha(c) || isdigit(c) || c == '_')
  {
    // Error if we hit the identifier length limit,
    // else append to buf[] and get next character
    if (lim - 1 == i)
    {
      fatal("Identifier too long");
    }
    else if (i < lim - 1)
    {
      buf[i++] = c;
    }
    c = next();
  }
  // We hit a non-valid character, put it back.
  // NUL-terminate the buf[] and return the length
  putback(c);
  buf[i] = '\0';
  return (i);
}


// Given a word from the input, return the matching
// keyword token number or 0 if it's not a keyword.
// Switch on the first letter so that we don't have
// to wate time 'strcmp()ing' against all the keywords.
static int keyword(char *s)
{
  switch (*s)
  {
    case 'i':
      if (!strcmp(s, "int"))
      {
        return INT_T;
      }
      break;
    case 'p':
      if (!strcmp(s, "print"))
      {
        return PRINT_T;
      }
      break;
  }
  return (0);
}



// Scan and return the next token found in the input.
// Return 1 if token valid, 0 if no tokens left.
int scan(token_T *t) 
{
  int c, tokentype;

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
    case ';': t->token = SEMI_T; break;
    case '=': if ((c = next()) == '=') t->token = EQUAL_T; else t->token = ASSIGN_T; break;
    case '!': if ((c = next()) == '=') t->token = NOT_EQUAL_T; else fatalc("Unrecognised character", c); break;
    case '<': if ((c = next()) == '=') t->token = LESS_OR_EQUAL_T; else t->token = LESS_THAN_T; break;
    case '>': if ((c = next()) == '=') t->token = GREATER_OR_EQUAL_T; else t->token = GREATER_THAN_T; break;
    default:

      // If it's a digit, scan the
      // literal integer value in
      if (isdigit(c)) 
      {
        t->intvalue = scanint(c);
        t->token = INTLIT_T;
        break;
      }
      else if (isalpha(c) || c == '_')
      {
        // Read in a keyword or identifier
        scanident(c, Text, TEXTLEN);

        // If it's a recognised keyword, return that token
        if (tokentype = keyword(Text))
        {
          t->token = tokentype; 
          break;
        }
        // Not a recognised keyword, so it must be an identifier
        t->token = IDENT_T;
        break;
      }
      // The character isn't part of any recognised token, error
      fatalc("Unrecognised character", c);
  }

  // We found a token
  return (1);
}
