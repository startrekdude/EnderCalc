#ifndef _INCLUDED_EXPR_H
#define _INCLUDED_EXPR_H

#include "vector.h"

#define TOKEN_EXPR 0
#define TOKEN_STR  1
#define TOKEN_NUM  2
#define TOKEN_SYM  3

typedef struct {
	char *ptr; // start location of slice
	unsigned int len; // number of characters in slice
} str_slice_t;

void token_tag(void **ptr, unsigned int tag);
unsigned int token_untag(void **ptr);

void expr_print(vector_t expr);

void token_delete(void *tk);
void expr_delete(vector_t expr);

#endif