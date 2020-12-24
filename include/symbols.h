#ifndef _INCLUDED_SYMBOLS_H
#define _INCLUDED_SYMBOLS_H

// Symbols with special handling
#define SYM_ANS 0
#define SYM_NEG 1

// Basic operators
#define SYM_ADD 2
#define SYM_SUB 3
#define SYM_MUL 4
#define SYM_DIV 5
#define SYM_MOD 6
#define SYM_EXP 7
#define SYM_POW 8
#define SYM_FAC 9

// Root operations
#define SYM_SQRT 10
#define SYM_CBRT 11
#define SYM_ROOT 12

// Logarithms
#define SYM_LN 13
#define SYM_LOG 14

// Constants
#define SYM_PI 15
#define SYM_E 16
#define SYM_TAU 17

// Trig operations
#define SYM_SIN 18
#define SYM_COS 19
#define SYM_TAN 20
#define SYM_ASIN 21
#define SYM_ACOS 22
#define SYM_ATAN 23

// sinh et al
#define SYM_SINH 24
#define SYM_COSH 25
#define SYM_TANH 26
#define SYM_ASINH 27
#define SYM_ACOSH 28
#define SYM_ATANH 29

// Stats operations
#define SYM_PERMU 30
#define SYM_COMBI 31

// Misc
#define SYM_ABS 32

// Recall operations (interact with memory)
#define SYM_RCL1 33
#define SYM_RCL2 34
#define SYM_RCL3 35
#define SYM_RCL4 36
#define SYM_RCL5 37
#define SYM_RCL6 38
#define SYM_RCL7 39
#define SYM_RCL8 40
#define SYM_RCL9 41

// Symbols added after v1.0
#define SYM_2FAC 42 // double factorial

#define SYM_MAX SYM_2FAC
#define SYM_RCL_MIN SYM_RCL1
#define SYM_RCL_MAX SYM_RCL9

typedef struct {
	unsigned int sym; // the symbol referred to
	char str[7]; // the string that should parse as the symbol
	unsigned char len; // length
} sym_str_t;

extern const sym_str_t g_sym_str[];
extern const unsigned int g_sym_str_len;

// translation table for display
extern const char * const g_sym_names[];

#define SYMC_NOT_CONSTANT -1

// constant symbols -> engine constants
extern const char g_sym_const[];

#define SYMO_NOT_OPERATOR  -1
#define SYMO_UNARY_PREFIX  0
#define SYMO_UNARY_POSTFIX 1
#define SYMO_BINARY        2

extern const char g_op_args[];

#define SYMP_NOT_OPERATOR 0xDEAD

// lower is higher
extern const unsigned int g_op_precedence[];

#define SYMA_NOT_OPERATOR -1
#define SYMA_LEFT         0
#define SYMA_RIGHT        1

extern const char g_op_associativity[];

// this structures describes how to collect the arguments for an operator
typedef struct {
	unsigned char len; // number of args
	char args[2]; // relative indices to pull args from
	char cleanup[2]; // relative indices to cleanup, compensating for shift
	char result; // after cleanup, relative index to replace with result
} arg_collect_t;

// indexed by SYMO values
extern const arg_collect_t g_op_arg_collect[];

// OMG the actual math - took days to get this far, ha
typedef int (op_func_t)(bf_t**, bf_t**);

extern op_func_t * const g_op_funcs[];

#endif