#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Types and type handling


// Return true if a type is an int type
// of any size, false otherwise
int inttype(int type)
{
  if (type == CHAR_P || type == INT_P || type == LONG_P)
    return (1);
  return(0);
}


// Return true if a type is of pointer type
int ptrtype(int type)
{
  if (type == VOIDPTR_P || type == CHARPTR_P ||
      type == INTPTR_P  || type == LONGPTR_P)
    return (1);
  return (0);
}


// Given a primitive type, return
// the type which is a pointer to it
int pointer_to(int type)
{
  int newtype;
  switch (type)
  {
    case VOID_P: newtype = VOIDPTR_P; break;
    case CHAR_P: newtype = CHARPTR_P; break;
    case INT_P:  newtype = INTPTR_P;  break;
    case LONG_P: newtype = LONGPTR_P; break;
    default: fatald("Unrecognised in pointer_to: type", type);
  }
  return (newtype);
}


// Given a primitive pointer type, return
// the type which it points to
int value_at(int type)
{
  int newtype;
  switch (type)
  {
    case VOIDPTR_P: newtype = VOID_P; break;
    case CHARPTR_P: newtype = CHAR_P; break;
    case INTPTR_P:  newtype = INT_P;  break;
    case LONGPTR_P: newtype = LONG_P; break;
    default: fatald("Unrecognised in value_at: type", type);
  }
  return (newtype);
}


// Given an AST tree and a type which we want it to become,
// possibly modify the tree by widening or scaling so that
// itis compatibel with this type. Return the original tree
// if no changes occurred, a modified tree, or NULL if the
// tree is not compatible with the given type.
// If this will be part of a binary operation, the AST op is not zero.
AST_T *modify_type(AST_T *tree, int rtype, int op)
{
  int ltype;
  int lsize, rsize;

  ltype = tree->type;

  // Compare scalar int types
  if (inttype(ltype) && inttype(rtype))
  {
    // Bth type same, nothing to do
    if (ltype == rtype)
      return (tree);

    // Get the sizes for each type
    lsize = genprimsize(ltype);
    rsize = genprimsize(rtype);

    // Tree's size is too big
    if (lsize > rsize)
      return (NULL);

    // Widen to the right
    if (rsize > lsize)
      return (mkastunary(WIDEN_A, rtype, tree, 0));
  }
  // For pointers on the left
  if (ptrtype(ltype))
  {
    // OK is same type on right and not doing a binary op
    if (op == 0 &&ltype == rtype)
      return (tree);
  }
  // We can scale only on ADD_A or SUB_A operation
  if (op == ADD_A || op == SUB_A)
  {
    // Left is int type, right is pointer type and the size
    // of the original type is >1: scale the left
    if (inttype(ltype) && ptrtype(rtype))
    {
      rsize = genprimsize(value_at(rtype));
      if (rsize > 1)
        return (mkastunary(SCALE_A, rtype, tree, rsize));
    }
  }
  // If we get here, the types are not compatible
  return (NULL);
}
