#include <string.h>

#include "engine.h"
#include "eval.h"

unsigned int map_err(int err);

// r = -a0
int calc_neg(bf_t **args, bf_t **result) {
	bf_set(*result, *args);
	bf_neg(*result);
	return EVAL_SUCCESS;
}

// r = a0 + a1
int calc_add(bf_t **args, bf_t **result) {
	return map_err(bf_add(*result, args[0], args[1], g_engine_prec, BF_RNDN));
}

// r = a0 - a1
int calc_sub(bf_t **args, bf_t **result) {
	return map_err(bf_sub(*result, args[0], args[1], g_engine_prec, BF_RNDN));
}

// r = a0 * a1
int calc_mul(bf_t **args, bf_t **result) {
	return map_err(bf_mul(*result, args[0], args[1], g_engine_prec, BF_RNDN));
}

// r = a0 / a1
int calc_div(bf_t **args, bf_t **result) {
	return map_err(bf_div(*result, args[0], args[1], g_engine_prec, BF_RNDN));
}

// r = a0 % a1
int calc_mod(bf_t **args, bf_t **result) {
	return map_err(bf_rem(*result, args[0], args[1], g_engine_prec, BF_RNDN, BF_DIVREM_EUCLIDIAN));
}

// r = e^a0
int calc_exp(bf_t **args, bf_t **result) {
	if (bf_cmp_lt(g_engine_const[ENGINE_CONST_MILLION], *args)) return EVAL_MATH_ERROR;
	return map_err(bf_exp(*result, *args, g_engine_prec, BF_RNDN));
}

// r = a0^a1
int calc_pow(bf_t **args, bf_t **result) {
	if (bf_cmp_lt(g_engine_const[ENGINE_CONST_MILLION], args[1])) return EVAL_MATH_ERROR;
	
	if (bf_cmp_lt(g_engine_const[ENGINE_CONST_MAX_POW_BASE], args[0])) return EVAL_MATH_ERROR;
	
#ifdef __GNUC__
	if (args[0]->expn > 0 && args[1]->expn > 0) {
		unsigned __int128 e1 = args[0]->expn;
		unsigned __int128 e2 = args[1]->expn;
		unsigned __int128 e3 = e1;
		unsigned __int128 lim = *((unsigned __int128*) "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff");
		while (e2) {
			if (e3 > lim / e1) return EVAL_MATH_ERROR;
			e3 *= e1;
			e2--;
		}
	}
#else
#warning "Compiling without anti-hang checks for pow"
#endif
	
	return map_err(bf_pow(*result, args[0], args[1], g_engine_prec, BF_RNDN));
}

// r = a0!
int calc_fac(bf_t **args, bf_t **result) {
	if ((*args)->sign == 1) {
		return EVAL_MATH_ERROR;
	}
	
	bf_set(*result, g_engine_const[ENGINE_CONST_ONE]);
	
	int err = bf_rint(*args, BF_RNDN);
	if (err & BF_ST_INEXACT) return EVAL_MATH_ERROR;
	if (bf_cmp_le(*args, g_engine_const[ENGINE_CONST_MAX_FAC])) {
		while (!bf_is_zero(*args)) {
			bf_mul(*result, *result, *args, g_engine_prec, BF_RNDN);
			bf_sub(*args, *args, g_engine_const[ENGINE_CONST_ONE], g_engine_prec, BF_RNDN);
		}
		return EVAL_SUCCESS;
	} else {
		return EVAL_MATH_ERROR;
	}
}

// r = sqrt(a0)
int calc_sqrt(bf_t **args, bf_t **result) {
	return map_err(bf_sqrt(*result, *args, g_engine_prec, BF_RNDN));
}

//r = cbrt(a0)
int calc_cbrt(bf_t **args, bf_t **result) {
	if ((*args)->sign == 1) {
		bf_neg(*args);
		int err = map_err(bf_pow(*result, *args, g_engine_const[ENGINE_CONST_THIRD], g_engine_prec, BF_RNDN));
		bf_neg(*result);
		return err;
	}
	return map_err(bf_pow(*result, *args, g_engine_const[ENGINE_CONST_THIRD], g_engine_prec, BF_RNDN));
}

//r = a0root(a1)
int calc_root(bf_t **args, bf_t **result) {
	// okay, so in the case of a positive odd integer root, we do allow negative a1 values
	// the logic for this is a bit complex
	unsigned char negate = 0;
	if (args[1]->sign == 1 && args[0]->sign == 0) {
		bf_t *rem = engine_num_alloc();
		bf_rem(rem, args[0], g_engine_const[ENGINE_CONST_TWO], g_engine_prec, BF_RNDN, BF_DIVREM_EUCLIDIAN);
		if (bf_cmp_eq(rem, g_engine_const[ENGINE_CONST_ONE])) {
			negate = 1;
			bf_neg(args[1]);
		}
		bf_set(*result, rem);
		engine_num_free(rem);
	}
	
	int err = map_err(bf_div(args[0], g_engine_const[ENGINE_CONST_ONE], args[0], g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) return err;
	err = map_err(bf_pow(*result, args[1], args[0], g_engine_prec, BF_RNDN));
	if (negate) bf_neg(*result);
	return err;
}

// r = ln(a0)
int calc_ln(bf_t **args, bf_t **result) {
	if (bf_is_zero(*args)) return EVAL_MATH_ERROR;
	return map_err(bf_log(*result, *args, g_engine_prec, BF_RNDN));
}

// r = a0log(a1)
int calc_log(bf_t **args, bf_t **result) {
	if (bf_is_zero(args[0])) return EVAL_MATH_ERROR;
	if (bf_is_zero(args[1])) return EVAL_MATH_ERROR;
	
	int err = map_err(bf_log(args[1], args[1], g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) return err;
	err = map_err(bf_log(args[0], args[0], g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) return err;
	return map_err(bf_div(*result, args[1], args[0], g_engine_prec, BF_RNDN));
}

// r = abs(a0)
int calc_abs(bf_t **args, bf_t **result) {
	bf_set(*result, *args);
	if ((*args)->sign == 1) bf_neg(*result);
	return EVAL_SUCCESS;
}