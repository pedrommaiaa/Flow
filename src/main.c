#include "include/defs.h"
#define extern_
#include "include/data.h"
#undef extern_
#include "include/decl.h"
#include <errno.h>

// Compiler setup and top-level execution

// Initialise global variables
static void init() {
  Line = 1;
  Putback = '\n';
}


// Main program: check arguments and print a usage
// if we don't have an argument. Open up the input
// file and call scanfile() to scan the tokens in it.
void main(int argc, char *argv[]) {
  AST_T *n;

  if (argc < 2)
  {
    printf("---- * Interactive Flow Shell * ----\n");
    
    printf("---- * Interactive Shell Terminated. * ----\n");
  }

  init();

  // Open up the input file
  if ((Infile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "[ERROR] Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  
  // Create the output file
  if ((Outfile = fopen("out.s", "w")) == NULL) {
    fprintf(stderr, "[ERROR] Unable to create out.s: %s\n", strerror(errno));
    exit(1);
  }

  
  scan(&Token);			// Get the first token from the input
  n = binexpr(0);		// Parse the expression in the file
  printf("%d\n", interpretAST(n));	// Calculate the final result
  generatecode(n);
  
  fclose(Outfile);
  exit(0);
}
