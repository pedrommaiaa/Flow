#include "include/defs.h"
#define extern_
#include "include/data.h"
#undef extern_
#include "include/decl.h"
#include <errno.h>


// Initialise global variables
static void init() 
{
  Line = 1;
  Putback = '\n';
}


// List of printable tokens, needs to be in the same order 
char *tokstr[] = { "+", "-", "*", "/", "int", ";", "print", NULL };

// Loop scanning in all the tokens in the input file.
// Print out details of each token found.
static void scanfile() 
{
  struct token T;

  while (scan(&T)) {
    printf("Token -> %s", tokstr[T.token]);
    if (T.token == INT_T)
      printf(", value -> %d", T.intvalue);
    printf("\n");
  }
}

// Main program: check arguments and print a usage
// if we don't have an argument. Open up the input
// file and call scanfile() to scan the tokens in it.
void main(int argc, char *argv[]) 
{
  if (argc != 2)
  {
    exit(1); 
  }

  init();

  if ((Infile = fopen(argv[1], "r")) == NULL) 
  {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  scanfile();
  exit(0);
}
