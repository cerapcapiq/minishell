

#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "visitor.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>


char *lineptr;


int main(int ac, char *av[])
{
	while (1)
	{
		lineptr = readline("minishell>");
		if (strlen(lineptr)>0) {add_history(lineptr);}
		if(!strcmp(lineptr, "exit")) { break;exit(1); }
	lexer_T* lexer = init_lexer(lineptr);

	parser_T* parser = init_parser(lexer); //parser take token from lexer, 
	AST_T* root = parser_parse(parser); //ast like recursive fx
	visitor_T* visitor = init_visitor();
	visitor_visit(visitor, root);

	printf("%d\n", root->type);
	printf("%zu\n", root->compound_size);
	}
	return 0;
}

//lexer->parser (ast) -> visitor
//lexer make a token
//parser ask lexer next token
//then parser will create abstract syntax tree (ast)
//ast will has multi node

//visitor (go to every node in ast, and perform the fx/execute it)
//visitor will go to root node of ast, look what type it is and call what type 