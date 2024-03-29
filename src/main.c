#include <stdio.h>

#include "caught.h"
#include "output.h"
#include "config.h"
#include "state.h"

int main()
{
    caught_internal_initialize_color_output();

    caught_output_printf("\n");
    caught_output_header();

    int passed_tests = 0;
    for (int i = 0; i < caught_internal_state.tests_num; ++i)
    {
        int prev_assertions = caught_internal_state.assertions;
        int prev_passed_assertions = caught_internal_state.passed_assertions;

        caught_internal_test test = caught_internal_state.tests[i];

        test.handler();

        if (caught_internal_state.original_stdin != -1)
        {
            caught_output_errorf("Must restore stdin after mocking it, did you forget to call RESTORE_STDIN()?");
        }
        if (caught_internal_state.original_stdout != -1)
        {
            caught_output_errorf("Must restore stdout after mocking it, did you forget to call RESTORE_STDOUT()?");
        }

        int this_assertions = caught_internal_state.assertions - prev_assertions;
        int this_passed_assertions = caught_internal_state.passed_assertions - prev_passed_assertions;
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

        caught_output_printf("\n");
        caught_output_test_summary(test.name, this_passed_assertions, this_failed_assertions);
    }

    int tests = caught_internal_state.tests_num;
    int failed_tests = tests - passed_tests;
    int passed_assertions = caught_internal_state.passed_assertions;
    int failed_assertions = caught_internal_state.assertions - caught_internal_state.passed_assertions;

    caught_output_printf("\n");
    caught_output_summary("Tests:      ", passed_tests, failed_tests);
    caught_output_summary("Assertions: ", passed_assertions, failed_assertions);
    caught_output_overall_result(failed_tests == 0);

    caught_internal_cleanup_state(true);

    return failed_tests != 0;
}
