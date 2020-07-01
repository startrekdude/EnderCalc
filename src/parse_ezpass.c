#include "engine.h"
#include "expr.h"
#include "parse.h"
#include "symbols.h"

// Replace every constant symbol with an actual number, go figure
// so like pi is 3.14, e is 2.7
// Operates inplace
void constants(vector_t expr) {
	unsigned int len = vector_get_len(expr);
	for (unsigned int i = 0; i < len; i++) {
		void *tk = vector_get(expr, i);
		unsigned int tag = token_untag(&tk);
		if (tag == TOKEN_SYM) {
			unsigned int sym = (unsigned int) (uintptr_t) tk;
			int c = g_sym_const[sym];
			if (c != SYMC_NOT_CONSTANT) {
				bf_t *num = engine_copy_const(c);
				token_tag((void**) &num, TOKEN_NUM);
				vector_set(expr, i, num);
				// We don't need to delete the old one-just a symbol, literally a number
			}
		} else if (tag == TOKEN_EXPR) {
			constants((vector_t) tk); // gotta recurse
		}
	}
}

// Adds implicit multiplication symbols - whenever a subexpr is preceded by a postfix op,
// a subexpr, or a decimal - never fails
void implicit_multiply(vector_t *expr) {
	vector_t result = vector_new();
	unsigned int len = vector_get_len(*expr);
	
	void *mul_tk = (void*) (uintptr_t) SYM_MUL;
	token_tag(&mul_tk, TOKEN_SYM);
	
	for (unsigned int i = 0; i < len; i++) {
		void *tk = vector_get(*expr, i);
		unsigned int tag = token_untag(&tk);
		
		if (tag == TOKEN_EXPR) {
			// First thing: recurse
			implicit_multiply((vector_t*) &tk);
			
			// Next: do we need a MUL symbol?
			if (i > 0) {
				void *prev_tk = vector_get(*expr, i - 1);
				unsigned int prev_tag = token_untag(&prev_tk);
				if (prev_tag == TOKEN_NUM || prev_tag == TOKEN_EXPR) {
					vector_add(result, mul_tk);
				} else if (prev_tag == TOKEN_SYM) {
					unsigned int sym = (unsigned int) (uintptr_t) prev_tk;
					char c = g_op_args[sym];
					if (c == SYMO_UNARY_POSTFIX || c == SYMO_NOT_OPERATOR) {
						vector_add(result, mul_tk);
					}
				}
			}
		}
		
		token_tag(&tk, tag);
		vector_add(result, tk);
	}
	vector_delete(*expr);
	*expr = result;
}

// Swaps out SUB for NEG in situations where SUB is never valid (NEG might not be either)
void unary_neg(vector_t expr) {
	void *neg_tk = (void*) (uintptr_t) SYM_NEG;
	token_tag(&neg_tk, TOKEN_SYM);
	
	unsigned int len = vector_get_len(expr);
	for (unsigned int i = 0; i < len; i++) {
		void *tk = vector_get(expr, i);
		unsigned int tag = token_untag(&tk);
		if (tag == TOKEN_SYM) {
			unsigned int sym = (unsigned int) (uintptr_t) tk;
			if (sym == SYM_SUB) {
				if (i == 0) {
					vector_set(expr, 0, neg_tk);
				} else {
					void *prev_tk = vector_get(expr, i - 1);
					unsigned int prev_tag = token_untag(&prev_tk);
					if (prev_tag == TOKEN_SYM) {
						unsigned int prev_sym = (unsigned int) (uintptr_t) prev_tk;
						char c = g_op_args[prev_sym];
						if (c != SYMO_UNARY_POSTFIX && c != SYMO_NOT_OPERATOR) {
							vector_set(expr, i, neg_tk);
						}
					}
				}
			}
		} else if (tag == TOKEN_EXPR) {
			unary_neg((vector_t) tk);
		}
	}
}