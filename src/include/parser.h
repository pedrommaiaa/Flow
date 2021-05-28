#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "AST.h"


typedef struct PARSER
{
  lexer_T* lexer;
  token_T* prev_token;
  token_T* current_token;
  data_type_T* data_type;
} parser_T;


parser_T* init_parser(lexer_T* lexer);

// etc

void parser_type_error(parser_T* parser);

void parser_syntax_error(parser_T* parser);

void parser_unexpected_token(parser_T* parser, int token_type);

AST_T* parser_parse(parser_T* parser);

AST_T* parser_eat(parser_T* parser, int token_type);

AST_T* parser_parse_statement(parser_T* parser);

AST_T* parser_parse_statements(parser_T* parser);

AST_T* parser_parse_type(parser_T* parser);

// values
AST_T* parser_parse_string(parser_T* parser);

AST_T* parser_parse_char(parser_T* parser);

AST_T* parser_parse_integer(parser_T* parser);

AST_T* parser_parse_variable(parser_T* parser);

AST_T* parser_parse_object(parser_T* parser);

// functions

AST_T* parser_parse_function_call(parser_T* parser, AST_T* expr);

AST_T* parser_parse_function_definition(parser_T* parser);
#endif
