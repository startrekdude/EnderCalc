#include <stdlib.h>

#include "parse.h"
#include "expr.h"

// Append a str slice of size 1 to the end of top
static void new_str_slice(vector_t top, char *s) {
	str_slice_t *str = (str_slice_t*) malloc(sizeof(str_slice_t));
	str->ptr = s;
	str->len = 1;
	token_tag((void**) &str, TOKEN_STR);
	vector_add(top, str);
}

// Turn a string into an expr composed of slices and sub expressions
// Error if too many opening brackets, or too many closing brackets
int match_brackets(char *s, vector_t *expr) {
	*expr = vector_new();
	vector_t stack = vector_new();
	vector_add(stack, *expr);
	
	while (*s != '\0') {
		vector_t top = (vector_t) vector_get_last(stack);
		
		if (*s == '(') {
			vector_t new = vector_new();
			vector_add(stack, new);
			token_tag((void**) &new, TOKEN_EXPR);
			vector_add(top, new);
		} else if (*s == ')') {
			if (vector_get_len(stack) == 1) {
				vector_delete(stack);
				return PARSE_UNMATCHED_CLOSE_BRACKET;
			}
			vector_remove(stack, vector_get_len(stack) - 1);
		} else if (vector_get_len(top) == 0) {
			new_str_slice(top, s);
		} else {
			void *last_tk = vector_get_last(top);
			unsigned int tag = token_untag(&last_tk);
			if (tag == TOKEN_STR) {
				str_slice_t *str = (str_slice_t*) last_tk;
				str->len++;
			} else {
				new_str_slice(top, s);
			}
		}
		s++;
	}
	
	// We should be at the top of the stack now
	if (vector_get_len(stack) > 1) {
		vector_delete(stack);
		return PARSE_UNMATCHED_OPEN_BRACKET;
	}
	
	vector_delete(stack);
	return PARSE_SUCCESS;
}