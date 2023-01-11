

#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

parser_T* init_parser(lexer_T* lexer)
{

	parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
	parser->lexer = lexer;
	parser->current_token = lexer_get_next_token(lexer);
	parser->prev_token = parser->current_token; //saving prev token to current token

	return (parser);
}

void parser_eat(parser_T* parser, int token_type) //when u expect certain token only after a certain token
{
	if (parser->current_token->type == token_type)
	{
		parser->prev_token = parser->current_token;
		parser->current_token = lexer_get_next_token(parser->lexer);
	}
	else
	{
		printf(
				"Unexpected token '%s' , with type %d",
				parser->current_token->value,
				parser->current_token->type);
	}
}

AST_T* parser_parse(parser_T* parser)
{
	return parser_parse_multistatement(parser);
}

AST_T* parser_parse_onestatement(parser_T* parser)
{
	switch (parser->current_token->type)
	{
		case TOKEN_ID: return parser_parse_id(parser);
	}
	return init_ast(AST_NOOP);
}


AST_T* parser_parse_multistatement(parser_T* parser)
{
	AST_T* compound = init_ast(AST_COMPOUND);  //becasue multi , hence compund(more than one token)
	compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

	AST_T* ast_statement = parser_parse_onestatement(parser); //take one at a time, then increase size by 1
	compound->compound_value[0] = ast_statement;
	compound->compound_size += 1;

	
	while (parser->current_token->type == TOKEN_SEMI)
	{
		parser_eat(parser, TOKEN_SEMI);
	
		AST_T* ast_statement = parser_parse_onestatement(parser);

		if (ast_statement)
		{
		compound->compound_size += 1;
		compound->compound_value = realloc(
			compound->compound_value,
			compound->compound_size * sizeof(struct AST_STRUCT*)
			);
		compound->compound_value[compound->compound_size-1] = ast_statement;
	}
	}

	return compound;
}

AST_T* parser_parse_variable_definition(parser_T* parser)
{
	parser_eat(parser, TOKEN_ID); //expecting var
	char *variable_definition_variable_name = parser->current_token->value;
	parser_eat(parser, TOKEN_ID); //expect the name
	parser_eat(parser, TOKEN_EQUALS); //expecting = to def the var
	AST_T* variable_definition_value = parser_parse_expr(parser);

	AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
	variable_definition->variable_definition_variable_name = variable_definition_variable_name;
	variable_definition->variable_definition_value = variable_definition_value;

	return variable_definition;
}

AST_T* parser_parse_function_definition(parser_T* parser)
{
    AST_T* ast = init_ast(AST_FUNCTION_DEFINITION);
    parser_eat(parser, TOKEN_ID); // function

    char* function_name = parser->current_token->value;
    ast->function_definition_name = calloc(
            strlen(function_name) + 1, sizeof(char)
    );
    strcpy(ast->function_definition_name, function_name);

    parser_eat(parser, TOKEN_ID); // function name

    parser_eat(parser, TOKEN_LPAREN);

    ast->function_definition_args =
        calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* arg = parser_parse_variable(parser);
    ast->function_definition_args_size += 1;
    ast->function_definition_args[ast->function_definition_args_size-1] = arg;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);

        ast->function_definition_args_size += 1;

        ast->function_definition_args =
            realloc(
                    ast->function_definition_args,
                    ast->function_definition_args_size * sizeof(struct AST_STRUCT*)
                   );

        AST_T* arg = parser_parse_variable(parser);
        ast->function_definition_args[ast->function_definition_args_size-1] = arg;
    }

    parser_eat(parser, TOKEN_RPAREN);
 
    
    ast->function_definition_body = parser_parse_multistatement(parser);


    return ast;
}

AST_T* parser_parse_id(parser_T* parser)  //detect the id of the token, then execute
{
	if (strcmp(parser->current_token->value, "var") == 0)
	{
		printf("var\n");
		return parser_parse_variable_definition(parser);
	}
	else
    if (strcmp(parser->current_token->value, "function") == 0)
    {
        return parser_parse_function_definition(parser);
    }
	else
	{
		return parser_parse_variable(parser);
	}
}

AST_T* parser_parse_variable(parser_T* parser)
{
	char *token_value = parser->current_token->value;
	parser_eat(parser, TOKEN_ID);

	if (parser->current_token->type == TOKEN_LPAREN) // if token is "(" must be fx
		return parser_parse_function_call(parser);

	AST_T* ast_variable = init_ast(AST_VARIABLE);
	ast_variable->variable_name = token_value;

	return ast_variable;
}

AST_T* parser_parse_str(parser_T* parser)
{
	AST_T* ast_string = init_ast(AST_STRING);
	ast_string->str_value = parser->current_token->value;

	parser_eat(parser, TOKEN_STRING);

	return ast_string;
}


AST_T* parser_parse_expr(parser_T* parser)
{
	switch (parser->current_token->type)
	{
		case TOKEN_STRING: return parser_parse_str(parser);   //using str declared by var, paste into fucntion eg : print(str)
		case TOKEN_ID: return parser_parse_id(parser);
	}
	return init_ast(AST_NOOP);
}

AST_T* parser_parse_function_call(parser_T* parser)
{
	AST_T* function_call = init_ast(AST_FUNCTION_CALL);
	function_call->function_call_name = parser->prev_token->value;
	parser_eat(parser, TOKEN_LPAREN);
	function_call->function_call_arg = calloc(1, sizeof(struct AST_STRUCT*));

	AST_T* ast_expr = parser_parse_expr(parser);
	function_call->function_call_arg[0] = ast_expr;
	function_call->function_call_arg_size += 1;
	
	while (parser->current_token->type == TOKEN_COMMA) //more than 1 
	{
		parser_eat(parser, TOKEN_COMMA);
		
		AST_T* ast_expr = parser_parse_expr(parser);
		function_call->function_call_arg_size += 1;
		function_call->function_call_arg = realloc(
			function_call->function_call_arg,
			function_call->function_call_arg_size * sizeof(struct AST_STRUCT*)
			);
		function_call->function_call_arg[function_call->function_call_arg_size - 1] = ast_expr;
	}
	parser_eat(parser, TOKEN_RPAREN);

	return function_call;


}

