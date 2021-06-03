#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// AST tree interpreter

// List of AST operators
// static char *ASTop[] = { "+", "-", "*", "/" };

// Given an AST, interpret the
// operators in it and return
// a final value.
int interpretAST(AST_T *n) {
  int leftval, rightval;

  // Get the left and right sub-tree values
  if (n->left)
    leftval = interpretAST(n->left);
  if (n->right)
    rightval = interpretAST(n->right);

  /* Debug: Print what we are about to do
  if (n->op == INTLIT_A)
    printf("int %d\n", n->intvalue);
  else
    printf("%d %s %d\n", leftval, ASTop[n->op], rightval);
  */

  switch (n->op) {
    case ADD_A: return (leftval + rightval);
    case SUB_A: return (leftval - rightval);
    case MUL_A: return (leftval * rightval);
    case DIV_A: return (leftval / rightval);
    case INTLIT_A: return (n->intvalue);
    default:
      fprintf(stderr, "[ERROR] Unknown AST operator %d\n", n->op);
      exit(1);
  }
}
