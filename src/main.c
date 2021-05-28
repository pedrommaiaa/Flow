#include "include/lexer.h"
#include "include/io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "\033[1;31mPease specify input file.\033[0m\n]");
    return 1;
  }
  
  char* contents = read_file(argv[1]);
  
  lexer_T* lexer = init_lexer(contents); 
  
  token_T* token = 0;

  while ((token = next_token(lexer))->type != TOKEN_EOF)
  {    
    printf("%s", token_to_str(token));
  }

  free(contents); 
  
  return 0;
}
