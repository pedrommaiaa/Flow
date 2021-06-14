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
  // Voids not compatible with anything
  if ((*left == VOID_P) || (*right == VOID_P))
    return (0);

  // Same types, they are compatible
  if (*left == *right)
  {
    *left = *right = 0;
    return (1);
  }

  // Widen CHAR_Ps to INT_Ps as required
  if ((*left == CHAR_P) && (*right == INT_P))
  {
    *left = WIDEN_A;
    *right = 0;
    return (1);
  }
  if ((*left == INT_P) && (*right == CHAR_P))
  {
    if (onlyright)
      return (0);
    *left = 0;
    *right = WIDEN_A;
    return (1);
  }

  // Anything remaining is compatible
  *left = *right = 0;
  return (1);
}
