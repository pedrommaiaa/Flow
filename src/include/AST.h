#ifndef AST_H
#define AST_H
#include "lexer.h"
#include "data_type.h"

struct RUNTIME_STRUCT;

typedef struct AST_STRUCT
{
  enum
  {
    AST_OBJECT,
    AST_VARIABLE, 
    AST_VARIABLE_DEFINITION,
    AST_FUNCTION_DEFINITION,
    AST_FUNCTION_CALL,
    AST_STRING,
    AST_CHAR,
    AST_INTEGER,
    AST_COMPOUND,
    AST_TYPE
  } type;

  int line_n;
  int int_value;
  long int long_int_value;
  unsigned int is_object_child;
  char char_value;
  char* string_value;
  struct AST_STRUCT* variable_value;
  struct AST_STRUCT* variable_type;
  struct AST_STRUCT* variable_assignment_left;
  char* function_name; 


} AST_T;

AST_T* init_ast(int type);
AST_T* init_ast_with_line(int type, int line_n);

AST_T* ast_copy(AST_T* ast);
AST_T* ast_copy_object(AST_T* ast);
AST_T* ast_copy_variable(AST_T* ast);
AST_T* ast_copy_variable_definition(AST_T* ast);
AST_T* ast_copy_function_definition(AST_T* ast);
AST_T* ast_copy_string(AST_T* ast);
AST_T* ast_copy_char(AST_T* ast);
AST_T* ast_copy_integer(AST_T* ast);
AST_T* ast_copy_compound(AST_T* ast);
AST_T* ast_copy_type(AST_T* ast);
AST_T* ast_copy_variable_assignment(AST_T* ast);
AST_T* ast_copy_variable_modifier(AST_T* ast);
AST_T* ast_copy_function_call(AST_T* ast);

char* ast_to_string(AST_T* ast);
char* ast_object_to_string(AST_T* ast);
char* ast_function_definition_to_string(AST_T* ast);
char* ast_function_call_to_string(AST_T* ast);
char* ast_integer_to_string(AST_T* ast);
char* ast_type_to_string(AST_T* ast);
#endif
