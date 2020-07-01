#ifndef _INCLUDED_MEM_H
#define _INCLUDED_MEM_H

#include "libbf.h"

// the calculator can store numbers in memory
// these also get written to the ini file
extern bf_t *g_mem[];
#define CALC_MEMORY_SIZE 9

void free_mem();

unsigned int mem_run_command(char *s);

// commands executed directly by command.c
void do_clear_mem(char *s);
void do_show_mem(char *s);

// for loading memory from the ini file
void mem_try_load(const char *section, const char *key, const char *value);

#endif