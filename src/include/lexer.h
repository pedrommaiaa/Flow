#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

typedef struct TOKEN
{
  char* value;
  enum
  {
    TOKEN_ID,
    TOKEN_EQUAL,
    TOKEN_INT,
    TOKEN_STRING,
    TOKEN_SEMI,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_EOF
  } type;
} token_T;


typedef struct LEXER
{
  char* code; // all the code
  size_t len_code; // len of the code
  
  char current_char; // current character
  unsigned int char_index; // current index
  unsigned int line_n; // line number
} lexer_T;


token_T* init_token(char* value, int type);
const char* token_kind_to_str(int type);
char* token_to_str(token_T* token, lexer_T* lexer);
void token_free(token_T* token);


lexer_T* init_lexer(char* code);
void lexer_free(lexer_T* lexer);
void advance(lexer_T* lexer);
void expect_char(lexer_T* lexer, char c);
char* current_char_as_str(lexer_T* lexer);
token_T* lexer_number(lexer_T* lexer);
token_T* lexer_id(lexer_T* lexer);
token_T* lexer_string(lexer_T* lexer);
token_T* current_character(lexer_T* lexer, int type);
token_T* next_token(lexer_T* lexer);
#endif
