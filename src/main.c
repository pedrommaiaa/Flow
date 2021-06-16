#include "include/defs.h"
#define extern_
#include "include/data.h"
#undef extern_
#include "include/decl.h"
#include <errno.h>

// Compiler setup and top-level execution

// Initialise global variables
static void init() 
{
  Line = 1;
  Putback = '\n';
}

// Print out a usage if started incorrectly
static void usage(char *prog) {
  fprintf(stderr, "Usage: %s infile\n", prog);
  exit(1);
}

// Main program: check arguments and print a usage
// if we don't have an argument. Open up the input
// file and call scanfile() to scan the tokens in it.
void main(int argc, char *argv[]) 
{
  AST_T *tree;

  if (argc != 2)
    usage(argv[0]);

  init();

  // Open up the input file
  if ((Infile = fopen(argv[1], "r")) == NULL) 
  {
    fprintf(stderr, "[ERROR] Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  
  // Create the output file
  if ((Outfile = fopen("out.s", "w")) == NULL) 
  {
    fprintf(stderr, "[ERROR] Unable to create out.s: %s\n", strerror(errno));
    exit(1);
  }

  // For now, ensure that void printint() is defined
  addglob("printint", CHAR_P, FUNCTION_S, 0);
  
  scan(&Token);			                 // Get the first token from the input
  genpreamble();                     // Output the preamble
  while (1)                          
  {
    tree = function_declaration();   // Parse a function and
    genAST(tree, NOREG, 0);          // Generate the assembly code for it
    if (Token.token == EOF_T)        // Stop when we have reached EOF
      break;
  } 
  fclose(Outfile);                   // Close the output file and exit
  exit(0);
}
