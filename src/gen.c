#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Generic code generator

// Generate and return a new label number
int genlabel(void) 
{
  static int id = 1;
  return (id++);
}

// Generate the code for an IF statement
// and an optional ELSE clause
static int genIF(AST_T *n) 
{
  int Lfalse, Lend;

  // Generate two labels: one for the
  // false compound statement, and one
  // for the end of the overall IF statement.
  // When there is no ELSE clause, Lfalse _is_
  // the ending label!
  Lfalse = genlabel();
  if (n->right)
    Lend = genlabel();

  // Generate the condition code followed
  // by a jump to the false label.
  // We cheat by sending the Lfalse label as a register.
  genAST(n->left, Lfalse, n->op);
  genfreeregs();

  // Generate the true compound statement
  genAST(n->mid, NOREG, n->op);
  genfreeregs();

  // If there is an optional ELSE clause,
  // generate the jump to skip to the end
  if (n->right)
    cgjump(Lend);

  // Now the false label
  cglabel(Lfalse);

  // Optional ELSE clause: generate the
  // false compound statement and the
  // end label
  if (n->right) 
  {
    genAST(n->right, NOREG, n->op);
    genfreeregs();
    cglabel(Lend);
  }

  return (NOREG);
}

// Generate the code for a WHILE statement
// and an optional ELSE clause
static int genWHILE(AST_T *n) 
{
  int Lstart, Lend;

  // Generate the start and end labels
  // and output the start label
  Lstart = genlabel();
  Lend = genlabel();
  cglabel(Lstart);

  // Generate the condition code followed
  // by a jump to the end label.
  // We cheat by sending the Lfalse label as a register.
  genAST(n->left, Lend, n->op);
  genfreeregs();

  // Generate the compound statement for the body
  genAST(n->right, NOREG, n->op);
  genfreeregs();

  // Finally output the jump back to the condition,
  // and the end label
  cgjump(Lstart);
  cglabel(Lend);
  return (NOREG);
}

// Given an AST, the register (if any) that holds
// the previous rvalue, and the AST op of the parent,
// generate assembly code recursively.
// Return the register id with the tree's final value
int genAST(AST_T *n, int reg, int parentASTop) 
{
  int leftreg, rightreg;

  // We now have specific AST node handling at the top
  switch (n->op) 
  {
    case IF_A:
      return (genIF(n));
    case WHILE_A:
      return (genWHILE(n));
    case GLUE_A:
      // Do each child statement, and free the
      // registers after each child
      genAST(n->left, NOREG, n->op);
      genfreeregs();
      genAST(n->right, NOREG, n->op);
      genfreeregs();
      return (NOREG);
    case FUNCTION_A:
      // Generate the function's preamble before the code
      cgfuncpreamble(n->v.id);
      genAST(n->left, NOREG, n->op);
      cgfuncpostamble(n->v.id);
      return (NOREG);
  }

  // General AST node handling below

  // Get the left and right sub-tree values
  if (n->left)
    leftreg = genAST(n->left, NOREG, n->op);
  if (n->right)
    rightreg = genAST(n->right, leftreg, n->op);

  switch (n->op) 
  {
    case ADD_A:
      return (cgadd(leftreg, rightreg));
    case SUB_A:
      return (cgsub(leftreg, rightreg));
    case MUL_A:
      return (cgmul(leftreg, rightreg));
    case DIV_A:
      return (cgdiv(leftreg, rightreg));
    case EQUAL_A:
    case NOT_EQUAL_A:
    case LESS_THAN_A:
    case GREATER_THAN_A:
    case LESS_OR_EQUAL_A:
    case GREATER_OR_EQUAL_A:
      // If the parent AST node is an IF_A or WHILE_A, generate
      // a compare followed by a jump. Otherwise, compare registers
      // and set one to 1 or 0 based on the comparison.
      if (parentASTop == IF_A || parentASTop == WHILE_A)
	      return (cgcompare_and_jump(n->op, leftreg, rightreg, reg));
      else
	      return (cgcompare_and_set(n->op, leftreg, rightreg));
    case INTLIT_A:
      return (cgloadint(n->v.intvalue, n->type));
    case IDENT_A:
      return (cgloadglob(n->v.id));
    case LVIDENT_A:
      return (cgstorglob(reg, n->v.id));
    case ASSIGN_A:
      // The work has already been done, return the result
      return (rightreg);
    case PRINT_A:
      // Print the left-child's value
      // and return no register
      genprintint(leftreg);
      genfreeregs();
      return (NOREG);
    case WIDEN_A:
      // Widen the child's type to the parent's type
      return (cgwiden(leftreg, n->left->type, n->type));
    case RETURN_A:
      cgreturn(leftreg, Functionid);
      return (NOREG);
    case FUNCCALL_A:
      return (cgcall(leftreg, n->v.id));
    default:
      fatald("Unknown AST operator", n->op);
  }
}

void genpreamble() 
{
  cgpreamble();
}
void genfreeregs() 
{
  freeall_registers();
}
void genprintint(int reg) 
{
  cgprintint(reg);
}
void genglobsym(int id) 
{
  cgglobsym(id);
}
int genprimsize(int type) 
{
  return (cgprimsize(type));
}