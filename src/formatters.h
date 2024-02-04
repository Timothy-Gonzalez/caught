#define _GNU_SOURCE
#include <string.h>
#include <stdbool.h>

#ifndef CAUGHT_FORMATTERS
#define CAUGHT_FORMATTERS

// Formatters take a value of a certain type
// they must return a dynamically allocated string that represents that value

char *caught_internal_formatter_ptr(void *value);
char *caught_internal_formatter_bool(bool value);
char *caught_internal_formatter_int(int value);
char *caught_internal_formatter_char(char value);
char *caught_internal_formatter_str(char *value);

#endif
