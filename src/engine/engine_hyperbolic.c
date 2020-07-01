#include "engine.h"
#include "eval.h"

unsigned int map_err(int err);

static int internal_calc_sinh_or_cosh(bf_t **args, bf_t **result, unsigned char use_cosh) {
	// limits - we don't want the calculator to hang
	if (bf_cmp_lt(g_engine_const[ENGINE_CONST_MAX_HYP], *args)) return EVAL_MATH_ERROR;
	if (bf_cmp_lt(*args, g_engine_const[ENGINE_CONST_MIN_HYP])) return EVAL_MATH_ERROR;
	
	bf_t *exp_pos = engine_num_alloc();
	bf_t *exp_neg = engine_num_alloc();
	
	int err = map_err(bf_exp(exp_pos, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(exp_pos);
		engine_num_free(exp_neg);
		return err;
	}
	
	bf_neg(*args);
	err = map_err(bf_exp(exp_neg, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(exp_pos);
		engine_num_free(exp_neg);
		return err;
	}
	
	if (use_cosh) {
		err = map_err(bf_add(*result, exp_pos, exp_neg, g_engine_prec, BF_RNDN));
	} else {
		err = map_err(bf_sub(*result, exp_pos, exp_neg, g_engine_prec, BF_RNDN));
	}
	engine_num_free(exp_pos);
	engine_num_free(exp_neg);
	if (err != EVAL_SUCCESS) return err;
	
	return map_err(bf_div(*result, *result, g_engine_const[ENGINE_CONST_TWO], g_engine_prec, BF_RNDN));
}

// r = sinh(a0)
int calc_sinh(bf_t **args, bf_t **result) {
	return internal_calc_sinh_or_cosh(args, result, 0);
}

// r = cosh(a0)
int calc_cosh(bf_t **args, bf_t **result) {
	return internal_calc_sinh_or_cosh(args, result, 1);
}

// r = tanh(a0)
int calc_tanh(bf_t **args, bf_t **result) {
	if (bf_cmp_lt(g_engine_const[ENGINE_CONST_MAX_HYP], *args)) return EVAL_MATH_ERROR;
	if (bf_cmp_lt(*args, g_engine_const[ENGINE_CONST_MIN_HYP])) return EVAL_MATH_ERROR;
	
	bf_t *exp_pos = engine_num_alloc();
	bf_t *exp_neg = engine_num_alloc();
	
	int err = map_err(bf_exp(exp_pos, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(exp_pos);
		engine_num_free(exp_neg);
		return err;
	}
	
	bf_neg(*args);
	err = map_err(bf_exp(exp_neg, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(exp_pos);
		engine_num_free(exp_neg);
		return err;
	}
	
	bf_t *top = engine_num_alloc();
	bf_t *bottom = engine_num_alloc();
	err = map_err(bf_sub(top, exp_pos, exp_neg, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(exp_pos);
		engine_num_free(exp_neg);
		engine_num_free(top);
		engine_num_free(bottom);
		return err;
	}
	
	err = map_err(bf_add(bottom, exp_pos, exp_neg, g_engine_prec, BF_RNDN));
	engine_num_free(exp_pos);
	engine_num_free(exp_neg);
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		engine_num_free(bottom);
		return err;
	}
	
	err = map_err(bf_div(*result, top, bottom, g_engine_prec, BF_RNDN));
	engine_num_free(top);
	engine_num_free(bottom);
	return err;
}

static int internal_calc_asinh_or_acosh(bf_t **args, bf_t **result, unsigned char use_acosh) {
	// fast-fail for <1 on acosh
	if (use_acosh && bf_cmp_lt(*args, g_engine_const[ENGINE_CONST_ONE])) return EVAL_MATH_ERROR;
	if (!use_acosh && bf_cmp_eq(*args, g_engine_const[ENGINE_CONST_ZERO])) {
		bf_set(*result, g_engine_const[ENGINE_CONST_ZERO]);
		return EVAL_SUCCESS;
	}
	
	bf_t *sqrt = engine_num_alloc();
	bf_set(sqrt, *args);
	
	int err = map_err(bf_mul(sqrt, sqrt, sqrt, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(sqrt);
		return err;
	}
	
	if (use_acosh) {
		err = map_err(bf_sub(sqrt, sqrt, g_engine_const[ENGINE_CONST_ONE], g_engine_prec, BF_RNDN));
	} else {
		err = map_err(bf_add(sqrt, sqrt, g_engine_const[ENGINE_CONST_ONE], g_engine_prec, BF_RNDN));
	}
	if (err != EVAL_SUCCESS) {
		engine_num_free(sqrt);
		return err;
	}
	
	err = map_err(bf_sqrt(*result, sqrt, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(sqrt);
		return err;
	}
	
	err = map_err(bf_add(*args, *args, *result, g_engine_prec, BF_RNDN));
	engine_num_free(sqrt);
	if (err != EVAL_SUCCESS) return err;
	
	return map_err(bf_log(*result, *args, g_engine_prec, BF_RNDN));
}

// r = asinh(a0)
int calc_asinh(bf_t **args, bf_t **result) {
	return internal_calc_asinh_or_acosh(args, result, 0);
}

// r = acosh(a0)
int calc_acosh(bf_t **args, bf_t **result) {
	return internal_calc_asinh_or_acosh(args, result, 1);
}

int calc_atanh(bf_t **args, bf_t **result) {
	bf_t *top = engine_num_alloc();
	bf_t *bottom = engine_num_alloc();
	bf_set(top, g_engine_const[ENGINE_CONST_ONE]);
	bf_set(bottom, g_engine_const[ENGINE_CONST_ONE]);
	
	int err = map_err(bf_add(top, top, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		engine_num_free(bottom);
		return err;
	}
	
	err = map_err(bf_sub(bottom, bottom, *args, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		engine_num_free(bottom);
		return err;
	}
	
	err = map_err(bf_div(*result, top, bottom, g_engine_prec, BF_RNDN));
	engine_num_free(top);
	engine_num_free(bottom);
	if (err != EVAL_SUCCESS) return err;
	
	err = map_err(bf_log(*args, *result, g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) return err;
	
	err = map_err(bf_mul(*result, *args, g_engine_const[ENGINE_CONST_HALF], g_engine_prec, BF_RNDN));
	return err;
}
