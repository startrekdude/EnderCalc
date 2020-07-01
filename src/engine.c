#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"
#include "eval.h"

// see header file
bf_context_t g_engine_ctx = {0};
limb_t g_engine_prec = 100;
limb_t g_dec_engine_prec = 20;
unsigned char g_use_degrees = 0;

// Precision for the table of constants
static const limb_t engine_const_prec = 150;

// The constant table
static const char * const engine_const_str[] = {
	"3.1415926535897932384626433832795029", // PI
	"2.7182818284590452353602874713526625", // E
	"6.2831853071795864769252867665590058", // TAU
	"3072000"                             , // MAX_FAC
	"1"                                   , // ONE
	"0.3333333333333333333333333333333333", // THIRD
	"2"                                   , // TWO
	"0.0174532925199432957692369076848861", // D2R
	"1000000"                             , // MILLION
	"0.0000001"                           , // MILLIONTH
	"-0.0000001"                          , // NEG_MILLIONTH
	"0"                                   , // ZERO
	"57.295779513082320876798154814105170", // R2D
	"12000000"                            , // MAX_HYP
	"-12000000"                           , // MIN_HYP
	"0.5"                                 , // HALF
	"9e1900000"                           , // MAX_POW_BASE
};
static const unsigned int engine_const_len = 17;

bf_t *g_engine_const[17];

static void* engine_bf_realloc(void *opaque, void *ptr, size_t size) {
	return realloc(ptr, size);
}

void engine_init() {
	bf_context_init(&g_engine_ctx, engine_bf_realloc, 0);
	
	// Initialize the constant table
	for (unsigned int i = 0; i < engine_const_len; i++) {
		bf_t *num = engine_num_alloc();
		bf_atof(num, engine_const_str[i], (const char**) 0, 10, engine_const_prec, 0);
		g_engine_const[i] = num;
	}
}

void engine_cleanup() {
	bf_context_end(&g_engine_ctx);
	
	for (unsigned int i = 0; i < engine_const_len; i++) {
		engine_num_free(g_engine_const[i]);
	}
}

// make a new number
bf_t* engine_num_alloc() {
	bf_t *num = (bf_t*) malloc(sizeof(bf_t));
	bf_init(&g_engine_ctx, num);
	return num;
}

// free a number
void engine_num_free(bf_t *num) {
	bf_delete(num);
	free(num);
}

// Print a number - this uses the decimal engine
// display a result - convert to lower-accuracy decimal fp first
void engine_num_print(bf_t *num, unsigned int show_real, unsigned int suppress_scientific_mode) {
	char *bin_s = bf_ftoa(0, num, 10, g_engine_prec, BF_FTOA_FORMAT_FRAC);
	
	// debug option: display the binary result
	if (show_real) printf("(Actual Result: %s)\n", bin_s);
	
	bfdec_t dec;
	bfdec_init(&g_engine_ctx, &dec);
	bfdec_atof(&dec, bin_s, (const char**) 0, g_dec_engine_prec, BF_RNDN);
	free(bin_s);
	
	char *dec_s = bfdec_ftoa(0, &dec, g_dec_engine_prec, suppress_scientific_mode ? BF_FTOA_FORMAT_FRAC : BF_FTOA_FORMAT_FREE_MIN);
	
	// Okay, so, for dec_s when suppressing scientfic notation, there may be trailing zeros
	// Here we try and get rid of them
	if (suppress_scientific_mode) {
		char *from_end = &dec_s[strlen(dec_s)-1];
		while (*from_end == '0' && from_end > dec_s) from_end--;
		if (*from_end == '.') from_end--;
		from_end++;
		*from_end = '\0';
	}
	
	puts(dec_s);
	
	free(dec_s);
	bfdec_delete(&dec);
}

// Copy a constant
bf_t* engine_copy_const(unsigned int i) {
	bf_t *num = engine_num_alloc();
	bf_t *c = g_engine_const[i];
	bf_set(num, c);
	bf_round(num, g_engine_prec, BF_RNDN);
	return num;
}

// convert a libbf error into an evaluation error
unsigned int map_err(int err) {
	if (err & BF_ST_DIVIDE_ZERO) return EVAL_ZERO_DIVISION;
	if (err & BF_ST_INVALID_OP) return EVAL_MATH_ERROR;
	return EVAL_SUCCESS;
}