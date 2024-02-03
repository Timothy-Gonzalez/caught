#include <stdio.h>

#include "caught.h"
#include "output.h"
#include "config.h"

int main()
{
    caught_internal_initialize_color_output();

    printf("\n");
    caught_output_header();

    int passed_tests = 0;
    for (int i = 0; i < caught_internal.tests_num; ++i)
    {
        int prev_assertions = caught_internal.assertions;
        int prev_passed_assertions = caught_internal.passed_assertions;

        caught_internal_test test = caught_internal.tests[i];

        test.handler();

        int this_assertions = caught_internal.assertions - prev_assertions;
        int this_passed_assertions = caught_internal.passed_assertions - prev_passed_assertions;
        int this_failed_assertions = this_assertions - this_passed_assertions;

        if (!this_failed_assertions)
        {
            passed_tests += 1;
        }

        int output = this_failed_assertions > 0;

#ifdef EXPLICIT_TEST_PASS
        output = 1;
#endif

        if (!output)
            continue;

        printf("\n");
        caught_output_test_summary(test.name, this_passed_assertions, this_failed_assertions);
    }

    int tests = caught_internal.tests_num;
    int failed_tests = tests - passed_tests;
    int passed_assertions = caught_internal.passed_assertions;
    int failed_assertions = caught_internal.assertions - caught_internal.passed_assertions;

    printf("\n");
    caught_output_summary("Tests:      ", passed_tests, failed_tests);
    caught_output_summary("Assertions: ", passed_assertions, failed_assertions);
    caught_output_overall_result(failed_tests == 0);

    free(caught_internal.tests);

    return failed_tests != 0;
}
