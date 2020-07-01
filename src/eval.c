#include <stdio.h>

#include "engine.h"
#include "eval.h"
#include "expr.h"
#include "mem.h"
#include "symbols.h"

bf_t *g_last_answer = 0;

// duplicate a bf_t into an expression at position i
static bf_t* duplicate_into(bf_t *num, vector_t expr, unsigned int i) {
	bf_t *clone, *tk;
	clone = tk = engine_num_alloc();
	bf_set(clone, num);
	
	token_tag((void**) &tk, TOKEN_NUM);
	vector_set(expr, i, tk);
	
	return clone;
}

// resolve an argument, optionally placing it back
static unsigned int resolve_arg(vector_t expr, unsigned int i, bf_t **result) {
	void *tk = vector_get(expr, i);
	unsigned int tag = token_untag(&tk);
	if (tag == TOKEN_NUM) {
		// numbers are simple = no conversion needed
		*result = tk;
		return EVAL_SUCCESS;
	} else if (tag == TOKEN_EXPR) {
		// for expressions, just solve
		vector_t sub_expr = (vector_t) tk;
		unsigned int err = eval_expr(sub_expr, result);
		return err;
	} else if (tag == TOKEN_SYM) {
		// only answer/memory symbols are allowed in place of a number
		unsigned int sym = (unsigned int) (uintptr_t) tk;
		if (sym == SYM_ANS) {
			if (g_last_answer == 0) return EVAL_NO_SAVED_ANSWER;
			*result = duplicate_into(g_last_answer, expr, i);
			return EVAL_SUCCESS;
		} else if (sym <= SYM_RCL_MAX && sym >= SYM_RCL_MIN) {
			bf_t *saved = g_mem[sym - SYM_RCL_MIN];
			if (saved == 0) return EVAL_NO_RECALL;
			*result = duplicate_into(saved, expr, i);
			return EVAL_SUCCESS;
		} else {
			return EVAL_SYNTAX_ERROR;
		}
	}
}

// The expression evaluator - sometimes errors, sometimes doesn't
unsigned int eval_expr(vector_t expr, bf_t **result) {
	unsigned int err;
	
	// Perform a reduction
	while (vector_get_len(expr) > 1) {
		// 1: Find the next operator, and its location
		unsigned char valid_op = 0;
		unsigned int op = 0;
		unsigned int op_index = 0;
		
		unsigned int len = vector_get_len(expr);
		for (unsigned int i = 0; i < len; i++) {
			void *tk = vector_get(expr, i);
			unsigned int tag = token_untag(&tk);
			if (tag != TOKEN_SYM) continue; // all operators are symbols
			unsigned int sym = (unsigned int) (uintptr_t) tk;
			
			unsigned int precedence = g_op_precedence[sym];
			if (precedence == SYMP_NOT_OPERATOR) continue; // not all symbols are operators
			
			// precedence and associativity rules
			if (valid_op == 0 || precedence < g_op_precedence[op] || 
			    (g_op_associativity[op] == SYMA_RIGHT && precedence <= g_op_precedence[op])) {
				valid_op = 1;
				op = sym;
				op_index = i;
			}
		}
		if (valid_op == 0) return EVAL_MISSING_OPERATOR;
		
		// 2: Collect the arguments
		bf_t *args[2];
		arg_collect_t desc = g_op_arg_collect[g_op_args[op]];
		unsigned int desc_len = desc.len;
		for (unsigned int i = 0; i < desc_len; i++) {
			char off = desc.args[i];
			if (off + op_index < 0 || off + op_index >= len) return EVAL_SYNTAX_ERROR;
			err = resolve_arg(expr, off + op_index, &args[i]);
			if (err != EVAL_SUCCESS) return err;
		}
		
		// 3: do the operation
		bf_t *op_result = engine_num_alloc();
		op_func_t *f = g_op_funcs[op];
		err = f(args, &op_result);
		if (err != EVAL_SUCCESS) {
			engine_num_free(op_result);
			return err;
		}
		if (!bf_is_finite(op_result)) {
			engine_num_free(op_result);
			return EVAL_OVERFLOW;
		}
		
		// 4: cleanup and slot in the result
		for (unsigned int i = 0; i < desc_len; i++) {
			void *tk = vector_get(expr, op_index + desc.cleanup[i]);
			token_delete(tk);
			vector_remove(expr, op_index + desc.cleanup[i]);
		}
		token_tag((void**) &op_result, TOKEN_NUM);
		vector_set(expr, desc.result + op_index, op_result);
	}
	
	if (vector_get_len(expr) == 0) return EVAL_SYNTAX_ERROR;
	
	// Down to one; let's make it a number
	err = resolve_arg(expr, 0, result);
	return err;
}

void eval_print_error(unsigned int err) {
	switch(err) {
	case EVAL_SUCCESS:
		printf("success");
		break;
	case EVAL_SYNTAX_ERROR:
		printf("syntax error");
		break;
	case EVAL_MISSING_OPERATOR:
		printf("missing operator");
		break;
	case EVAL_NO_SAVED_ANSWER:
		printf("no saved answer");
		break;
	case EVAL_ZERO_DIVISION:
		printf("zero division");
		break;
	case EVAL_MATH_ERROR:
		printf("math error");
		break;
	case EVAL_OVERFLOW:
		printf("overflow");
		break;
	case EVAL_NO_RECALL:
		printf("unable to recall");
		break;
	default:
		printf("unknown error");
	}
}