#ifndef _INCLUDED_ENGINE_H
#define _INCLUDED_ENGINE_H

#include "libbf.h"

#define ENGINE_CONST_PI            0
#define ENGINE_CONST_E             1
#define ENGINE_CONST_TAU           2
#define ENGINE_CONST_MAX_FAC       3
#define ENGINE_CONST_ONE           4
#define ENGINE_CONST_THIRD         5
#define ENGINE_CONST_TWO           6
#define ENGINE_CONST_D2R           7
#define ENGINE_CONST_MILLION       8
#define ENGINE_CONST_MILLIONTH     9
#define ENGINE_CONST_NEG_MILLIONTH 10
#define ENGINE_CONST_ZERO          11
#define ENGINE_CONST_R2D           12
#define ENGINE_CONST_MAX_HYP       13
#define ENGINE_CONST_MIN_HYP       14
#define ENGINE_CONST_HALF          15
#define ENGINE_CONST_MAX_POW_BASE  16

// the global context used for all math operations
extern bf_context_t g_engine_ctx;

// The precision used for the binary engine (in bits)
extern limb_t g_engine_prec;

// The precision used for the decimal engine (in decimals)
// this is /only/ used for display, and should be
// slightly lower than the binary engine -
// this is because binary can't actually represent certain
// numbers, so we wanna round before displaying them
extern limb_t g_dec_engine_prec;

extern bf_t *g_engine_const[];

void engine_init();
void engine_cleanup();

bf_t* engine_num_alloc();
void engine_num_free(bf_t *num);

void engine_num_print(bf_t *num, unsigned int show_real, unsigned int suppress_scientific_mode);

bf_t* engine_copy_const(unsigned int i);

// 0 = radians, 1 = degrees
extern unsigned char g_use_degrees;

// the functions that actually do things
int calc_neg(bf_t **args, bf_t **result);
int calc_add(bf_t **args, bf_t **result);
int calc_sub(bf_t **args, bf_t **result);
int calc_mul(bf_t **args, bf_t **result);
int calc_div(bf_t **args, bf_t **result);
int calc_mod(bf_t **args, bf_t **result);
int calc_exp(bf_t **args, bf_t **result);
int calc_pow(bf_t **args, bf_t **result);
int calc_fac(bf_t **args, bf_t **result);
int calc_sqrt(bf_t **args, bf_t **result);
int calc_cbrt(bf_t **args, bf_t **result);
int calc_root(bf_t **args, bf_t **result);
int calc_ln(bf_t **args, bf_t **result);
int calc_log(bf_t **args, bf_t **result);
int calc_sin(bf_t **args, bf_t **result);
int calc_cos(bf_t **args, bf_t **result);
int calc_tan(bf_t **args, bf_t **result);
int calc_asin(bf_t **args, bf_t **result);
int calc_acos(bf_t **args, bf_t **result);
int calc_atan(bf_t **args, bf_t **result);
int calc_sinh(bf_t **args, bf_t **result);
int calc_cosh(bf_t **args, bf_t **result);
int calc_tanh(bf_t **args, bf_t **result);
int calc_asinh(bf_t **args, bf_t **result);
int calc_acosh(bf_t **args, bf_t **result);
int calc_atanh(bf_t **args, bf_t **result);
int calc_permu(bf_t **args, bf_t **result);
int calc_combi(bf_t **args, bf_t **result);
int calc_abs(bf_t **args, bf_t **result);
int calc_2fac(bf_t **args, bf_t **result);

#endif