#include <stdio.h>

#include "expr.h"
#include "parse.h"

unsigned int g_parse_debug = 0;

// Run the entire parser on the expression
int parse_expr(char *s, vector_t *expr) {
	// Match brackets
	int err = match_brackets(s, expr);
	if (err != PARSE_SUCCESS) {
		expr_delete(*expr);
		return err;
	}
	if (g_parse_debug) {
		printf("match_brackets: ");
		expr_print(*expr);
	}
	
	// Tokenize (this destroys the old expr)
	err = tokenize(expr);
	if (err != PARSE_SUCCESS) return err;
	if (vector_get_len(*expr) == 0) {
		expr_delete(*expr);
		return PARSE_EMPTY_EXPRESSION;
	}
	if (g_parse_debug) {
		printf("tokenize: ");
		expr_print(*expr);
	}
	
	constants(*expr);
	if (g_parse_debug) {
		printf("constants: ");
		expr_print(*expr);
	}
	
	implicit_multiply(expr);
	if (g_parse_debug) {
		printf("implicit_multiply: ");
		expr_print(*expr);
	}
	
	unary_neg(*expr);
	if (g_parse_debug) {
		printf("unary_neg: ");
		expr_print(*expr);
	}
	
	return err;
}

void parse_print_error(int err) {
	switch(err) {
	case PARSE_SUCCESS:
		printf("success");
		break;
	case PARSE_UNMATCHED_CLOSE_BRACKET:
		printf("unmatched close bracket");
		break;
	case PARSE_UNMATCHED_OPEN_BRACKET:
		printf("unmatched open bracket");
		break;
	case PARSE_INVALID_SYMBOL:
		printf("invalid symbol");
		break;
	case PARSE_EMPTY_EXPRESSION:
		printf("empty expression");
		break;
	default:
		printf("unknown error");
	}
}