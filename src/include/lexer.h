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
    TOKEN_EOF
  } type;
} token_T;

typedef struct LEXER
{
  char* code; // all the code
  size_t len_code; // len of the code
  char c; // current character
  unsigned int i; // current index
} lexer_T;


token_T* init_token(char* value, int type);
const char* token_kind_to_str(int type);


lexer_T* init_lexer(char* code);
void lexer_advance(lexer_T* lexer);
token_T* lexer_number(lexer_T* lexer);
token_T* lexer_id(lexer_T* lexer);
token_T* lexer_string(lexer_T* lexer);
token_T* current_character(lexer_T* lexer, int type);
token_T* next_token(lexer_T* lexer);
