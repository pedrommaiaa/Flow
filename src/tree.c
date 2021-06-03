#include "include/defs.h"
#include "include/data.h"
#include "include/decl.h"

// AST tree functions

// Build and return a generic AST node
AST_T *mkastnode(int op, AST_T *left, AST_T *right, int intvalue) {
  AST_T *n;

  // Malloc a new ASTnode
  n = (AST_T *) malloc(sizeof(AST_T));
  if (n == NULL) {
    fprintf(stderr, "[ERROR] Unable to malloc in mkastnode()\n");
    exit(1);
  }
  // Copy in the field values and return it
  n->op = op;
  n->left = left;
  n->right = right;
  n->intvalue = intvalue;
  return (n);
}


// Make an AST leaf node
AST_T *mkastleaf(int op, int intvalue) {
  return (mkastnode(op, NULL, NULL, intvalue));
}

// Make a unary AST node: only one child
AST_T *mkastunary(int op, AST_T *left, int intvalue) {
  return (mkastnode(op, left, NULL, intvalue));
}
