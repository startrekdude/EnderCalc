#ifndef _INCLUDED_PARSE_H
#define _INCLUDED_PARSE_H

#include "vector.h"

#define PARSE_SUCCESS                 0
#define PARSE_UNMATCHED_CLOSE_BRACKET 1
#define PARSE_UNMATCHED_OPEN_BRACKET  2
#define PARSE_INVALID_SYMBOL          3
#define PARSE_EMPTY_EXPRESSION        4

// 0 = off, 1 = on
extern unsigned int g_parse_debug;

int match_brackets(char *s, vector_t *expr);
int tokenize(vector_t *expr);
void constants(vector_t expr); // cannot fail, inplace
void implicit_multiply(vector_t *expr); // cannot fail, does create new vector
void unary_neg(vector_t expr); // cannot fail, inplace

int parse_expr(char *s, vector_t *expr);
void parse_print_error(int err);

#endif