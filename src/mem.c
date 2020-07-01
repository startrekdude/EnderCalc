#include <stdlib.h>
#include <string.h>

#include "calc.h"
#include "command.h"
#include "engine.h"
#include "eval.h"
#include "mem.h"
#include "minini.h"

// slots 1 thru 9 - init with nullptrs
bf_t *g_mem[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// free the entire memory
void free_mem() {
	for (unsigned int i = 0; i < CALC_MEMORY_SIZE; i++) {
		bf_t *num = g_mem[i];
		if (num != 0) engine_num_free(num);
		g_mem[i] = 0;
	}
}

// Store memory slot to the ini file
static void sto_slot(unsigned int i) {
	if (g_config_path == 0) return;
	
	// first, the key name in the ini file (4 chars + null)
	char key[5];
	sprintf(key, "mem%u", i + 1);
	
	// now, are we storing to ini or clearing?
	bf_t *num = g_mem[i];
	if (num == 0) {
		ini_puts("Memory", key, 0, g_config_path);
	} else {
		char *s = bf_ftoa(0, num, 16, g_engine_prec, BF_FTOA_FORMAT_FREE);
		ini_puts("Memory", key, s, g_config_path);
		free(s);
	}
}

// Store a number into a memory slot
// (i should be between 0 and 8, inclusive)
static void do_sto(unsigned int i, char *s) {
	// mode 1, add the last answer (if no number specified, skipping spaces)
	while (*s == ' ') s++;
	if (*s == '\0') {
		if (g_last_answer == 0) {
			puts("No saved answer.");
		} else {
			// free the slot and copy it over
			if (g_mem[i] == 0) g_mem[i] = engine_num_alloc();
			bf_set(g_mem[i], g_last_answer);
			engine_num_print(g_mem[i], 0, g_suppress_scientific_notation);
		}
		sto_slot(i);
		return;
	}
	
	// mode 2, try to parse a number
	g_mem[i] = engine_num_alloc();
	char *end;
	bf_atof(g_mem[i], s, (const char**) &end, 10, g_engine_prec, BF_RNDN);
	if (*end != '\0') {
		printf("%s: not a number\n", s);
		engine_num_free(g_mem[i]);
		g_mem[i] = 0;
	} else {
		engine_num_print(g_mem[i], 0, g_suppress_scientific_notation);
	}
	sto_slot(i);
}

// Recall a single number from memory
static void do_rcl(unsigned int i) {
	bf_t *num = g_mem[i];
	printf("MEM%u = ", i + 1);
	if (num == 0) {
		puts("(empty)");
	} else {
		engine_num_print(num, 0, g_suppress_scientific_notation);
	}
}

// Clear a single memory slot
static void do_clr(unsigned int i) {
	bf_t *num = g_mem[i];
	if (num != 0) engine_num_free(num);
	g_mem[i] = 0;
	printf("Cleared slot %u.\n", i + 1);
	sto_slot(i);
}

unsigned int mem_run_command(char *s) {
	// the store and recall commands - /stoX and /rclX
	// also, new /clrX
	unsigned int is_sto = strncmp("sto", s, 3) == 0;
	unsigned int is_rcl = strncmp("rcl", s, 3) == 0;
	unsigned int is_clr = strncmp("clr", s, 3) == 0;
	if (is_sto || is_rcl || is_clr) {
		char c = s[3];
		if (c >= '1' && c <= '9') {
			unsigned int i = c - '1';
			if (is_sto) do_sto(i, &s[4]);
			if (is_rcl) do_rcl(i);
			if (is_clr) do_clr(i);
		} else {
			printf("%c: invalid memory slot\n", c);
		}
		return 1;
	}
	return 0;
}

// commands executed directly by command.c
void do_clear_mem(char *s) {
	free_mem();
	if (g_config_path)
		ini_puts("Memory", 0, 0, g_config_path);
	puts("Cleared memory.");
}

void do_show_mem(char *s) {
	for (unsigned int i = 0; i < CALC_MEMORY_SIZE; i++) {
		do_rcl(i);
	}
}

// called for ini sections
void mem_try_load(const char *section, const char *key, const char *value) {
	if (strcmp(section, "Memory") == 0 && strncmp(key, "mem", 3) == 0) {
		char c = key[3];
		if (c >= '1' && c <= '9') {
			unsigned int i = c - '1';
			bf_t *num = engine_num_alloc();
			char *end;
			bf_atof(num, value, (const char**) &end, 16, g_engine_prec, BF_RNDN);
			if (*end == '\0') {
				g_mem[i] = num;
			} else {
				engine_num_free(num);
			}
		}
	}
}