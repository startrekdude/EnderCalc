#include <stdio.h>

#include "calc.h"
#include "engine.h"
#include "libbf.h"
#include "readline.h"
#include "stats.h"
#include "vector.h"

// the statistics module of the calculator
// calculates sample or population mean, variance, and standard deviation

#define POPULATION 0
#define SAMPLE 1

// remove any remaining input from stdin
static void clear_stdin() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}

static unsigned int sample_or_population() {
	while (1) {
		printf("\r(P)opulation or (S)ample? ");
		char c = getchar() & ~32; // convert to uppercase
		clear_stdin();
		if (c == 'P') return POPULATION;
		if (c == 'S') return SAMPLE;
	}
}

// Free a dataset
static void free_dataset(vector_t ds) {
	unsigned int len = vector_get_len(ds);
	for (unsigned int i = 0; i < len; i++) {
		bf_t *num = (bf_t*) vector_get(ds, i);
		engine_num_free(num);
	}
	vector_delete(ds);
}

// Read a dataset from the user; allow multiple of same number using 4.5|50 syntax
static unsigned int read_dataset(vector_t *ds) {
	*ds = vector_new();
	char *line, *original_line;
	line = original_line = readline("Dataset? ");
	
	while (*line != '\0') {
		while (*line == ' ') line++; // skip spaces
		
		char c = *line;
		unsigned char was_pipe = 0;
		if (c == '-' || c == '|') {
			if (c == '-') c = line[1];
			if (c == '|' && vector_get_len(*ds) > 0) {
				was_pipe = 1;
				line++;
				c = *line;
			}
		}
		if (c < '0' || c > '9') {
			printf("Invalid character: %c\n", c);
			free_dataset(*ds);
			free(original_line);
			return 1;
		}
		
		if (was_pipe) {
			bf_t *repeat = (bf_t*) vector_get_last(*ds);
			long int count = strtol(line, &line, 10) - 1;
			if (count <= 0 || count > 65535) {
				printf("Invalid value for repeat: %d\n", count + 1);
				free_dataset(*ds);
				free(original_line);
				return 1;
			}
			while (count) {
				bf_t *num = engine_num_alloc();
				bf_set(num, repeat);
				vector_add(*ds, num);
				count--;
			}
			continue;
		}
		
		// handle repeated datapoints via the 4.3|5 syntax
		if (c == '|') {
			*line++;
			c = *line;
		}
		
		bf_t *num = engine_num_alloc();
		vector_add(*ds, num);
		bf_atof(num, line, (const char**) &line, 10, g_engine_prec, BF_RNDN);
	}
	
	free(original_line);
	return 0;
}

// Calculate the mean of a dataset
static bf_t* calc_mean(vector_t ds) {
	unsigned int len = vector_get_len(ds);
	
	bf_t *den = engine_num_alloc();
	bf_set_ui(den, len);
	
	bf_t *acc = engine_num_alloc();
	for (unsigned int i = 0; i < len; i++) {
		bf_t *num = (bf_t*) vector_get(ds, i);
		bf_add(acc, acc, num, g_engine_prec, BF_RNDN);
	}
	
	bf_div(acc, acc, den, g_engine_prec, BF_RNDN);
	engine_num_free(den);
	return acc;
}

// Calculate the variance (messes up the data set for performance)
static bf_t* calc_variance(vector_t ds, bf_t *mean, unsigned int ds_type) {
	unsigned int len = vector_get_len(ds);
	
	bf_t *den = engine_num_alloc();
	bf_set_ui(den, len);
	if (ds_type == SAMPLE) bf_sub(den, den, g_engine_const[ENGINE_CONST_ONE], g_engine_prec, BF_RNDN);
	
	bf_t *acc = engine_num_alloc();
	for (unsigned int i = 0; i < len; i++) {
		bf_t *num = (bf_t*) vector_get(ds, i);
		bf_sub(num, num, mean, g_engine_prec, BF_RNDN);
		bf_mul(num, num, num , g_engine_prec, BF_RNDN);
		bf_add(acc, acc, num , g_engine_prec, BF_RNDN);
	}
	
	bf_div(acc, acc, den, g_engine_prec, BF_RNDN);
	engine_num_free(den);
	return acc;
}

void do_stats(char *s) {
	unsigned int ds_type = sample_or_population();
	vector_t ds;
	unsigned int err = read_dataset(&ds);
	if (err == 1) return;
	
	unsigned int len = vector_get_len(ds);
	printf("Dataset has %u point(s).\n", len);
	if (len == 0) {
		free_dataset(ds);
		return;
	}
	
	bf_t *mean = calc_mean(ds);
	printf("Mean: ");
	engine_num_print(mean, 0, g_suppress_scientific_notation);
	
	bf_t *variance = calc_variance(ds, mean, ds_type);
	printf("Variance: ");
	engine_num_print(variance, 0, g_suppress_scientific_notation);
	
	bf_t *std_dev = engine_num_alloc();
	bf_sqrt(std_dev, variance, g_engine_prec, BF_RNDN);
	printf("Standard Deviation: ");
	engine_num_print(std_dev, 0, g_suppress_scientific_notation);
	
	engine_num_free(mean);
	engine_num_free(variance);
	engine_num_free(std_dev);
	free_dataset(ds);
}