#include <stddef.h>
#define _GNU_SOURCE
#include <stdio.h>
extern int asprintf(char **strp, const char *fmt, ...);

#include "assertions.h"
#include "output.h"

void caught_internal_fancy_str(const char *str)
{
    if (str == NULL)
    {
        printf("NULL");
    }
    else
    {
        printf("%s", str);
    }
}

void caught_internal_handle_assertion(caught_internal_assertion *assertion)
{
    caught_internal.assertions += 1;
    if (assertion->pass)
    {
        caught_internal.passed_assertions += 1;
    }

    int show_regardless_of_pass = 0;

#ifdef EXPLICIT_ASSERTION_PASS
    show_regardless_of_pass = 1;
#endif

    if (!assertion->pass || show_regardless_of_pass)
    {
        caught_output_assertion(assertion);
    }

    free(assertion->expected);
    free(assertion->got);
}

#define CAUGHT_INTERNAL_READ_EXPECTED_AND_GET(type, format, expected_exp, got_exp) \
    type caught_internal_expected = (expected_exp);                                \
    type caught_internal_got = (got_exp);                                          \
    asprintf(&assertion.expected, format, caught_internal_expected);               \
    asprintf(&assertion.got, format, caught_internal_got);

int caught_internal_expect_equal_int(caught_internal_assertion *assertion, int expected, int got)
{
    char *format = "%d";
    asprintf(&assertion->expected, format, expected);
    asprintf(&assertion->got, format, got);

    return expected == got;
}
int caught_internal_expect_equal_char(caught_internal_assertion *assertion, char expected, char got)
{
    char *format = "'%c'";
    asprintf(&assertion->expected, format, expected);
    asprintf(&assertion->got, format, got);

    return expected == got;
}
int caught_internal_expect_equal_str(caught_internal_assertion *assertion, char *expected, char *got)
{
    char *format = "\"%s\"";
    asprintf(&assertion->expected, format, expected);
    asprintf(&assertion->got, format, got);

    int null_case = expected != got && (expected == NULL || got == NULL);
    return !null_case && (strcmp(expected, got) == 0);
}
