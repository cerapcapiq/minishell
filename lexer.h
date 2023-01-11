

#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct LEXER_STRUCT // this struct keep the current state of lexer
{
	char c;
	unsigned int i;
	char *contents; //input that we pass from main/terminal
} lexer_T;

lexer_T* init_lexer(char* contents);

void lexer_advance(lexer_T *lexer); // go to next lexer/contents until end of char

void lexer_skip_whitespace(lexer_T *lexer);

token_T *lexer_get_next_token(lexer_T* lexer);

token_T *lexer_collect_str(lexer_T* lexer);

token_T *lexer_advance_with_token(lexer_T* lexer, token_T* token);

char *lexer_get_current_char_to_str(lexer_T* lexer); //convert char input as str

token_T* lexer_collect_id(lexer_T* lexer);

#endif





