

#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct AST_STRUCT
{
	enum {
		AST_VARIABLE_DEFINITION,
		AST_FUNCTION_DEFINITION,
		AST_VARIABLE,
		AST_FUNCTION_CALL,
		AST_STRING,
		AST_COMPOUND, // if multi ast
		AST_NOOP	
	} type;

	char *variable_definition_variable_name; //content of(after) var init
	struct AST_STRUCT* variable_definition_value;

	char *variable_name;

	 struct AST_STRUCT* function_definition_body;
    char* function_definition_name;
    struct AST_STRUCT** function_definition_args;
    size_t function_definition_args_size;


	char *function_call_name;
	struct AST_STRUCT** function_call_arg;
	size_t function_call_arg_size;

	char* str_value;

	struct AST_STRUCT** compound_value;
	size_t compound_size;

} AST_T;

AST_T* init_ast(int type);

#endif

