#include "include/parser.h"
#include <stdlib.h>

parser_T* init_parser(lexer_T* lexer)
{
  parser_T* parser = calloc(1, sizeof(struct PARSER));
  parser->lexer = lexer;
  parser->current_token = next_token(parser->lexer);
  parser->prev_token = (void*)0;
  parser->data_type = (void*)0;

  return parser;
}

// etc

void parser_type_error(parser_T* parser)
{
  printf("[Line %d] Syntax error\n", parser->lexer->line_n);
  exit(1);
}

void parser_syntax_error(parser_T* parser);

void parser_unexpected_token(parser_T* parser, int token_type);

AST_T* parser_parse(parser_T* parser);

AST_T* parser_eat(parser_T* parser, int token_type);

/*
AST_T* parser_parse_statement(parser_T* parser, scope_T* scope);

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope);

AST_T* parser_parse_type(parser_T* parser, scope_T* scope);

// values
AST_T* parser_parse_string(parser_T* parser, scope_T* scope);

AST_T* parser_parse_char(parser_T* parser, scope_T* scope);

AST_T* parser_parse_integer(parser_T* parser, scope_T* scope);

AST_T* parser_parse_variable(parser_T* parser, scope_T* scope);

AST_T* parser_parse_object(parser_T* parser, scope_T* scope);

// functions

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope, AST_T* expr);

AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope);
*/
