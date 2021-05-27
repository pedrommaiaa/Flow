#include "include/token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


token_T* init_token(char* value, int type)
{
  token_T* token = calloc(1, sizeof(struct TOKEN));
  token->value = value;
  token->type = type;

  return token;
}


int main(int argc, char** argv[])
{
  char code[] = "int number = 42;";
  int len = strlen(code);
  int i = 0;
  token_T* token = 0;

  while (i < len)
  {
    if (code[i] == ' ')
    {
      i++;
    }
    
    if (isalpha(code[i]))
    {
      char* value = calloc(1, sizeof(char));
      while (isalpha(code[i]))
      {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){code[i], 0});
        i++;
      }
      token = init_token(value, TOKEN_ID);
      printf("Value: %s, Type: %d\n", token->value, token->type);
    }
       
    if (isdigit(code[i]))
    {
      char* value = calloc(1, sizeof(char));
      
      while (isdigit(code[i]))
      {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){code[i], 0});
        i++;
      }
      token = init_token(value, TOKEN_INT);
      printf("Value: %s, Type: %d\n", token->value, token->type);
    }
    
    if (code[i] == ';')
    {
      char* value = calloc(2, sizeof(char));
      value[0] = code[i];
      value[1] = '\0'; 
      token = init_token(value, TOKEN_SEMI);
      printf("Value: %s, Type: %d\n", token->value, token->type);
      i++;
    }
    
    if (code[i] == '=')
    {
      char* value = calloc(2, sizeof(char));
      value[0] = code[i];
      value[1] = '\0'; 
      token = init_token(value, TOKEN_EQUAL);
      printf("Value: %s, Type: %d\n", token->value, token->type);
      i++;
    }
    
    else
    {
      i++;
    }
  } 
  
  return 0;
}
