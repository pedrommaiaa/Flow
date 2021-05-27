#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


lexer_T* init_lexer(char* code)
{
  lexer_T* lexer = calloc(1, sizeof(struct LEXER));
  lexer->code = code; // raw code
  lexer->len_code = strlen(code); // len code
  lexer->i = 0; // index
  lexer->c = code[lexer->i]; // current character
}


void advance(lexer_T* lexer)
{
  if (lexer->i < lexer->len_code && lexer->c != '\0')
  {
    lexer->i += 1;
    lexer->c = lexer->code[lexer->i];
  }
}


token_T* lexer_number(lexer_T* lexer)
{
  char* value = calloc(1, sizeof(char));
  
  while (isdigit(lexer->c))
  {
    value = realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, (char[]){lexer->c, 0});
    advance(lexer);
  }
  return init_token(value, TOKEN_INT);

}


token_T* lexer_id(lexer_T* lexer)
{
  char* value = calloc(1, sizeof(char));
  while (isalpha(lexer->c))
  {
    value = realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, (char[]){lexer->c, 0});
    advance(lexer); 
  }
  return init_token(value, TOKEN_ID);
}


token_T* next_token(lexer_T* lexer)
{
  while (lexer->c != '\0')
  {
    if (lexer->c == ' ')
    {
      advance(lexer);
    }
    
    if (isalpha(lexer->c))
    {
      return lexer_id(lexer);
    }

    if (isdigit(lexer->c))
    {
      return lexer_number(lexer);
    }
    else
    {
      printf("[Lexer]: Unexpected character '%c'\n", lexer->c);
      advance(lexer);
    }
  }

  return init_token(0, TOKEN_EOF);
}










