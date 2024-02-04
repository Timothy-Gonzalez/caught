#include <stddef.h>
#include <stdio.h>
#include "output.h"

#define CAUGHT_COLOR_SUCCESS "\x1b[32m" // ANSI green
#define CAUGHT_COLOR_FAIL "\x1b[31m"    // ANSI red
#define CAUGHT_COLOR_WARNING "\x1b[33m" // ANSI yellow
#define CAUGHT_COLOR_INFO "\x1b[34m"    // ANSI blue
#define CAUGHT_OUTPUT_BOLD "\x1b[1m"    // ANSI bold
#define CAUGHT_OUTPUT_RESET "\x1b[0m"   // ANSI reset

#define CAUGHT_OUTPUT_HEADER "==================================== Caught ====================================\n\n" \
                             "                    A lightweight & simple C testing library\n\n"                     \
                             "                      Copyright(c) 2024 Timothy Gonzalez\n\n"                         \
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
void caught_output_fail()
{
    if (!caught_color_enabled)
        return;
    printf("%s", CAUGHT_COLOR_FAIL);
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
    printf("%s", CAUGHT_OUTPUT_HEADER);
    caught_output_bold();
    caught_output_info();
    printf("\nLoaded %i tests\n", caught_internal.tests_num);
    caught_output_reset();
}

void caught_output_status_tag(int pass)
{
    caught_output_bold();
    pass ? caught_output_success() : caught_output_fail();
    printf("%s", pass ? "PASS " : "FAIL ");
    caught_output_reset();
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
