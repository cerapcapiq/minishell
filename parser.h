
#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"
#include "ast.h"

typedef struct PARSER_STRUCT
{
	lexer_T* lexer;
	token_T* current_token;
	token_T* prev_token;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void  parser_eat(parser_T* parser, int token_type);  //this parser expected a certain token, if not, exit/error

AST_T* parser_parse(parser_T* parser); //main parser from token to ast, will return whole input into AST tree

AST_T* parser_parse_onestatement(parser_T* parser);

AST_T* parser_parse_multistatement(parser_T* parser);

AST_T* parser_parse_expr(parser_T* parser);  // for math expression

AST_T* parser_parse_factor(parser_T* parser);  //if token invovle multiplication

AST_T* parser_parse_term(parser_T* parser); //token for addition math

AST_T* parser_parse_function_call(parser_T* parser);

AST_T* parser_parse_variable(parser_T* parser);

AST_T* parser_parse_str(parser_T* parser);

AST_T* parser_parse_variable_definition(parser_T *parser);

AST_T* parser_parse_id(parser_T* parser);
#endif
