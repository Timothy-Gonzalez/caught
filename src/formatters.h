#define _GNU_SOURCE
#include <string.h>
#include <stdbool.h>
#include "fork.h"

#ifndef CAUGHT_FORMATTERS
#define CAUGHT_FORMATTERS

// Formatters take a value of a certain type
// they must return a dynamically allocated string that represents that value

char *caught_internal_formatter_ptr(void *value);
char *caught_internal_formatter_ptr_ptr(void **value);

char *caught_internal_formatter_bool(bool value);
char *caught_internal_formatter_bool_ptr(bool *value);

char *caught_internal_formatter_int(int value);
char *caught_internal_formatter_int_ptr(int *value);

char *caught_internal_formatter_char(char value);
char *caught_internal_formatter_char_ptr(char *value);

char *caught_internal_formatter_str(char *value);
char *caught_internal_formatter_str_ptr(char **value);

char *caught_internal_formatter_exit_status(caught_internal_process_status value);

#define CAUGHT_INTERNAL_FORMATTER_FORMAT(fstr, ...) \
    char *result;                                   \
    asprintf(&result, fstr, __VA_ARGS__);           \
    return result;
#define CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value) \
    if (value == NULL)                              \
    {                                               \
        return NULL;                                \
    }

#endif
