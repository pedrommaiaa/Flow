#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Generic code generator

// Given an AST, generate
// assembly code recursively
int genAST(AST_T *n, int reg)
{
  int leftreg, rightreg;

  // Get the left and right sub-tree values
  if (n->left)
    leftreg = genAST(n->left, -1);
  if (n->right)
    rightreg = genAST(n->right, leftreg);

  switch (n->op)
  {
    case ADD_A: return (cgadd(leftreg, rightreg));
    case SUB_A: return (cgsub(leftreg, rightreg));
    case MUL_A: return (cgmul(leftreg, rightreg));
    case DIV_A: return (cgdiv(leftreg, rightreg));

    case EQUAL_A: return (cgequal(leftreg, rightreg));
    case NOT_EQUAL_A: return (cgnotequal(leftreg, rightreg));
    case LESS_THAN_A: return (cglessthan(leftreg, rightreg));
    case GREATER_THAN_A: return (cggreaterthan(leftreg, rightreg));
    case LESS_OR_EQUAL_A: return (cglessequal(leftreg, rightreg));
    case GREATER_OR_EQUAL_A: return (cggreaterequal(leftreg, rightreg));

    case INTLIT_A: return (cgloadint(n->v.intvalue));
    case IDENT_A: return (cgloadglob(Gsym[n->v.id].name));
    case LVIDENT_A: return (cgstorglob(reg, Gsym[n->v.id].name));
    case ASSIGN_A: return (rightreg);// The work has already been done, return the result
    default: fatald("Unknown AST operator", n->op);
  }
}


void genpreamble() 
{
  cgpreamble();
}

void genpostamble() 
{
  cgpostamble();
}

void genfreeregs() 
{
  freeall_registers();
}

void genprintint(int reg) 
{
  cgprintint(reg);
}

void genglobsym(char *s) 
{
  cgglobsym(s);
}
