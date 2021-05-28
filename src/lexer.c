#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


token_T* init_token(char* value, int type)
{
  token_T* token = calloc(1, sizeof(struct TOKEN));
  token->value = value;
  token->type = type;

  return token;
}

lexer_T* init_lexer(char* code)
{
  lexer_T* lexer = calloc(1, sizeof(struct LEXER));
  lexer->code = code; // raw code
  lexer->len_code = strlen(code); // len code
  lexer->i = 0; // index
  lexer->c = code[lexer->i]; // current character

  return lexer; 
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


token_T* lexer_string(lexer_T* lexer)
{
  char* value = calloc(1, sizeof(char));
  advance(lexer);

  while (lexer->c != '"')
  {
    value = realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, (char[]){lexer->c, 0});
    advance(lexer);
  }
  
  advance(lexer);
  char* formatted = calloc(1, sizeof(char));
  unsigned int len = strlen(value);

  unsigned int i = 0;
  while (value[i] != '\0' && i < len)
  {
    formatted = realloc(formatted, (strlen(formatted) + 2) * sizeof(char));
    strcat(formatted, (char[]){value[i], 0});
    i += 1;
  }
  free(value);

  return init_token(formatted, TOKEN_STRING);
}


token_T* current_character(lexer_T* lexer, int type)
{
  char* value = calloc(2, sizeof(char));
  value[0] = lexer->c;
  value[1] = '\0'; 
  token_T* token = init_token(value, type);
  advance(lexer);

  return token;
}


token_T* next_token(lexer_T* lexer)
{
  while (lexer->c != '\0')
  {
    if (lexer->c == ' ' || lexer->c == '\n')
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
    
    switch (lexer->c)
    {
      case '=': return current_character(lexer, TOKEN_EQUAL);
      case ';': return current_character(lexer, TOKEN_SEMI);
      case '(': return current_character(lexer, TOKEN_LPAREN);
      case ')': return current_character(lexer, TOKEN_RPAREN);
      case '"': return lexer_string(lexer);
      case '\0': break;
      default: printf("[Lexer]: Unexpected character '%c' (%d)\n", lexer->c, (int)lexer->c); exit(1); break;
    }
  }

  return init_token(0, TOKEN_EOF);
}


const char* token_kind_to_str(int type)
{
  switch (type)
  {
    case TOKEN_ID: return "TOKEN_ID";
    case TOKEN_EQUAL: return "TOKEN_EQUAL";
    case TOKEN_INT: return "TOKEN_INT";
    case TOKEN_STRING: return "TOKEN_STRING";
    case TOKEN_SEMI: return "TOKEN_SEMI";
    case TOKEN_LPAREN: return "TOKEN_LPAREN";
    case TOKEN_RPAREN: return "TOKEN_RPAREN";
    case TOKEN_EOF: return "TOKEN_EOF";
  }

  return "Token kind not stringable.\n";
}
