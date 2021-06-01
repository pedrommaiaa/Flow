#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define TEXTLEN 512

// Tokens
enum {
  PLUS_T, MINUS_T, STAR_T, SLASH_T, INT_T, SEMI_T, PRINT_T,
  LPAREN_T, RPAREN_T, LBRACE_T, RBRACE_T
};

// Token structure
struct token {
  int token;
  int intvalue;
};
