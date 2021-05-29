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
  
  lexer->char_index = 0; // index
  lexer->line_n = 1; // index
  lexer->current_char = lexer->code[lexer->char_index]; // current character

  return lexer; 
}

void token_free(token_T* token)
{
  free(token->value);
  free(token);
}


void advance(lexer_T* lexer)
{
  if (lexer->current_char == '\n' || lexer->current_char == 10)
  {
    lexer->line_n += 1;
  }
  
  
  if (lexer->char_index < lexer->len_code && lexer->current_char != '\0')
  {
    lexer->char_index += 1;
    lexer->current_char = lexer->code[lexer->char_index];
  }

  else
  {
    lexer->current_char = '\0';
  }
}

void expect_char(lexer_T* lexer, char c)
{
  if (lexer->current_char != c)
  {
    printf("Error: [Line %d] Lexer expected the current char to be '%c', but it was '%c'.", lexer->line_n, c, lexer->current_char);
    exit(1);
  }
}

token_T* lexer_number(lexer_T* lexer)
{

  char* buffer = calloc(1, sizeof(char));
  buffer[0] = '\0';

  while (isdigit(lexer->current_char))
  {
    char* strchar = current_char_as_str(lexer);
    buffer = realloc(buffer, strlen(buffer) + 2);
    strcat(buffer, strchar);
    free(strchar);
     
    advance(lexer);
  }
  return init_token(buffer, TOKEN_INT);
}


token_T* lexer_id(lexer_T* lexer)
{
  char* value = calloc(1, sizeof(char));
  while (isalnum(lexer->current_char) || lexer->current_char == '_')
  {
    value = realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, (char[]){lexer->current_char, 0});
    advance(lexer); 
  }
  return init_token(value, TOKEN_ID); 
}


token_T* lexer_string(lexer_T* lexer)
{
  expect_char(lexer, '"');
  advance(lexer);

  size_t initial_index = lexer->char_index;

  while (lexer->current_char != '"')
  {
    if (lexer->current_char == '\0')
    {
      printf("[Line %d] Missing closing quotation mark\n.", lexer->line_n); exit(1);
    }
    
    advance(lexer);
  }
  
  size_t length = lexer->char_index - initial_index + 1;
  char* buffer = calloc(length, sizeof(char));
  memcpy(buffer, &lexer->code[initial_index], length);
  buffer[length - 1] = '\0';
  
  advance(lexer);
  
  /* format to string */
  char* formatted = calloc(1, sizeof(char));
  unsigned int len = strlen(buffer);

  unsigned int i = 0;
  while (buffer[i] != '\0' && i < len)
  {
    formatted = realloc(formatted, (strlen(formatted) + 2) * sizeof(char));
    strcat(formatted, (char[]){buffer[i], 0});
    i += 1;
  }
  /* end format */
  
  token_T* token = init_token(formatted, TOKEN_STRING);
  free(buffer);

  return token;
}


token_T* current_character(lexer_T* lexer, int type)
{
  char* value = calloc(2, sizeof(char));
  value[0] = lexer->current_char;
  value[1] = '\0'; 
  token_T* token = init_token(value, type);
  advance(lexer);

  return token;
}


token_T* next_token(lexer_T* lexer)
{
  while (lexer->current_char != '\0')
  {
    while (lexer->current_char == ' ' || lexer->current_char == '\n' || lexer->current_char == 10)
    {
      advance(lexer);
    }
    
    if (lexer->current_char == '#')
    {
      advance(lexer);
      while (lexer->current_char != '\n' && lexer->current_char != 10)
      {
        advance(lexer); 
      }
      continue;
    }
    
    if (isdigit(lexer->current_char))
    {
      return lexer_number(lexer);
    }
    
    if (isalnum(lexer->current_char))
    {
      return lexer_id(lexer);
    }

    switch (lexer->current_char)
    {
      case '=': return current_character(lexer, TOKEN_EQUAL);
      case ';': return current_character(lexer, TOKEN_SEMI);
      case '(': return current_character(lexer, TOKEN_LPAREN);
      case ')': return current_character(lexer, TOKEN_RPAREN);
      case '{': return current_character(lexer, TOKEN_LBRACE);
      case '}': return current_character(lexer, TOKEN_RBRACE);
      case '"': return lexer_string(lexer);
      case '\0': break;
      default: printf("[Line %d]: Unexpected character '%c', ASCII number: %d\n", lexer->line_n, lexer->current_char, (int)lexer->current_char); exit(1); break;
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
    case TOKEN_LBRACE: return "TOKEN_LBRACE";
    case TOKEN_RBRACE: return "TOKEN_RBRACE";
    case TOKEN_EOF: return "TOKEN_EOF";
  }

  return "Token kind not stringable.\n";
}

char* token_to_str(token_T* token, lexer_T* lexer)
{
  const char* kind_str = token_kind_to_str(token->type);
  const char* template = " %s -> '%s'  line: (%d)\n";

  char* str = calloc(strlen(kind_str) + strlen(template) + 12, sizeof(char));
  sprintf(str, template, kind_str, token->value, lexer->line_n);

  return str;
}

char* current_char_as_str(lexer_T* lexer)
{
  char* str = calloc(2, sizeof(char));
  str[0] = lexer->current_char;
  str[1] = '\0';

  return str;
}
