#include "engine.h"
#include "eval.h"

unsigned int map_err(int err);

// r = a0 P a1
int calc_permu(bf_t **args, bf_t **result) {
	bf_t *scratch = engine_num_alloc();
	bf_t *bottom_fac = engine_num_alloc();
	
	int err = map_err(bf_sub(scratch, args[0], args[1], g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(scratch);
		engine_num_free(bottom_fac);
		return err;
	}
	
	err = calc_fac(&scratch, &bottom_fac);
	if (err != EVAL_SUCCESS) {
		engine_num_free(scratch);
		engine_num_free(bottom_fac);
		return err;
	}
	
	err = calc_fac(args, &scratch);
	if (err != EVAL_SUCCESS) {
		engine_num_free(scratch);
		engine_num_free(bottom_fac);
		return err;
	}
	
	err = map_err(bf_div(*result, scratch, bottom_fac, g_engine_prec, BF_RNDN));
	engine_num_free(scratch);
	engine_num_free(bottom_fac);
	return err;
}

// r = a0 C a1
int calc_combi(bf_t **args, bf_t **result) {
	bf_t *top = engine_num_alloc();
	bf_t *arg0_bkup = engine_num_alloc();
	bf_set(arg0_bkup, *args);
	int err = calc_fac(&arg0_bkup, &top);
	engine_num_free(arg0_bkup);
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		return err;
	}
	
	bf_t *scratch1 = engine_num_alloc();
	bf_t *arg_bkup = engine_num_alloc();
	bf_set(arg_bkup, args[1]);
	err = calc_fac(&arg_bkup, &scratch1);
	engine_num_free(arg_bkup);
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		engine_num_free(scratch1);
		return err;
	}
	
	bf_t *scratch2 = engine_num_alloc();
	
	err = map_err(bf_sub(scratch2, args[0], args[1], g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		engine_num_free(scratch1);
		engine_num_free(scratch2);
		return err;
	}
	
	err = calc_fac(&scratch2, &args[1]);
	engine_num_free(scratch2);
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		engine_num_free(scratch1);
		return err;
	}
	
	err = map_err(bf_mul(scratch1, scratch1, args[1], g_engine_prec, BF_RNDN));
	if (err != EVAL_SUCCESS) {
		engine_num_free(top);
		engine_num_free(scratch1);
		return err;
	}
	
	err = map_err(bf_div(*result, top, scratch1, g_engine_prec, BF_RNDN));
	engine_num_free(top);
	engine_num_free(scratch1);
	return err;
}