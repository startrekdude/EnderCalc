#include <stdio.h>
#include <sys/timeb.h>

#include "calc.h"
#include "command.h"
#include "engine.h"
#include "eval.h"
#include "expr.h"
#include "mem.h"
#include "parse.h"
#include "readline.h"

unsigned int g_debug = 0;
unsigned int g_suppress_scientific_notation = 0;
unsigned int g_time = 0;

// Run a single command, given as a string (does not free it)
static void run_cmd(char *cmd) {
	// meta commands (/prec, /debug) are handled elsewhere
	if (cmd[0] == '/') {
		exec_meta_command(&cmd[1]);
		return;
	}
	
	vector_t expr;
	int err = parse_expr(cmd, &expr);
	if (err == PARSE_SUCCESS) {
		if (g_debug) expr_print(expr);
		bf_t *result;
		err = eval_expr(expr, &result);
		if (err != EVAL_SUCCESS) {
			expr_delete(expr);
			printf("Evaluation error: ");
			eval_print_error(err);
			puts("");
			return;
		}
		vector_delete(expr); // leave the number intact
		engine_num_print(result, g_debug, g_suppress_scientific_notation);
		
		if (g_last_answer != 0) engine_num_free(g_last_answer);
		g_last_answer = result;
	} else {
		printf("Parse error: ");
		parse_print_error(err);
		puts("");
	}
}

static char* null_matches(const char *text, int state) {
	return 0;
}

// stub to disable readline's autocomplete - file names make no sense
static char** null_completion(const char *text, int start, int end) {
	rl_attempted_completion_over = 1;
	return rl_completion_matches(text, null_matches);
}

int main(int argc, char **argv) {
	puts("EnderCalc v1.0");
	
	engine_init();
	
	rl_attempted_completion_function = null_completion;
	
	// calculate and store the config file path
	g_config_path = (char*) malloc(512); // bigger than both win32's MAX_PATH and limits.h PATH_MAX
#ifdef _WIN32	
	char *env_home_drive = getenv("HOMEDRIVE");
	strcpy(g_config_path, env_home_drive);
	strcat(g_config_path, getenv("HOMEPATH"));
	strcat(g_config_path, "\\");
#else // unix-y system
	char *env_home = getenv("HOME");
	strcpy(g_config_path, env_home);
	strcat(g_config_path, "/");
#endif
	strcat(g_config_path, ".endercalc");
	command_load_config();
	
	while (1) {
		char *line = readline("? ");
		if (line[0] == '\0') {
			free(line);
			break;
		}
		add_history(line); // if it doesn't parse, all the more reason - they can go and fix it
		
		struct timeb start, stop;
		ftime(&start);
		
		run_cmd(line);
		
		ftime(&stop);
		unsigned int msec = (stop.time - start.time) * 1000 + stop.millitm - start.millitm;
		if (g_time && line[0] != '/') printf("Command completed in %d ms.\n", msec);
		
		free(line);
	}
	
	if (g_last_answer != 0) engine_num_free(g_last_answer);
	free_mem();
	free(g_config_path);
	engine_cleanup();
}