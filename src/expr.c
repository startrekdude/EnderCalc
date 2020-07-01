#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "expr.h"
#include "libbf.h"
#include "symbols.h"

// Add a two-bit tag to a pointer, inplace
void token_tag(void **ptr, unsigned int tag) {
	assert(tag <= 3);
	
	uintptr_t val = (uintptr_t) *ptr;
	
	// We set at most two bits (62 and 63)
	for (unsigned int i = 0; i < 2; i++) {
		unsigned int bit = (tag & (1 << i)) >> i;
		
		// Use a 64 bit value as the mask, or it truncates
		uintptr_t shift = 1;
		shift <<= 62 + i;
		
		if (bit) {	
			val |= shift;
		} else {
			val &= ~shift;
		}
	}
	
	*ptr = (void*) val;
}

// Remove the token from the pointer and return it
unsigned int token_untag(void **ptr) {
	uintptr_t val = (uintptr_t) *ptr;
	unsigned int tag = (val & 0xc000000000000000) >> 62;
	
	// somewhat hacky way to sign-extend the value
	intptr_t sval = (intptr_t) *ptr;
	sval <<= 16;
	sval >>= 16;
	*ptr = (void*) sval;
	return tag;
}

// Fairly basic - iterate over the expression and print each token
static void internal_expr_print(vector_t expr) {
	unsigned int len = vector_get_len(expr);
	for (unsigned int i = 0; i < len; i++) {
		void *tk = vector_get(expr, i);
		unsigned int tag = token_untag(&tk);
		
		// Print the token
		if (tag == TOKEN_EXPR) {
			putchar('(');
			internal_expr_print(tk);
			putchar(')');
		} else if (tag == TOKEN_STR) {
			str_slice_t *str = (str_slice_t*) tk;
			printf("%.*s", str->len, str->ptr);
		} else if (tag == TOKEN_NUM) {
			bf_t *num = (bf_t*) tk;
			char *s = bf_ftoa(0, num, 10, g_engine_prec, BF_FTOA_FORMAT_FREE_MIN);
			printf("%s", s);
			free(s);
		} else if (tag == TOKEN_SYM) {
			printf("%s", g_sym_names[(unsigned int) (uintptr_t) tk]);
		}
		
		// Spaces between tokens
		if (i + 1 < len)
			putchar(' ');
	}
}

// print - but followed by newline
void expr_print(vector_t expr) {
	internal_expr_print(expr);
	puts("");
}

// I'd comment these but they're pretty self-explanatory

void token_delete(void *tk) {
	unsigned int tag = token_untag(&tk);
	if (tag == TOKEN_EXPR) {
		expr_delete(tk);
	} else if (tag == TOKEN_STR) {
		free(tk);
	} else if (tag == TOKEN_NUM) {
		engine_num_free((bf_t*) tk);
	} // symbols don't need to be freed
}

void expr_delete(vector_t expr) {
	unsigned int len = vector_get_len(expr);
	for (unsigned int i = 0; i < len; i++)
		token_delete(vector_get(expr, i));
	vector_delete(expr);
}