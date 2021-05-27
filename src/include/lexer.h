#include "token.h"
#include <stdio.h>


typedef struct LEXER
{
  char* code; // all the code
  size_t len_code; // len of the code
  char c; // current character
  unsigned int i; // current index
} lexer_T;


lexer_T* init_lexer(char* code);

void lexer_advance(lexer_T* lexer);

token_T* lexer_number(lexer_T* lexer);

token_T* lexer_id(lexer_T* lexer);

token_T* next_token(lexer_T* lexer);
