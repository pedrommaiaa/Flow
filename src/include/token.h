typedef struct TOKEN
{
  char* value;
  enum
  {
    TOKEN_ID,
    TOKEN_EQUAL,
    TOKEN_INT,
    TOKEN_SEMI,
    TOKEN_EOF
  } type;
} token_T;


token_T* init_token(char* value, int type);
