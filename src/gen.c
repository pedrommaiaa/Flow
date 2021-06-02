#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Generic code generator

// Given an AST, generate
// assembly code recursively
static int genAST(struct ASTnode *n)
{
  int leftreg, rightreg;

  // Get the left and right sub-tree values
  if (n->left)
  {
    leftreg = genAST(n->left);
  }
  if (n->right)
  {
    rightreg = genAST(n->right);
  }

  switch (n->op)
  {
    case ADD_A: return (cgadd(leftreg, rightreg));
    case SUB_A: return (cgsub(leftreg, rightreg));
    case MUL_A: return (cgmul(leftreg, rightreg));
    case DIV_A: return (cgdiv(leftreg, rightreg));
    case INTLIT_A: return (cgload(n->intvalue));
    default: 
        fprintf(stderr, "[ERROR] Unkown AST operator %d\n", n->op);
        exit(1);
  }
}

void generatecode(struct ASTnode *n)
{
  int reg;

  cgpreamble();
  reg = genAST(n);
  cgprintint(reg);
  cgpostamble();
}