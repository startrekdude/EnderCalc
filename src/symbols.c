#include "engine.h"
#include "symbols.h"

// THIS FILE CONTAINS DATA ONLY
// (NO FUNCTIONS)
// arrays are indexed by the symbol #defines - i try to make this clear

// This *must* be sorted by length
const sym_str_t g_sym_str[] = {
	{ SYM_PERMU, "arrange", 7},
	{ SYM_COMBI, "choose" , 6},
	{ SYM_ASINH, "asinh"  , 5 },
	{ SYM_ACOSH, "acosh"  , 5 },
	{ SYM_ATANH, "atanh"  , 5 },
	{ SYM_SINH , "sinh"   , 4 },
	{ SYM_COSH , "cosh"   , 4 },
	{ SYM_TANH , "tanh"   , 4 },
	{ SYM_ASIN , "asin"   , 4 },
	{ SYM_ACOS , "acos"   , 4 },
	{ SYM_ATAN , "atan"   , 4 },
	{ SYM_SQRT , "sqrt"   , 4 },
	{ SYM_CBRT , "cbrt"   , 4 },
	{ SYM_ROOT , "root"   , 4 },
	{ SYM_RCL1 , "rcl1"   , 4 },
	{ SYM_RCL2 , "rcl2"   , 4 },
	{ SYM_RCL3 , "rcl3"   , 4 },
	{ SYM_RCL4 , "rcl4"   , 4 },
	{ SYM_RCL5 , "rcl5"   , 4 },
	{ SYM_RCL6 , "rcl6"   , 4 },
	{ SYM_RCL7 , "rcl7"   , 4 },
	{ SYM_RCL8 , "rcl8"   , 4 },
	{ SYM_RCL9 , "rcl9"   , 4 },
	{ SYM_ANS  , "ans"    , 3 },
	{ SYM_EXP  , "exp"    , 3 },
	{ SYM_LOG  , "log"    , 3 },
	{ SYM_TAU  , "tau"    , 3 },
	{ SYM_SIN  , "sin"    , 3 },
	{ SYM_COS  , "cos"    , 3 },
	{ SYM_TAN  , "tan"    , 3 },
	{ SYM_ABS  , "abs"    , 3 },
	{ SYM_2FAC , "!!"     , 2 },
	{ SYM_POW  , "**"     , 2 },
	{ SYM_LN   , "ln"     , 2 },
	{ SYM_PI   , "pi"     , 2 },
	{ SYM_ADD  , "+"      , 1 },
	{ SYM_SUB  , "-"      , 1 },
	{ SYM_MUL  , "*"      , 1 },
	{ SYM_DIV  , "/"      , 1 },
	{ SYM_MOD  , "%"      , 1 },
	{ SYM_POW  , "^"      , 1 },
	{ SYM_FAC  , "!"      , 1 },
	{ SYM_E    , "e"      , 1 },
	{ SYM_PERMU, "P"      , 1 },
	{ SYM_COMBI, "C"      , 1 },
};
const unsigned int g_sym_str_len = 43;

const char * const g_sym_names[] = {
	"ANS",
	"NEG",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"EXP",
	"POW",
	"FAC",
	"SQRT",
	"CBRT",
	"ROOT",
	"LN",
	"LOG",
	"PI",
	"E",
	"TAU",
	"SIN",
	"COS",
	"TAN",
	"ASIN",
	"ACOS",
	"ATAN",
	"SINH",
	"COSH",
	"TANH",
	"ASINH",
	"ACOSH",
	"ATANH",
	"PERMU",
	"COMBI",
	"ABS",
	"RCL1",
	"RCL2",
	"RCL3",
	"RCL4",
	"RCL5",
	"RCL6",
	"RCL7",
	"RCL8",
	"RCL9",
	"2FAC",
};

const char g_sym_const[] = {
	SYMC_NOT_CONSTANT, // ANS
	SYMC_NOT_CONSTANT, // NEG
	SYMC_NOT_CONSTANT, // ADD
	SYMC_NOT_CONSTANT, // SUB
	SYMC_NOT_CONSTANT, // MUL
	SYMC_NOT_CONSTANT, // DIV
	SYMC_NOT_CONSTANT, // MOD
	SYMC_NOT_CONSTANT, // EXP
	SYMC_NOT_CONSTANT, // POW
	SYMC_NOT_CONSTANT, // FAC
	SYMC_NOT_CONSTANT, // SQRT
	SYMC_NOT_CONSTANT, // CBRT
	SYMC_NOT_CONSTANT, // ROOT
	SYMC_NOT_CONSTANT, // LN
	SYMC_NOT_CONSTANT, // LOG
	ENGINE_CONST_PI  , // PI
	ENGINE_CONST_E   , // E
	ENGINE_CONST_TAU , // TAU
	SYMC_NOT_CONSTANT, // SIN
	SYMC_NOT_CONSTANT, // COS
	SYMC_NOT_CONSTANT, // TAN
	SYMC_NOT_CONSTANT, // ASIN
	SYMC_NOT_CONSTANT, // ACOS
	SYMC_NOT_CONSTANT, // ATAN
	SYMC_NOT_CONSTANT, // SINH
	SYMC_NOT_CONSTANT, // COSH
	SYMC_NOT_CONSTANT, // TANH
	SYMC_NOT_CONSTANT, // ASINH
	SYMC_NOT_CONSTANT, // ACOSH
	SYMC_NOT_CONSTANT, // ATANH
	SYMC_NOT_CONSTANT, // PERMU
	SYMC_NOT_CONSTANT, // COMBI
	SYMC_NOT_CONSTANT, // ABS
	SYMC_NOT_CONSTANT, // RCL1
	SYMC_NOT_CONSTANT, // RCL2
	SYMC_NOT_CONSTANT, // RCL3
	SYMC_NOT_CONSTANT, // RCL4
	SYMC_NOT_CONSTANT, // RCL5
	SYMC_NOT_CONSTANT, // RCL6
	SYMC_NOT_CONSTANT, // RCL7
	SYMC_NOT_CONSTANT, // RCL8
	SYMC_NOT_CONSTANT, // RCL9
	SYMC_NOT_CONSTANT, // 2FAC
};

const char g_op_args[] = {
	SYMO_NOT_OPERATOR , // ANS
	SYMO_UNARY_PREFIX , // NEG
	SYMO_BINARY       , // ADD
	SYMO_BINARY       , // SUB
	SYMO_BINARY       , // MUL
	SYMO_BINARY       , // DIV
	SYMO_BINARY       , // MOD
	SYMO_UNARY_PREFIX , // EXP
	SYMO_BINARY       , // POW
	SYMO_UNARY_POSTFIX, // FAC
	SYMO_UNARY_PREFIX , // SQRT
	SYMO_UNARY_PREFIX , // CBRT
	SYMO_BINARY       , // ROOT
	SYMO_UNARY_PREFIX , // LN
	SYMO_BINARY       , // LOG
	SYMO_NOT_OPERATOR , // PI
	SYMO_NOT_OPERATOR , // E
	SYMO_NOT_OPERATOR , // TAU
	SYMO_UNARY_PREFIX , // SIN
	SYMO_UNARY_PREFIX , // COS
	SYMO_UNARY_PREFIX , // TAN
	SYMO_UNARY_PREFIX , // ASIN
	SYMO_UNARY_PREFIX , // ACOS
	SYMO_UNARY_PREFIX , // ATAN
	SYMO_UNARY_PREFIX , // SINH
	SYMO_UNARY_PREFIX , // COSH
	SYMO_UNARY_PREFIX , // TANH
	SYMO_UNARY_PREFIX , // ASINH
	SYMO_UNARY_PREFIX , // ACOSH
	SYMO_UNARY_PREFIX , // ATANH
	SYMO_BINARY       , // PERMU
	SYMO_BINARY       , // COMBI
	SYMO_UNARY_PREFIX , // ABS
	SYMA_NOT_OPERATOR , // RCL1
	SYMA_NOT_OPERATOR , // RCL2
	SYMA_NOT_OPERATOR , // RCL3
	SYMA_NOT_OPERATOR , // RCL4
	SYMA_NOT_OPERATOR , // RCL5
	SYMA_NOT_OPERATOR , // RCL6
	SYMA_NOT_OPERATOR , // RCL7
	SYMA_NOT_OPERATOR , // RCL8
	SYMA_NOT_OPERATOR , // RCL9
	SYMO_UNARY_POSTFIX, // 2FAC
};

const unsigned int g_op_precedence[] = {
	SYMP_NOT_OPERATOR, // ANS
	0                , // NEG
	5                , // ADD
	5                , // SUB
	4                , // MUL
	4                , // DIV
	4                , // MOD
	0                , // EXP
	3                , // POW
	1                , // FAC
	0                , // SQRT
	0                , // CBRT
	3                , // ROOT
	0                , // LN
	2                , // LOG
	SYMP_NOT_OPERATOR, // PI
	SYMP_NOT_OPERATOR, // E
	SYMP_NOT_OPERATOR, // TAU
	0                , // SIN
	0                , // COS
	0                , // TAN
	0                , // ASIN
	0                , // ACOS
	0                , // ATAN
	0                , // SINH
	0                , // COSH
	0                , // TANH
	0                , // ASINH
	0                , // ACOSH
	0                , // ATANH
	2                , // PERMU
	2                , // COMBI
	0                , // ABS
	SYMP_NOT_OPERATOR, // RCL1
	SYMP_NOT_OPERATOR, // RCL2
	SYMP_NOT_OPERATOR, // RCL3
	SYMP_NOT_OPERATOR, // RCL4
	SYMP_NOT_OPERATOR, // RCL5
	SYMP_NOT_OPERATOR, // RCL6
	SYMP_NOT_OPERATOR, // RCL7
	SYMP_NOT_OPERATOR, // RCL8
	SYMP_NOT_OPERATOR, // RCL9
	1                , // 2FAC
};

const char g_op_associativity[] = {
	SYMA_NOT_OPERATOR, // ANS
	SYMA_RIGHT       , // NEG
	SYMA_LEFT        , // ADD
	SYMA_LEFT        , // SUB
	SYMA_LEFT        , // MUL
	SYMA_LEFT        , // DIV
	SYMA_LEFT        , // MOD
	SYMA_RIGHT       , // EXP
	SYMA_RIGHT       , // POW
	SYMA_LEFT        , // FAC
	SYMA_RIGHT       , // SQRT
	SYMA_RIGHT       , // CBRT
	SYMA_LEFT        , // ROOT
	SYMA_RIGHT       , // LN
	SYMA_LEFT        , // LOG
	SYMA_NOT_OPERATOR, // PI
	SYMA_NOT_OPERATOR, // E
	SYMA_NOT_OPERATOR, // TAU
	SYMA_RIGHT       , // SIN
	SYMA_RIGHT       , // COS
	SYMA_RIGHT       , // TAN
	SYMA_RIGHT       , // ASIN
	SYMA_RIGHT       , // ACOS
	SYMA_RIGHT       , // ATAN
	SYMA_RIGHT       , // SINH
	SYMA_RIGHT       , // COSH
	SYMA_RIGHT       , // TANH
	SYMA_RIGHT       , // ASINH
	SYMA_RIGHT       , // ACOSH
	SYMA_RIGHT       , // ATANH
	SYMA_LEFT        , // PERMU
	SYMA_LEFT        , // COMBI
	SYMA_RIGHT       , // ABS
	SYMA_NOT_OPERATOR, // RCL1
	SYMA_NOT_OPERATOR, // RCL2
	SYMA_NOT_OPERATOR, // RCL3
	SYMA_NOT_OPERATOR, // RCL4
	SYMA_NOT_OPERATOR, // RCL5
	SYMA_NOT_OPERATOR, // RCL6
	SYMA_NOT_OPERATOR, // RCL7
	SYMA_NOT_OPERATOR, // RCL8
	SYMA_NOT_OPERATOR, // RCL9
	SYMA_LEFT        , // 2FAC
};

const arg_collect_t g_op_arg_collect[] = {
	{1, {1, 0} , {1, 0} , 0 }, // SYMO_UNARY_PREFIX
	{1, {-1, 0}, {-1, 0}, -1}, // SYMO_UNARY_POSTFIX
	{2, {-1, 1}, {1, -1}, -1}, // SYMO_BINARY
};

// actual math stuff - took a while to get here
op_func_t * const g_op_funcs[] = {
	0         , // ANS
	calc_neg  , // NEG
	calc_add  , // ADD
	calc_sub  , // SUB
	calc_mul  , // MUL
	calc_div  , // DIV
	calc_mod  , // MOD
	calc_exp  , // EXP
	calc_pow  , // POW
	calc_fac  , // FAC
	calc_sqrt , // SQRT
	calc_cbrt , // CBRT
	calc_root , // ROOT
	calc_ln   , // LN
	calc_log  , // LOG
	0         , // PI
	0         , // E
	0         , // TAU
	calc_sin  , // SIN
	calc_cos  , // COS
	calc_tan  , // TAN
	calc_asin , // ASIN
	calc_acos , // ACOS
	calc_atan , // ATAN
	calc_sinh , // SINH
	calc_cosh , // COSH
	calc_tanh , // TANH
	calc_asinh, // ASINH
	calc_acosh, // ACOSH
	calc_atanh, // ATANH
	calc_permu, // PERMU
	calc_combi, // COMBI
	calc_abs  , // ABS
	0         , // RCL1
	0         , // RCL2
	0         , // RCL3
	0         , // RCL4
	0         , // RCL5
	0         , // RCL6
	0         , // RCL7
	0         , // RCL8
	0         , // RCL9
	calc_2fac , // 2FAC
};