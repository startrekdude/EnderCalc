#ifndef _INCLUDED_COMMAND_H
#define _INCLUDED_COMMAND_H

// Various meta-command that influence parts of the calculator
// Set precision, show extra debug information, etc

extern char* g_config_path;

typedef void (cmd_func_t)(char *s);

typedef struct {
	char str[16]; // name of the command w/o slashes 
	unsigned int len; // length of name
	cmd_func_t *op; // pointer to executing function
} cmd_t;

void command_load_config();
void exec_meta_command(char *s);

#endif