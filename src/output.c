#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include "output.h"
#include "assertion-result.h"
#include "state.h"
#include "caught.h"

#define CAUGHT_COLOR_SUCCESS "\x1b[32m"            // ANSI green
#define CAUGHT_COLOR_BACKGROUND_SUCCESS "\x1b[42m" // ANSI background green
#define CAUGHT_COLOR_FAIL "\x1b[31m"               // ANSI red
#define CAUGHT_COLOR_BACKGROUND_FAIL "\x1b[41m"    // ANSI background red
#define CAUGHT_COLOR_WARNING "\x1b[33m"            // ANSI yellow
#define CAUGHT_COLOR_INFO "\x1b[34m"               // ANSI blue
#define CAUGHT_OUTPUT_BOLD "\x1b[1m"               // ANSI bold
#define CAUGHT_OUTPUT_RESET "\x1b[0m"              // ANSI reset

#define CAUGHT_OUTPUT_HEADER_WIDTH 80
#define CAUGHT_OUTPUT_HEADER_BOTTOM "                    A lightweight & simple C testing library\n\n" \
                                    "                      Copyright(c) 2024 Timothy Gonzalez\n\n"     \
                                    "================================================================================\n"

int caught_color_enabled = 0;

void caught_internal_initialize_color_output()
{
    const char *TERM = getenv("TERM");
    const char *COLOR = getenv("COLOR");

    if (TERM != NULL && strstr(TERM, "color") != NULL)
    {
        caught_color_enabled = 1;
    }

    if (COLOR != NULL)
    {
        caught_color_enabled = !!strcmp(COLOR, "0");
    }
}

void caught_output_success()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_COLOR_SUCCESS);
}
void caught_output_background_success()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_COLOR_BACKGROUND_SUCCESS);
}
void caught_output_fail()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_COLOR_FAIL);
}
void caught_output_background_fail()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_COLOR_BACKGROUND_FAIL);
}
void caught_output_info()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_COLOR_INFO);
}
void caught_output_bold()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_OUTPUT_BOLD);
}
void caught_output_reset()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_OUTPUT_RESET);
}

void caught_output_header()
{
    char *header_text;
    asprintf(&header_text, " Caught %s ", CAUGHT_VERSION_STRING);
    int equalsLeft = CAUGHT_OUTPUT_HEADER_WIDTH - strlen(header_text);
    int leftEquals = equalsLeft / 2;
    int rightEquals = equalsLeft - leftEquals;

    for (; leftEquals > 0; leftEquals--)
        putchar('=');

    printf("%s", header_text);
    free(header_text);

    for (; rightEquals > 0; rightEquals--)
        putchar('=');

    printf("\n\n%s", CAUGHT_OUTPUT_HEADER_BOTTOM);
    caught_output_bold();
    caught_output_info();
    printf("\nLoaded %i tests\n", caught_internal_state.tests_num);
    caught_output_reset();
}

void caught_output_internal_error(bool use_perror, char *fstr, va_list args)
{
    if (caught_color_enabled)
    {
        fprintf(stderr, "%s", CAUGHT_OUTPUT_BOLD CAUGHT_COLOR_BACKGROUND_FAIL CAUGHT_COLOR_FAIL);
    }
    fprintf(stderr, " %s ", "✖ ERROR");

    if (caught_color_enabled)
    {
        fprintf(stderr, "%s ", CAUGHT_OUTPUT_RESET CAUGHT_COLOR_FAIL CAUGHT_OUTPUT_BOLD);
    }

    vfprintf(stderr, fstr, args);

    if (use_perror && errno != 0)
    {
        fprintf(stderr, ": %s\n", strerror(errno));
        errno = 0; // Reset errno after printing it
    }

    if (caught_color_enabled)
    {
        fprintf(stderr, "%s", CAUGHT_OUTPUT_RESET);
    }

    exit(EXIT_FAILURE);
}

void caught_output_perrorf(char *fstr, ...)
{
    va_list args;
    va_start(args, fstr);
    caught_output_internal_error(1, fstr, args);
    va_end(args);
}

void caught_output_errorf(char *fstr, ...)
{
    va_list args;
    va_start(args, fstr);
    caught_output_internal_error(0, fstr, args);
    va_end(args);
}

void caught_output_status_tag(int pass)
{
    caught_output_bold();
    pass ? caught_output_success() : caught_output_fail();
    pass ? caught_output_background_success() : caught_output_background_fail();
    printf(caught_color_enabled ? " %s " : "%s", pass ? "PASS" : "FAIL");
    caught_output_reset();
    printf(" ");
}

void caught_output_assertion_result(caught_internal_assertion_result assertion_result)
{
    printf("\n");

    caught_output_status_tag(assertion_result.pass);
    printf("./%s:%i:\n", assertion_result.file, assertion_result.line);
    caught_output_info();
    printf("    %s\n", assertion_result.expression);
    caught_output_reset();

    const char *expected_statement = "expected";
    const char *to_be_statement = caught_operator_to_to_be_statement(assertion_result.operator);
    int statement_padding = (strlen(expected_statement) > strlen(to_be_statement)) ? strlen(expected_statement) : strlen(to_be_statement);

    printf("        %s:%*s ", expected_statement, statement_padding - (int)strlen(expected_statement), "");
    caught_output_success();
    printf("%s", assertion_result.lhs == NULL ? "NULL" : assertion_result.lhs);
    caught_output_reset();
    printf("\n        %s:%*s ", to_be_statement, statement_padding - (int)strlen(to_be_statement), "");
    assertion_result.pass ? caught_output_success() : caught_output_fail();
    printf("%s", assertion_result.rhs == NULL ? "NULL" : assertion_result.rhs);
    caught_output_reset();
    printf("\n");
}

void caught_output_test_summary(const char *test_name, int passed, int failed)
{
    int total = passed + failed;
    caught_output_status_tag(failed == 0);
    printf("%s: %i passed", test_name, passed);
    if (failed)
    {
        printf(", %i failed", failed);
    }
    printf(", %i total\n", total);
}

void caught_output_summary(const char *prefix, int passed, int failed)
{
    int total = passed + failed;
    printf("%s", prefix);
    caught_output_success();
    if (failed == 0)
    {
        caught_output_bold();
    }
    printf("%i passed", passed);
    caught_output_reset();
    printf("%s", ", ");
    if (failed > 0)
    {
        caught_output_bold();
        caught_output_fail();
        printf("%i failed", failed);
        caught_output_reset();
        printf("%s", ", ");
    }
    printf("%i total\n", total);
}

void caught_output_overall_result(int pass)
{
    caught_output_bold();
    pass ? caught_output_success() : caught_output_fail();
    puts(pass ? "All tests passed!" : "Some tests failed");
    caught_output_reset();
}
