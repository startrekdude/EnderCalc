#include <assert.h>
#include <string.h>

#include "engine.h"
#include "expr.h"
#include "libbf.h"
#include "parse.h"
#include "symbols.h"

// Read a number at the given position (return characters read)
static int read_num(str_slice_t *str, bf_t **num) {
	*num = engine_num_alloc();
	
	// conveniently enough, libbf has an out parameter that returns chars read
	const char *pnext;
	bf_atof(*num, str->ptr, &pnext, 10, g_engine_prec, BF_ATOF_NO_NAN_INF);
	
	return pnext - str->ptr;
}

// Read a symbol at the given position - return > SYM_MAX for error
// (otherwise, return characters read)
static int read_sym(str_slice_t *str, uintptr_t *sym) {
	// Find the symbol string that (prefix) matches
	for (unsigned int i = 0; i < g_sym_str_len; i++) {
		const sym_str_t *ss = &g_sym_str[i];
		if (strncmp(ss->str, str->ptr, ss->len) == 0) {
			*sym = ss->sym;
			return ss->len;
		}
	}
	*sym = SYM_MAX+1;
	return 0;
}

// Tokenize a string into the given vector - possibility of error
static int tokenize_str(str_slice_t *str, vector_t expr) {
	while (str->len > 0) {
		char c = *str->ptr;
		if (c == ' ') {
			// Skip whitespace
			str->len--;
			str->ptr++;
		} else if (c >= '0' && c <= '9') {
			// Read a number
			bf_t *num;
			unsigned int i = read_num(str, &num);
			str->len -= i;
			str->ptr += i;
			token_tag((void**) &num, TOKEN_NUM);
			vector_add(expr, num);
		} else {
			// Read a symbol
			uintptr_t sym;
			unsigned int i = read_sym(str, &sym);
			if (sym > SYM_MAX) return PARSE_INVALID_SYMBOL;
			str->len -= i;
			str->ptr += i;
			token_tag((void**) &sym, TOKEN_SYM);
			vector_add(expr, (void*)sym);
		}
	}
	return PARSE_SUCCESS;
}

// Tokenize the expression. Split STR into NUM and SYM
static int internal_tokenize(vector_t expr, vector_t *result) {
	*result = vector_new();
	unsigned int len = vector_get_len(expr);
	
	for (unsigned int i = 0; i < len; i++) {
		void *tk = vector_get(expr, i);
		unsigned int tag = token_untag(&tk);
		
		if (tag == TOKEN_EXPR) {
			// Recursively run the tokenizer
			vector_t sub_result;
			int err = internal_tokenize((vector_t) tk, &sub_result);
			if (err != PARSE_SUCCESS) {
				expr_delete(*result);
				return err;
			}
			token_tag((void**) &sub_result, TOKEN_EXPR);
			vector_add(*result, sub_result);
		} else if (tag == TOKEN_STR) {
			// Try to tokenize the string - a lot of the work happens here
			str_slice_t *str = (str_slice_t*) tk;
			int err = tokenize_str(str, *result);
			if (err != PARSE_SUCCESS) {
				expr_delete(*result);
				return err;
			}
		} else {
			// Panic! This would be a programmer error, so crashing is ok
			assert(0);
		}
	}
	
	return PARSE_SUCCESS;
}

// Wrapper that frees the old expression
int tokenize(vector_t *expr) {
	vector_t result;
	int err = internal_tokenize(*expr, &result);
	expr_delete(*expr);
	*expr = result;
	return err;
}