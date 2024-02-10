#include "formatters.h"

#define _GNU_SOURCE
#include <stdio.h>

char *caught_internal_formatter_ptr(void *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_FORMAT("%p", value);
}
char *caught_internal_formatter_ptr_ptr(void **value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_ptr(*value);
}
char *caught_internal_formatter_ptr_array(void **value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY(value, length, caught_internal_formatter_ptr)
}

char *caught_internal_formatter_bool(bool value)
{
    return (value) ? strdup("true") : strdup("false");
}
char *caught_internal_formatter_bool_ptr(bool *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_bool(*value);
}
char *caught_internal_formatter_bool_array(bool *value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY(value, length, caught_internal_formatter_bool)
}

char *caught_internal_formatter_int(int value)
{
    CAUGHT_INTERNAL_FORMATTER_FORMAT("%i", value);
}
char *caught_internal_formatter_int_ptr(int *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_int(*value);
}
char *caught_internal_formatter_int_array(int *value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY(value, length, caught_internal_formatter_int)
}

char *caught_internal_formatter_char(char value)
{
    if (value == '\0')
    {
        return strdup("'\\0'");
    }
    CAUGHT_INTERNAL_FORMATTER_FORMAT("'%c'", value)
}
char *caught_internal_formatter_char_ptr(char *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_char(*value);
}
char *caught_internal_formatter_char_array(char *value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY_ALLOW_NULL_TERMINATOR(value, length, caught_internal_formatter_char, '\0')
}

char *caught_internal_formatter_str(char *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_FORMAT("\"%s\"", value)
}
char *caught_internal_formatter_str_ptr(char **value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_str(*value);
}
char *caught_internal_formatter_str_array(char **value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY_ALLOW_NULL_TERMINATOR(value, length, caught_internal_formatter_str, NULL)
}

char *caught_internal_formatter_exit_status(caught_internal_process_status value)
{
    const char *type = value.type ? "Signal" : "Exit code";
    if (value.status_str)
    {
        CAUGHT_INTERNAL_FORMATTER_FORMAT("%s (%i)", value.status_str, value.status)
    }
    CAUGHT_INTERNAL_FORMATTER_FORMAT("%s %i", type, value.status)
}
