#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HELP_USE_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#endif

#include "calc.h"
#include "command.h"
#include "engine.h"
#include "eval.h"
#include "mem.h"
#include "minini.h"
#include "parse.h"
#include "stats.h"

char *g_config_path = (char*) 0; // set by calc.c

static inline const char* on_off(unsigned int x) {
	return x ? "On" : "Off";
}

static void do_parsedebug(char *s) {
	g_parse_debug ^= 1;
	
	if (g_config_path)
		ini_putl("EnderCalc", "parse_debug", g_parse_debug, g_config_path);
	
	printf("Parse debug: %s\n", on_off(g_parse_debug));
}

static void do_degrees(char *s) {
	g_use_degrees ^= 1;
	
	if (g_config_path)
		ini_putl("EnderCalc", "use_degrees", g_use_degrees, g_config_path);
	
	printf("Toggled, trigonometric functions now use %s.\n", g_use_degrees ? "degrees" : "radians");
}

static void do_format(char *s) {
	g_suppress_scientific_notation ^= 1;
	
	if (g_config_path)
		ini_putl("EnderCalc", "suppress_scientific_notation", g_suppress_scientific_notation, g_config_path);
	
	printf("Suppress Scientific Notation: %s\n", on_off(g_suppress_scientific_notation));
}

// Print the last calculated answer in whichever formatting mode *isn't* active
static void do_format_last(char *s) {
	if (g_last_answer == 0) {
		puts("No saved answer.");
		return;
	}
	
	engine_num_print(g_last_answer, 0, g_suppress_scientific_notation ^ 1);
}

static void do_debug(char *s) {
	g_debug ^= 1;
	
	if (g_config_path)
		ini_putl("EnderCalc", "debug", g_debug, g_config_path);
	
	printf("Debug: %s\n", on_off(g_debug));
}

// Parse as a positive long int, return -1 for failure
// Nothing should be after 
// Precision should be at least 1
static long int parse_prec(const char *s) {
	char *end = 0;
	long int num = strtol(s, &end, 10);
	if (*end != '\0' || num < 1) return -1;
	return num;
}

static void print_prec_err(char *s) {
	printf("Invalid precision: %s\n", s);
	puts("Precision must be a natural number.");
}

static void do_bprec(char *s) {
	long int prec = parse_prec(s);
	if (prec == -1) {
		print_prec_err(s);
		return;
	}
	
	g_engine_prec = prec;
	
	if (g_config_path)
		ini_putl("EnderCalc", "engine_prec", g_engine_prec, g_config_path);
	
	printf("Binary engine precision: %lu bits\n", g_engine_prec);
}

static void do_dprec(char *s) {
	long int prec = parse_prec(s);
	if (prec == -1) {
		print_prec_err(s);
		return;
	}
	
	g_dec_engine_prec = prec;
	
	if (g_config_path)
		ini_putl("EnderCalc", "dec_engine_prec", g_dec_engine_prec, g_config_path);
	
	printf("Decimal engine precision: %lu digits\n", g_dec_engine_prec);
}

static void do_prec(char *s) {
	long int prec = parse_prec(s);
	if (prec == -1) {
		print_prec_err(s);
		return;
	}
	
	g_dec_engine_prec = prec;
	g_engine_prec = prec * 5;
	
	if (g_config_path) {
		ini_putl("EnderCalc", "engine_prec", g_engine_prec, g_config_path);
		ini_putl("EnderCalc", "dec_engine_prec", g_dec_engine_prec, g_config_path);
	}
	
	printf("Decimal engine precision: %lu digits\n", g_dec_engine_prec);
	printf("Binary engine precision: %lu bits\n", g_engine_prec);
}

static void do_about(char *s) {
	puts("EnderCalc v1.0 by Sam Haskins");
	puts("(licensed under the GNU General Public License, version 3)");
	puts("=============================");
	puts("Open-source libaries:");
	puts("libbf 20200119 by Fabrice Bellard");
	puts("WinEditLine 2.206 by Paolo Tosco");
	puts("minIni 1.2b by Thiadmer Riemersma");
}

static void do_time(char *s) {
	g_time ^= 1;
	
	if (g_config_path)
		ini_putl("EnderCalc", "time", g_time, g_config_path);
	
	if (g_time)
		puts("Now timing math operations.");
	else
		puts("No longer timing math operations.");
}

static void do_show(char *s) {
	printf("Trigonometric functions use  : %s\n", g_use_degrees ? "Degrees" : "Radians");
	printf("Debug                        : %s\n", on_off(g_debug));
	printf("Parse debug                  : %s\n", on_off(g_parse_debug));
	printf("Timing                       : %s\n", on_off(g_time));
	printf("Suppress Scientific Notation : %s\n", on_off(g_suppress_scientific_notation));
	printf("Binary engine precision      : %lu bits\n", g_engine_prec);
	printf("Decimal engine precision     : %lu digits\n", g_dec_engine_prec);
}

// When compiled for Windows, we can actually open the help pdf
// Otherwise, just mention it
static void do_help(char *s) {
#ifdef HELP_USE_WIN32
	char path[MAX_PATH+20]; // reference.pdf could be longer than EnderCalc.exe - add a little extra space
	GetModuleFileName(GetModuleHandle(NULL), path, MAX_PATH);
	PathRemoveFileSpecA(path);
	PathAppendA(path, "reference.pdf");
	ShellExecuteA(0, "open", path, 0, 0, SW_SHOWDEFAULT);
#else
	puts("Please consult reference.pdf, available in the same directory as the executable, for help.");
#endif
}

static const cmd_t commands[] = {
	{ "parsedebug", 10, do_parsedebug  },
	{ "formatlast", 10, do_format_last },
	{ "clearmem"  , 8 , do_clear_mem   },
	{ "showmem"   , 7 , do_show_mem    },
	{ "degrees"   , 7 , do_degrees     },
	{ "radians"   , 7 , do_degrees     },
	{ "format"    , 6 , do_format      },
	{ "debug"     , 5 , do_debug       },
	{ "bprec"     , 5 , do_bprec       },
	{ "dprec"     , 5 , do_dprec       },
	{ "about"     , 5 , do_about       },
	{ "stats"     , 5 , do_stats       }, // stats.c
	{ "prec"      , 4 , do_prec        },
	{ "time"      , 4 , do_time        },
	{ "show"      , 4 , do_show        },
	{ "help"      , 4 , do_help        },
	{ "last"      , 4 , do_format_last },
	{ "mem"       , 3 , do_show_mem    },
};
static const unsigned int commands_len = 18;

// Arg is without the leading slash
// Acts only as a dispatcher
void exec_meta_command(char *s) {
	for (unsigned int i = 0; i < commands_len; i++) {
		const cmd_t *cmd = &commands[i];
		if (strncmp(cmd->str, s, cmd->len) == 0) {
			// reject constructions like /dprecenugasfhasf
			char cont = s[cmd->len];
			if (cont == ' ' || strlen(s) == cmd->len) {
				cmd->op(&s[cmd->len]);
				return;
			}
		}
	}
	
	if (!mem_run_command(s)) printf("%s: command not found\n", s);
}

// Config loading, woo - first the callback
static int ini_kv_callback(const char *section, const char *key, const char *value, const void *unused) {
	// First, check the section
	if (strcmp(section, "EnderCalc") == 0) {
		unsigned int bool_value = 1 ? strcmp(value, "1") == 0 : 0;
		long int prec_value = parse_prec(value);
		
		if (strcmp(key, "parse_debug") == 0)
			g_parse_debug = bool_value;
		if (strcmp(key, "debug") == 0)
			g_debug = bool_value;
		if (strcmp(key, "use_degrees") == 0)
			g_use_degrees = bool_value;
		if (strcmp(key, "time") == 0)
			g_time = bool_value;
		if (strcmp(key, "suppress_scientific_notation") == 0)
			g_suppress_scientific_notation = bool_value;
		
		if (strcmp(key, "engine_prec") == 0 && prec_value != -1)
			g_engine_prec = prec_value;
		if (strcmp(key, "dec_engine_prec") == 0 && prec_value != -1)
			g_dec_engine_prec = prec_value;
	}
	// the memory
	mem_try_load(section, key, value);
	
	return 1;
}

void command_load_config() {
	ini_browse(ini_kv_callback, 0, g_config_path);
}