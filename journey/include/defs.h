
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Token structure
struct token {
  int token;
  int intvalue;
};

// Tokens
enum {
  PLUS_T, MINUS_T, MUL_T, DIV_T, INT_T
};
