#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// Types and type handling


// Given two primitive types,
// return true if they are compatible,
// false otherwise. Also return either
// zero or an WIDEN_A operation if one
// has to be widened to match the other.
// If onlyright is true, only widen left to right.
int type_compatible(int *left, int *right, int onlyright) 
{
  int leftsize, rightsize;

  // Same types, they are compatible
  if (*left == *right) {
    *left = *right = 0;
    return (1);
  }
  // Get the sizes for each type
  leftsize = genprimsize(*left);
  rightsize = genprimsize(*right);

  // Types with zero size are not
  // not compatible with anything
  if ((leftsize == 0) || (rightsize == 0))
    return (0);

  // Widen types as required
  if (leftsize < rightsize) 
  {
    *left = WIDEN_A;
    *right = 0;
    return (1);
  }
  if (rightsize < leftsize) 
  {
    if (onlyright)
      return (0);
    *left = 0;
    *right = WIDEN_A;
    return (1);
  }
  // Anything remaining is the same size
  // and thus compatible
  *left = *right = 0;
  return (1);
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
