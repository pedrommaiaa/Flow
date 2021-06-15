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
static int scanint(char c) 
{
  int val = 0;
  int k = c - '0';
  if (k > 0)
  {
    // Convert c from ASCII digit to integer.
    while (k >= 0 && k <= 9)
    {
      val = val*10 + k;
      c = next();
      k = c - '0';
    }  
  }
  else 
  {
    // Implement hex here
  }

  // We hit a non-integer character, put it back.
  putback(c);
  return (val);
}

// Scan an identifier from the input file and
// store it in buf[]. Return the identifier's length
static int scanident(int c, char *buf, int lim) {
  int i = 0;

  // Allow digits, alpha and underscores
  while (isalpha(c) || isdigit(c) || '_' == c) {
    // Error if we hit the identifier length limit,
    // else append to buf[] and get next character
    if (lim - 1 == i) {
      fatal("Identifier too long");
    } else if (i < lim - 1) {
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
// to waste time strcmp()ing against all the keywords.
static int keyword(char *s) {
  switch (*s) {
    case 'c':
      if (!strcmp(s, "char"))
	return (CHAR_T);
      break;
    case 'e':
      if (!strcmp(s, "else"))
	return (ELSE_T);
      break;
    case 'f':
      if (!strcmp(s, "for"))
	return (FOR_T);
      break;
    case 'i':
      if (!strcmp(s, "if"))
	return (IF_T);
      if (!strcmp(s, "int"))
	return (INT_T);
      break;
    case 'l':
      if (!strcmp(s, "long"))
	return (LONG_T);
      break;
    case 'p':
      if (!strcmp(s, "print"))
	return (PRINT_T);
      break;
    case 'r':
      if (!strcmp(s, "return"))
	return (RETURN_T);
      break;
    case 'w':
      if (!strcmp(s, "while"))
	return (WHILE_T);
      break;
    case 'v':
      if (!strcmp(s, "void"))
	return (VOID_T);
      break;
  }
  return (0);
}

// A pointer to a rejected token
static struct token *Rejtoken = NULL;

// Reject the token that we just scanned
void reject_token(struct token *t) {
  if (Rejtoken != NULL)
    fatal("Can't reject token twice");
  Rejtoken = t;
}

// Scan and return the next token found in the input.
// Return 1 if token valid, 0 if no tokens left.
int scan(struct token *t) {
  int c, tokentype;

  // If we have any rejected token, return it
  if (Rejtoken != NULL) {
    t = Rejtoken;
    Rejtoken = NULL;
    return (1);
  }
  // Skip whitespace
  c = skip();

  // Determine the token based on
  // the input character
  switch (c) {
    case EOF:
      t->token = EOF_T;
      return (0);
    case '+':
      t->token = PLUS_T;
      break;
    case '-':
      t->token = MINUS_T;
      break;
    case '*':
      t->token = STAR_T;
      break;
    case '/':
      t->token = SLASH_T;
      break;
    case ';':
      t->token = SEMI_T;
      break;
    case '{':
      t->token = LBRACE_T;
      break;
    case '}':
      t->token = RBRACE_T;
      break;
    case '(':
      t->token = LPAREN_T;
      break;
    case ')':
      t->token = RPAREN_T;
      break;
    case '=':
      if ((c = next()) == '=') {
	t->token = EQUAL_T;
      } else {
	putback(c);
	t->token = ASSIGN_T;
      }
      break;
    case '!':
      if ((c = next()) == '=') {
	t->token = NOT_EQUAL_T;
      } else {
	fatalc("Unrecognised character", c);
      }
      break;
    case '<':
      if ((c = next()) == '=') {
	t->token = LESS_OR_EQUAL_T;
      } else {
	putback(c);
	t->token = LESS_OR_EQUAL_T;
      }
      break;
    case '>':
      if ((c = next()) == '=') {
	t->token = GREATER_OR_EQUAL_T;
      } else {
	putback(c);
	t->token = GREATER_THAN_T;
      }
      break;
    default:

      // If it's a digit, scan the
      // literal integer value in
      if (isdigit(c)) {
	t->intvalue = scanint(c);
	t->token = INTLIT_T;
	break;
      } else if (isalpha(c) || '_' == c) {
	// Read in a keyword or identifier
	scanident(c, Text, TEXTLEN);

	// If it's a recognised keyword, return that token
	if (tokentype = keyword(Text)) {
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