#include "formatters.h"

#define _GNU_SOURCE
#include <stdio.h>

char *caught_internal_formatter_ptr(void *value)
{
    char *result;
    asprintf(&result, "%p", value);
    return result;
}

char *caught_internal_formatter_bool(bool value)
{
    return (value) ? strdup("true") : strdup("false");
}

char *caught_internal_formatter_int(int value)
{
    char *result;
    asprintf(&result, "%i", value);
    return result;
}

char *caught_internal_formatter_char(char value)
{
    char *result;
    asprintf(&result, "%c", value);
    return result;
}

char *caught_internal_formatter_str(char *value)
{
    if (value == NULL)
    {
        return NULL;
    }
    char *result;
    asprintf(&result, "\"%s\"", value);
    return result;
}
