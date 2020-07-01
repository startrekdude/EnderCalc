#include "engine.h"
#include "eval.h"

unsigned int map_err(int err);

static int is_trig_result_zero(bf_t *result) {
	return (result->sign == 0 && bf_cmp_lt(result, g_engine_const[ENGINE_CONST_MILLIONTH])) || \
	       (result->sign == 1 && bf_cmp_lt(g_engine_const[ENGINE_CONST_NEG_MILLIONTH], result));
}

// r = sin(a0)
int calc_sin(bf_t **args, bf_t **result) {
	if (g_use_degrees) {
		int err = map_err(bf_mul(*args, *args, g_engine_const[ENGINE_CONST_D2R], g_engine_prec, BF_RNDN));
		if (err != EVAL_SUCCESS) return err;
	}
	
	// if it's below a millionth, we round to zero
	int err = map_err(bf_sin(*result, *args, g_engine_prec, BF_RNDN));
	if (is_trig_result_zero(*result)) bf_set(*result, g_engine_const[ENGINE_CONST_ZERO]);
	return err;
}

// r = cos(a0)
int calc_cos(bf_t **args, bf_t **result) {
	if (g_use_degrees) {
		int err = map_err(bf_mul(*args, *args, g_engine_const[ENGINE_CONST_D2R], g_engine_prec, BF_RNDN));
		if (err != EVAL_SUCCESS) return err;
	}
	
	// if it's below a millionth, we round to zero
	int err = map_err(bf_cos(*result, *args, g_engine_prec, BF_RNDN));
	if (is_trig_result_zero(*result)) bf_set(*result, g_engine_const[ENGINE_CONST_ZERO]);
	return err;
}

// r = tan(a0)
int calc_tan(bf_t **args, bf_t **result) {
	if (g_use_degrees) {
		int err = map_err(bf_mul(*args, *args, g_engine_const[ENGINE_CONST_D2R], g_engine_prec, BF_RNDN));
		if (err != EVAL_SUCCESS) return err;
	}
	int err = map_err(bf_tan(*result, *args, g_engine_prec, BF_RNDN));
	
	// tan can actually give an infinite result - this is considered an error
	if (bf_cmp_lt(g_engine_const[ENGINE_CONST_MILLION], *result)) return EVAL_MATH_ERROR;
	if (is_trig_result_zero(*result)) bf_set(*result, g_engine_const[ENGINE_CONST_ZERO]);
	return err;
}

// r = asin(a0)
int calc_asin(bf_t **args, bf_t **result) {
	int err = map_err(bf_asin(*result, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS || !g_use_degrees) return err;
	return map_err(bf_mul(*result, *result, g_engine_const[ENGINE_CONST_R2D], g_engine_prec, BF_RNDN));
}

// r = acos(a0)
int calc_acos(bf_t **args, bf_t **result) {
	int err = map_err(bf_acos(*result, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS || !g_use_degrees) return err;
	return map_err(bf_mul(*result, *result, g_engine_const[ENGINE_CONST_R2D], g_engine_prec, BF_RNDN));
}

// r = atan(a0)
int calc_atan(bf_t **args, bf_t **result) {
	int err = map_err(bf_atan(*result, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS || !g_use_degrees) return err;
	return map_err(bf_mul(*result, *result, g_engine_const[ENGINE_CONST_R2D], g_engine_prec, BF_RNDN));
}