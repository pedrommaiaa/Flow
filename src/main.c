#include "include/lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char** argv[])
{
  lexer_T* lexer = init_lexer("int number = 42;"
                              "print(number);"); 
  token_T* token = 0;

  while ((token = next_token(lexer))->type != TOKEN_EOF)
  {    
    const char* kind_str = token_kind_to_str(token->type);
    printf("%s -> %s\n", kind_str, token->value);
  } 
  
  return 0;
}
