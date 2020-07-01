#ifndef _INCLUDED_EVAL_H
#define _INCLUDED_EVAL_H

#include "libbf.h"
#include "vector.h"

#define EVAL_SUCCESS          0
#define EVAL_SYNTAX_ERROR     1
#define EVAL_MISSING_OPERATOR 2
#define EVAL_NO_SAVED_ANSWER  3
#define EVAL_ZERO_DIVISION    4
#define EVAL_MATH_ERROR       5
#define EVAL_OVERFLOW         6
#define EVAL_NO_RECALL        7

extern bf_t *g_last_answer;

unsigned int eval_expr(vector_t expr, bf_t **result);
void eval_print_error(unsigned int err);

#endif