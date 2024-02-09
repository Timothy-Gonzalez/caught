#define _GNU_SOURCE
#include <string.h>
#include <stdbool.h>
#include "fork.h"
#include "assertion-result.h"

#ifndef CAUGHT_FORMATTERS
#define CAUGHT_FORMATTERS

#define CAUGHT_INTERNAL_FORMATTER_ARRAY_INITAL_SIZE 16
#define CAUGHT_INTERNAL_FORMATTER_ARRAY_GROWTH_RATE 2

// Formatters take a value of a certain type
// they must return a dynamically allocated string that represents that value

char *caught_internal_formatter_ptr(void *value);
char *caught_internal_formatter_ptr_ptr(void **value);
char *caught_internal_formatter_ptr_array(void **value, ssize_t length);

char *caught_internal_formatter_bool(bool value);
char *caught_internal_formatter_bool_ptr(bool *value);
char *caught_internal_formatter_bool_array(bool *value, ssize_t length);

char *caught_internal_formatter_int(int value);
char *caught_internal_formatter_int_ptr(int *value);
char *caught_internal_formatter_int_array(int *value, ssize_t length);

char *caught_internal_formatter_char(char value);
char *caught_internal_formatter_char_ptr(char *value);
char *caught_internal_formatter_char_array(char *value, ssize_t length);

char *caught_internal_formatter_str(char *value);
char *caught_internal_formatter_str_ptr(char **value);
char *caught_internal_formatter_str_array(char **value, ssize_t length);

char *caught_internal_formatter_exit_status(caught_internal_process_status value);

#define CAUGHT_INTERNAL_FORMATTER_FORMAT(fstr, ...) \
    char *result;                                   \
    asprintf(&result, fstr, __VA_ARGS__);           \
    return result;
#define CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value) \
    if (value == NULL)                              \
    {                                               \
        return strdup("NULL");                      \
    }
#define CAUGHT_INTERNAL_FORMATTER_ARRAY(array_exp, array_length, formatter) \
    if (array_length <= 0)                                                  \
    {                                                                       \
        caught_output_errorf("Invalid length of array: %lu", array_length); \
    }                                                                       \
    char *result = malloc(CAUGHT_INTERNAL_FORMATTER_ARRAY_INITAL_SIZE);     \
    size_t result_size = 0;                                                 \
    size_t result_capacity = CAUGHT_INTERNAL_FORMATTER_ARRAY_INITAL_SIZE;   \
    result[0] = '{';                                                        \
    result[1] = ' ';                                                        \
    result[2] = '\0';                                                       \
    result_size += 2;                                                       \
    int i;                                                                  \
    for (i = 0; i < array_length; ++i)                                      \
    {                                                                       \
        char *formatted = formatter(array_exp[i]);                          \
        if (i != 0)                                                         \
        {                                                                   \
            char *old_formatted = formatted;                                \
            asprintf(&formatted, ", %s", old_formatted);                    \
            free(old_formatted);                                            \
        }                                                                   \
        size_t size = strlen(formatted);                                    \
        while (result_size + size + 3 > result_capacity)                    \
        {                                                                   \
            result_capacity *= CAUGHT_INTERNAL_FORMATTER_ARRAY_GROWTH_RATE; \
            result = realloc(result, result_capacity);                      \
        }                                                                   \
        memcpy(result + result_size, formatted, size);                      \
        result_size += size;                                                \
        free(formatted);                                                    \
    }                                                                       \
    result[result_size] = ' ';                                              \
    result[result_size + 1] = '}';                                          \
    result[result_size + 2] = '\0';                                         \
    return result;

#define CAUGHT_INTERNAL_FORMATTER_ARRAY_ALLOW_NULL_TERMINATOR(array_exp, array_length, formatter, terminator) \
    if (array_length < 0)                                                                                     \
    {                                                                                                         \
        array_length = -1;                                                                                    \
        do                                                                                                    \
        {                                                                                                     \
            array_length += 1;                                                                                \
        } while (array_exp[array_length] != terminator);                                                      \
    }                                                                                                         \
    CAUGHT_INTERNAL_FORMATTER_ARRAY(array_exp, array_length, formatter)

#endif
