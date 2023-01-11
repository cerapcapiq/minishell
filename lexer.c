
#include "token.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


// init lexer by allocate memory for struct(input)
lexer_T* init_lexer(char *contents) //*contents is input that we pass from main/terminal
{
	lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
	lexer->contents = contents;
	lexer->i = 0; //start at first char of input
	lexer->c = contents[lexer->i];
	return lexer;
	
}

void lexer_advance(lexer_T* lexer) // go to next character until end (until null terminator)
{
	if (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
	{
		lexer->i += 1;
		lexer->c = lexer->contents[lexer->i];
	}
}

void lexer_skip_whitespace(lexer_T *lexer) //skip space and newline
{
	while (lexer->c == ' ' || lexer->c == 10)
	{
		lexer_advance(lexer);
	}
}


token_T* lexer_collect_str(lexer_T* lexer)
{
	lexer_advance(lexer); // to skip the quote ''

	char* value = calloc(1, sizeof(char));
	value[0] = '\0';

	while (lexer->c != '"') // loop until we find the end quote ''
	{
		char *s = lexer_get_current_char_to_str(lexer);
		value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
		strcat(value, s);

		lexer_advance(lexer);

	}
	lexer_advance(lexer); //to ignore end quote

	return init_token(TOKEN_STRING, value);
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token)
{
	lexer_advance(lexer);

	return token;
}



token_T* lexer_get_next_token(lexer_T* lexer)  //parsing and init the token from lexer, 
{
	while (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
	{
		if (lexer->c == ' ' || lexer->c == 10)
			lexer_skip_whitespace(lexer);
		if (lexer->c == '"')  //detect a str 
			return lexer_collect_str(lexer);
		if (isalnum(lexer->c))
				return lexer_collect_id(lexer);
		switch (lexer->c)
		{
			case '=': return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_to_str(lexer))); break;
			case ';': return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_to_str(lexer))) ; break;
			case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_to_str(lexer))) ; break;
			case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_to_str(lexer))) ; break;
			case ',': return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_to_str(lexer))) ; break;
		}
	}
	return init_token(TOKEN_EOF, "\0");
}

token_T* lexer_collect_id(lexer_T* lexer)
{
	char *value = calloc(1, sizeof(char));
	value[0] = '\0';

	while (isalnum(lexer->c))  //loop if the lexer is alnum 
	{
		char *s = lexer_get_current_char_to_str(lexer);
		value = realloc(value, (strlen(value) + strlen(s) + 1 * sizeof(char)));
		strcat(value, s);

		lexer_advance(lexer);
		}

		return init_token(TOKEN_ID,	value);
}





char* lexer_get_current_char_to_str(lexer_T* lexer)
{
	char *str = calloc(2, sizeof(char));
	str[0] = lexer->c;
	str[1] = '\0';

	return str;
}


					  
					
			
					  
					 


