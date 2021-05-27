#include "include/lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char** argv[])
{
  lexer_T* lexer = init_lexer("int number = 42;");
  
  token_T* token = 0;

  while ((token = next_token(lexer))->type != TOKEN_EOF)
  {
    
    printf("TOKEN(%d, %s)\n", token->type, token->value);
    /*    
    if (code[i] == '(')
    {
      char* value = calloc(2, sizeof(char));
      value[0] = code[i];
      value[1] = '\0'; 
      token = init_token(value, TOKEN_SEMI);
      printf("Value: %s, Type: %d\n", token->value, token->type);
      i++;
    }
    
    if (code[i] == ')')
    {
      char* value = calloc(2, sizeof(char));
      value[0] = code[i];
      value[1] = '\0'; 
      token = init_token(value, TOKEN_EQUAL);
      printf("Value: %s, Type: %d\n", token->value, token->type);
      i++;
    }
    */
  } 
  
  return 0;
}
